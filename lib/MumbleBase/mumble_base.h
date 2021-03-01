#pragma once

#include "mumble.pb.h"
#define DESINCT_MSG_COUNT 26


// #define DEBUG

#ifdef DEBUG
    #define DebugLn(x) Serial.println(x)
    #define DebugP(x) Serial.print(x)
    #define DebugHex(x) Serial.print(x, HEX); Serial.print(" ") 
#else
    #define DebugLn(x) (void)0
    #define DebugP(x) (void)0
    #define DebugLn(x) (void)0
#endif

#define ChannelCount 32

/**
 * @brief Enum for all of mumbles Message Types
 * 
 */
typedef enum {Version = 0, UDPTunnel, Authenticate,Ping,Reject,ServerSync,ChannelRemove,ChannelState,UserRemove,UserState,BanList,TextMessage,
    PermissionDenied,ACL,QueryUsers,CryptSetup,ContextActionModify,ContextAction,UserList,VoiceTarget,PermissionQuery,CodecVersion,UserStats,
    RequestBlob,ServerConfig,SuggestConfig} MessageType;


//definded in mumble.cpp
const extern pb_msgdesc_t* FieldLookupTable[DESINCT_MSG_COUNT]; // maps each Message Types number to a nanopb desiptions for de/encoding
const extern char* MsgNameLookup[DESINCT_MSG_COUNT]; // maps Message Types number to it's name as string

/**
 * @brief simple data struct for a user
 * countains name, current channel and the unique session id
 */
typedef struct 
{
    int id;
    int channel;
    char name[32];
} MumbleUser;

/**
 * @brief simple data struct for a user
 * countains name, unique id, and user count in the channel
 */
typedef struct 
{
    int id;
    char name[32];
    int userCount;
} MumbleChannel;

/**
 * @brief Union type of all MumbleProto messages
 * used to save memory and buffer the last message
 */
typedef union
{
    MumbleProto_Version version;
    MumbleProto_UDPTunnel udpTunnel;
    MumbleProto_Authenticate auchenticate;
    MumbleProto_Ping ping;
    MumbleProto_Reject reject;
    MumbleProto_ServerSync serverSync;
    MumbleProto_ChannelRemove channelRemove;
    MumbleProto_ChannelState channelState;
    MumbleProto_UserRemove userRemove;
    MumbleProto_UserState userState;
    MumbleProto_BanList banList;
    MumbleProto_TextMessage textMessage;
    MumbleProto_PermissionDenied permissionDenied;
    MumbleProto_ACL acl;
    MumbleProto_QueryUsers queryUsers;
    MumbleProto_CryptSetup cryptSetup;
    MumbleProto_ContextActionModify contextActionModify;
    MumbleProto_ContextAction contextAction;
    MumbleProto_UserList userList;
    MumbleProto_VoiceTarget voiceTarget;
    MumbleProto_PermissionQuery permissionQuery;
    MumbleProto_CodecVersion codecVersion;
    MumbleProto_UserStats userStats;
    MumbleProto_RequestBlob requestBlob;
    MumbleProto_ServerConfig serverConfig;
    MumbleProto_SuggestConfig suggestConfig;

} MumbleMessageMem;


