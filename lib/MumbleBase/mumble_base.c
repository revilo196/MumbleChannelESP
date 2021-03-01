#include "mumble_base.h"

const pb_msgdesc_t* FieldLookupTable[DESINCT_MSG_COUNT] = {MumbleProto_Version_fields, MumbleProto_UDPTunnel_fields,MumbleProto_Authenticate_fields,
    MumbleProto_Ping_fields,MumbleProto_Reject_fields,MumbleProto_ServerSync_fields,MumbleProto_ChannelRemove_fields,MumbleProto_ChannelState_fields,
    MumbleProto_UserRemove_fields,MumbleProto_UserState_fields, MumbleProto_BanList_fields, MumbleProto_TextMessage_fields, 
    MumbleProto_PermissionDenied_fields, MumbleProto_ACL_fields, MumbleProto_QueryUsers_fields, MumbleProto_CryptSetup_fields, 
    MumbleProto_ContextActionModify_fields, MumbleProto_ContextAction_fields, MumbleProto_UserList_fields, MumbleProto_VoiceTarget_fields, 
    MumbleProto_PermissionQuery_fields, MumbleProto_CodecVersion_fields, MumbleProto_UserStats_fields, MumbleProto_RequestBlob_fields, 
    MumbleProto_ServerConfig_fields, MumbleProto_SuggestConfig_fields};

const char* MsgNameLookup[DESINCT_MSG_COUNT] = {"Version", "UDPTunnel", "Authenticate","Ping","Reject","ServerSync","ChannelRemove","ChannelState","UserRemove","UserState","BanList","TextMessage",
    "PermissionDenied","ACL","QueryUsers","CryptSetup","ContextActionModify","ContextAction","UserList","VoiceTarget","PermissionQuery","CodecVersion","UserStats",
    "RequestBlob","ServerConfig","SuggestConfig"};