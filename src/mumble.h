#pragma once 

#include <WiFiClientSecure.h>
#include <Arduino.h>
#include "mumble_base.h"
#include "mumble_messages.h"

#define BUF_LEN 2048

#ifdef DEBUG
    // #define DEBUG_SEND_PACKAGE
    // #define DEBUG_SEND
    // #define DEBUG_READ
    // #define DEBUG_UPDATE
#endif

union MumbleVersion
{
    uint32_t combined;
    struct
    {
        uint8_t patch;
        uint8_t minor;
        uint8_t major;
        uint8_t reserved;
    }  version;
    
};


class MumbleConnection : public MumbleMessageProvider
{
private:
    //server connection
    const char *hostname;
    int port;
    WiFiClientSecure * client;

    // Memory reserved for mumble messages
    MumbleMessageMem message;
    void inline reset_message() { memset(&message, 0, sizeof(MumbleMessageMem));}

    //Predefined Messages:
    MumbleProto_Version versionMsg;
    MumbleProto_Ping pingMsg;
    MumbleProto_Authenticate authMsg;


    //last handled message
    MessageType messageType;
    int messageBytes;

    //send and receive buffer
    char buffer[BUF_LEN];

    int parse_incomming_message(char* data, int length);
    void send_msg(MessageType type);

public:
    MumbleConnection(const char *hostname, int port, const char *username, WiFiClientSecure * client);
    ~MumbleConnection();

    void connect();
    void send_ping();
    int read_available();
};

