#pragma once
#include "mumble_messages.h"

class DummyMsgProvider : public MumbleMessageProvider
{
public:
    MumbleMessageMem message;
    MessageType type;
    void send(){  update(type, &message); }
    void send(MessageType t, const MumbleMessageMem * m){  update(t, m); }
    void send(const MumbleProto_UserState * msg) {type=UserState; message.userState = (*msg); send();}
    void send(const MumbleProto_UserRemove * msg) {type=UserRemove; message.userRemove = (*msg); send();}
    void send(const MumbleProto_ChannelState * msg) {type=ChannelState; message.channelState = (*msg); send();}

};