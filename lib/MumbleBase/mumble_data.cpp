#include "mumble_data.h"
#include <algorithm>
#include <functional>
#include <string>


/**
 * @brief Construct a new Mumble Data:: Mumble Data object
 * attaches to the given MumbleMessageProvider
 * @param prov MumbleMessageProvider to get incomming messages from
 */
MumbleData::MumbleData(MumbleMessageProvider * prov) {
    this->p = prov;
    this->p->add_Listener(this,UserState);
    this->p->add_Listener(this,UserRemove);
    this->p->add_Listener(this,ChannelState);
    this->p->add_Listener(this,ChannelRemove);
}

/**
 * @brief receive Messages from a MumbleMessageProvider
 * updates the database with the incomming information
 * 
 * TODO: Handle ChannelRemove messages
 * 
 * @param t Type of the Mumble message
 * @param m MumbleMessageMem Union only valid when interpeted as the given type
 */
void MumbleData::notify(MessageType t, const MumbleMessageMem * m ) {

    switch (t)
    {
        case UserState:
            updateUser(&m->userState);
            break;

        case UserRemove:
            updateUserRemove(&m->userRemove);
            break;

        case ChannelState:
            updateChannel(&m->channelState);
            break;

        case ChannelRemove:
            break;
        default:
            break;
    }


}

/**
 * @brief use incomming ChannelState to update the channel database
 * 
 * @param channelmsg ChannelState Message
 */
void MumbleData::updateChannel(const MumbleProto_ChannelState* channelmsg) {

    if(channelmsg->has_channel_id) {

        size_t cid =  channelmsg->channel_id;
        
        if(channelMap.count(cid) == 0 ) {
            MumbleChannel c = {};
            channelMap[cid] = c;
        }; 

        channelMap[cid].id = cid;
        
        if(channelmsg->has_name) {
            strncpy(channelMap[cid].name, channelmsg->name, std::min(sizeof(MumbleChannel::name),  sizeof(channelmsg->name)));
        }
        
        update((int)ChannelState); // emit event
    }
}

/**
 * @brief use incomming UserState message to update user database
 * 
 * @param usermsg UserState Message
 */
void MumbleData::updateUser(const MumbleProto_UserState* usermsg) {
    if(usermsg->has_session) {
        
        size_t uid =  usermsg->session;


        if(userMap.count(uid) == 0 ) {
            MumbleUser c = {};
            userMap[uid] = c;
        }; 

        userMap[uid].id = uid;

        if(usermsg->has_name) {
            strncpy(userMap[uid].name, usermsg->name, std::min(sizeof(MumbleUser::name),  sizeof(usermsg->name)));
        }

        if(usermsg->has_channel_id) {
            userMap[uid].channel = usermsg->channel_id;

            this->updateChannelCounts();
        }

        update((int)UserState); //emit Event
    }
}

/**
 * @brief use incomming UserState message to update user database
 * 
 * @param userremmsg 
 */
void MumbleData::updateUserRemove(const MumbleProto_UserRemove* userremmsg) {
   userMap.erase(userremmsg->session);
   this->updateChannelCounts();
   update((int)UserRemove); // emit event
}


/**
 * @brief count the users in each channel
 * currently resets the count every time
 * TODO:only update the counts not reset every time
 */
void MumbleData::updateChannelCounts() {

    for (auto it = channelMap.begin(); it != channelMap.end(); it++)
    {
        it->second.userCount = 0;
    }

    for (auto it = userMap.cbegin(); it != userMap.cend(); it++)
    {
        channelMap[it->second.channel].userCount++;
    }
    update(ChannelCount); //emit event
}

/**
 * @brief database access to get a list of all channel Ids
 * 
 * @return IdList  std::vector<int> list of all channel id
 */
IdList MumbleData::getAllChannelIDs() const {
    IdList ids(channelMap.size());
    int i = 0;
    for (auto it = channelMap.cbegin(); it != channelMap.cend(); it++)
    {
        ids[i] = it->second.id;
        i++;
    }
        
    return ids;
}

/**
 * @brief database access to get a list of all unique user SessionIds
 * 
 * @return IdList  std::vector<int> list of all user ids
 */
IdList MumbleData::getAllUserIDs() const {
    IdList ids(userMap.size());
    int i = 0;
    for (auto it = userMap.cbegin(); it != userMap.cend(); it++)
    {
        ids[i] = it->second.id;
        i++;
    }
        
    return ids;
}

/**
 * @brief try to lookup a username in the databse
 * 
 * @param name username to lookup
 * @return int unique session id of the user  or -1 if no user found
 */
int MumbleData::findUserName(const char * name) const {

    for (auto it = userMap.cbegin(); it != userMap.cend(); it++)
    {
        if(strcmp(it->second.name, name) == 0) {
            return it->second.id;
        }

    }
    return -1;
}  

/**
 * @brief try to lookup a channel in the databse
 * 
 * @param name channel to lookup
 * @return int unique id of the channel  or -1 if no channel found
 */
int MumbleData::findChannelName(const char * name) const {

    for (auto it = channelMap.cbegin(); it != channelMap.cend(); it++)
    {
        if(strcmp(it->second.name, name) == 0) {
            return it->second.id;
        }

    }
    return -1;
}

//lookup user info by id
MumbleUser  MumbleData::getUser(int id) const {
    MumbleUserMap::const_iterator it;
    it = userMap.find(id);
    if(it != userMap.cend()) {
        return it->second;
    } else {
        // not found
        MumbleUser u;
        u.id = -1;
        return u;
    }
}

//lookup channel info by id
MumbleChannel  MumbleData::getChannel(int id) const {
    MumbleChannelMap::const_iterator it;
    it = channelMap.find(id);
    if(it != channelMap.cend()) {
        return it->second;
    } else {
        //Not found
        MumbleChannel c;
        c.id = -1;
        return c;
    }
}