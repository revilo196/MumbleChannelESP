#include "mumble_notify.h"


void EventProvider::update(int eventType) const{
    for (auto &&i : listeners)
    {
        if(i.second == eventType) {
            i.first->notify(eventType);
        }
    }
}

void EventProvider::add_Listener(EventListener* l,int eventType) {
    listeners.emplace_back(std::make_pair(l,eventType));
}

void EventProvider::remove_Listener(EventListener* l) {

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