
#include "mumble_messages.h"

/**
 * @brief notify all listeners matching the MessageType
 */
void MumbleMessageProvider::update(MessageType t, const MumbleMessageMem * m ) const{
    for (auto &&i : listeners)
    {
        if(i.second == t) {
            i.first->notify(t, m);
        }
    }
}

/**
 * @brief add a listeners to receive notifications
 */
void MumbleMessageProvider::add_Listener(MumbleMessageListener* l,MessageType t) {
    listeners.emplace_back(std::make_pair(l,t));
}

/**
 * @brief remove all connections to the given  MumbleMessageListener
 * if MumbleMessageListener listens for multible MessageTypes all are removed
 */
void MumbleMessageProvider::remove_Listener(MumbleMessageListener* l) {

    NotifyList::iterator it = listeners.begin();
    while ( it != listeners.end())
    {
        if (it->first == l) {
            it = listeners.erase(it);
        } else {
            it++;
        }
    }
}