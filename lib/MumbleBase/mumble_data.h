#pragma once

#include <map>
#include "mumble_base.h"
#include "mumble_notify.h"
#include "mumble_messages.h"
#include <vector>

//shorthand alias for maps and vectors
typedef std::map<int,MumbleChannel> MumbleChannelMap;
typedef std::map<int,MumbleUser>  MumbleUserMap;
typedef std::vector<int> IdList;


/**
 * @brief contains, manages and updates a database of channel and users of the mumble server
 */
class MumbleData : public EventProvider, public MumbleMessageListener
{
private:
    MumbleUserMap userMap;
    MumbleChannelMap channelMap;
    MumbleMessageProvider * p;
    void updateChannelCounts();
    void updateUser(const MumbleProto_UserState* usermsg);
    void updateChannel(const MumbleProto_ChannelState* channelmsg);
    void updateUserRemove(const MumbleProto_UserRemove* userremmsg);
public:
    MumbleData(MumbleMessageProvider * prov);
    ~MumbleData(){p->remove_Listener(this);}

    // used for testing reset
    inline void clear() {userMap.clear(); channelMap.clear();}

    // event input
    virtual void notify(MessageType t, const MumbleMessageMem * m );

    // data access
    MumbleChannel getChannel(int id) const;
    MumbleUser getUser(int id) const; 
    IdList getAllChannelIDs() const;
    IdList getAllUserIDs() const;
    int findUserName(const char * name) const;
    int findChannelName(const char * name) const;

    
};
