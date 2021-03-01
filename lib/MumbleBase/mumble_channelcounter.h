#include "mumble_notify.h"
#include "mumble_data.h"
#include <string>


/**
 * @brief watch a channel count, emit signal when over / under a set limt
 * 
 * connects to a MumbleData(EventProvider) instance, and listens for ChannelCount events.
 * will provide OVEREQU/UNDER events
 * 
 */
class MumbleChannelThreasholdProvider : public EventListener, public EventProvider
{
public:
    enum EventType{ OVEREQU=33, UNDER=34 };

private:
    const MumbleData * _data;
    EventProvider * _prov;
    std::string _name;
    int channelId;
    int _thres;

    virtual void notify(int msg) override;
public:
    MumbleChannelThreasholdProvider(MumbleData * data, std::string name, int thres);
    MumbleChannelThreasholdProvider(const MumbleData * data, EventProvider * prov, std::string name, int thres);
    MumbleChannelThreasholdProvider(MumbleData * data, int channelID, int thres);
    MumbleChannelThreasholdProvider(const MumbleData * data, EventProvider * prov, int channelID , int thres);

    /**
     * @brief connects to MumbleData, and looks up the provided channel name and resolves its ids
     * @return false connection error / name not found
     */
    bool connect();
    
    ~MumbleChannelThreasholdProvider();
};

