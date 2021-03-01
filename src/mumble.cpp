#include "mumble.h"
#include <pb_encode.h>
#include <pb_decode.h>


/**
 * @brief Construct a new Mumble:: Mumble object 
 * 
 * init a headles Mumble client  
 * 
 * inits default messages and message lookup table
 * 
 * @param hostname hostname of the mumble server to conenct to
 * @param port mumble server port
 * @param client Arduino WiFiSecureClient pointer
 */
MumbleConnection::MumbleConnection(const char *hostname, int port, const char *username, WiFiClientSecure * client)
{
    this->hostname = hostname;
    this->port = port;
    this->client = client;
    messageType = Version;
    messageBytes = 0;


    //fill some Client Messages
    MumbleVersion version;
    version.version.major = 1;
    version.version.minor = 3;
    version.version.patch = 3;
    versionMsg.version = version.combined;
    versionMsg.has_version = true;
    strncpy(versionMsg.release,  "1.3.3", 6);
    versionMsg.has_release = true;

    strncpy(versionMsg.os, "Arduino", 8);
    versionMsg.has_os = true;
    strncpy(versionMsg.os_version, "ArduinoESP32", 13);
    versionMsg.has_os_version = true;

    strncpy(authMsg.username, username, 32);
    authMsg.has_username = true;

    //read only access connect
}

MumbleConnection::~MumbleConnection()
{
}

/**
 * @brief send a ping to the Mumble server to keep the connection running
 * 
 */
void MumbleConnection::send_ping() {
    reset_message();
    message.ping = pingMsg;
    send_msg(Ping);
}


/**
 * @brief read all controll messages available in TLS client
 * 
 * parse and handle every incoming msg's
 * 
 * calls parse_incomming_mesage() and work_incomming_message()
 * for every reveived message 
 * 
 * @return (>0) error, count of messages received
 */
int MumbleConnection::read_available() {

    if(client->available() == 0) {return 0;} //return if no data is available
    
    int len = client->readBytes(buffer, BUF_LEN);

    if (len < 0) {
        // read error
        return -3;
    }

    char * read_ptr = buffer;
    
    int msg_counter = 0;    
    //Run as long as there are Packages in the buffer 
    while (len >= 6)
    {
        #ifdef DEBUG_READ
            DebugP("Bytes in buffer: ");
            DebugLn(len);
        #endif

        //READ Header
        messageType = (MessageType) (read_ptr[0] << 8 | read_ptr[1]);
        messageBytes = read_ptr[2] << 24 | read_ptr[3] << 16 | read_ptr[4] << 8 | read_ptr[5];

        //increment the bufferpointer  
        read_ptr += 6;
        //decrement the available data in buffer
        len-=6;
        
        #ifdef DEBUG_READ
            DebugP("Parsing Type: ");
            DebugP(MsgNameLookup[(int)messageType]);
            DebugP(" Size: ");
            DebugLn(messageBytes);
        #endif
    
        assert(len >= messageBytes);

        //parse the message;
        int msg = parse_incomming_message(read_ptr, messageBytes);

        if(msg >= 0) {
            // only work on successful messages
            this->update(this->messageType, &message);
        }

        // go to next message in the package if existst
        len-= messageBytes;        //increment the bufferpointer  
        read_ptr += messageBytes;  //decrement the available data in buffer
        msg_counter++; //count messages
    }
    
    return msg_counter;

}

/**
 * @brief parse an incoming protobuffer packet to its coresponding data struct
 * 
 * @param data datapointer to the protobuffer packet
 * @param length length of the packet
 * @return MessageType or -1 on error
 */
int MumbleConnection::parse_incomming_message(char* data, int length) {
    
    pb_istream_t stream = pb_istream_from_buffer((uint8_t*) data, length);
    bool status;
 
    //get message type description from table
    const pb_msgdesc_t * np_type = FieldLookupTable[(int) this->messageType];

        
    status = pb_decode(&stream, np_type, &message);

    if(status) {
        return this->messageType;
    } else {
        printf("Decoding failed: %s\n", PB_GET_ERROR(&stream));
        return -1;
    }

}


/**
 * @brief Connect to the Mumble server
 */
void MumbleConnection::connect() {

    client->setInsecure(); //skip verification 
    
    DebugLn("Connect TLS");
    if (!client->connect(hostname, port)){
        DebugLn("Connection failed!");
    }   

    while (client->available() == 0)
    {
        //Wait for TLS connection
        delay(500);
        DebugLn(".");
    }


    DebugLn("Send_Version");
    message.version = versionMsg;
    this->send_msg(Version);

    DebugLn("Send_Auth");
    message.auchenticate = authMsg;
    this->send_msg(Authenticate);

    // wait for messages
    for (size_t i = 0; i < 3; i++)
    {
        DebugLn("Read_Server Response");
        read_available();
        delay(1000);
    }

}

/**
 * @brief send a control message to the mumble server
 * 
 * send predefined control messages to the mumble server
 * 
 * @param type message type
 */
void MumbleConnection::send_msg(MessageType type) {

    int typeID = (int)type;

    #ifdef DEBUG_SEND
    DebugP("Send MessageType: "); 
    DebugLn(MsgNameLookup[typeID]);
    #endif

    uint8_t* dest_ptr = (uint8_t*)(buffer+6);
    pb_ostream_t stream = pb_ostream_from_buffer(dest_ptr, BUF_LEN-6);

    bool status = pb_encode(&stream, FieldLookupTable[typeID], &message);
    assert(status);
    size_t message_length = stream.bytes_written;

    //build package header with correct byte order
    buffer[0] = (typeID >> 8) & 0xFF;
    buffer[1] = (typeID >> 0) & 0xFF;
    buffer[2] = (message_length >> 24) & 0xFF;
    buffer[3] = (message_length >> 16) & 0xFF;
    buffer[4] = (message_length >> 8) & 0xFF;
    buffer[5] = (message_length >> 0) & 0xFF;

    #ifdef DEBUG_SEND_PACKAGE
    DebugLn("message_length");
    DebugLn(message_length);

    DebugLn("MSG_HEADER:");
    for (size_t i = 0; i < 6; i++)
    {
       DebugHex(buffer[i]);
    }
    DebugLn();

    DebugLn("MSG_PROTO_PAYLOAD:");
    for (size_t i = 0; i < message_length; i++)
    {
       DebugHex(buffer[i+6]);
    }
    DebugLn();
    #endif

    int byte_send = client->write_P(buffer,6+message_length);
    assert(byte_send == 6+message_length);
}
