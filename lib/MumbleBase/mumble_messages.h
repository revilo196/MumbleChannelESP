#pragma once
#include "mumble_base.h"
#include <vector>

class MumbleMessageListener;

/**
 * @brief Base class for a provider of mumble control messages 
 */
class MumbleMessageProvider
{
private:
    typedef std::vector<std::pair<MumbleMessageListener*,MessageType>> NotifyList;
    NotifyList listeners;
protected:
    void update(MessageType t, const MumbleMessageMem * m ) const;
public:
   void add_Listener(MumbleMessageListener* l,MessageType t);
   void remove_Listener(MumbleMessageListener* l);
};

/**
 * @brief base interface for a listent for  mumble control messages 
 */
class MumbleMessageListener
{
public:
    virtual void notify(MessageType t, const MumbleMessageMem * m ) = 0;
};



/**
 * @brief simple impemantatin of a  MumbleMessageListener that calls a function pointer
 */
class MumbleMessageListenerFunc : public MumbleMessageListener
{   
public:
    typedef void(*MsgListenFunction)(MessageType t, const MumbleMessageMem * m);
private:
    MsgListenFunction func;
public:
    MumbleMessageListenerFunc( MsgListenFunction myfunc) {func = myfunc; }
    virtual void notify(MessageType t, const MumbleMessageMem * m ) override { func(t,m); };
};