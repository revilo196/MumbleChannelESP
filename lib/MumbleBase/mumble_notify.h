#pragma once
#include <vector>

class EventListener;

/**
 * @brief General Event Provider Base Class
 */
class EventProvider
{
    typedef std::vector<std::pair<EventListener*,int>> NotifyList;
private:
    NotifyList listeners;
protected:
    void update(int eventType) const;
public:
    void add_Listener(EventListener* l,int eventType);
    void remove_Listener(EventListener* l);
};


/**
 * @brief simple interface for a General event Listener
 */
class EventListener
{
public:
    virtual void notify(int msg) = 0;
};

/**
 * @brief simple EventListener implementaions that calls a function pointer
 */
class EventListenerFunc : public EventListener
{
    void(*fuc)();
public:
    EventListenerFunc( void(*myfuc)()) {fuc = myfuc; }
    virtual void notify(int msg) override { fuc(); };
};