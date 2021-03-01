#include "mumble_channelcounter.h"

MumbleChannelThreasholdProvider::MumbleChannelThreasholdProvider(MumbleData * data, std::string name, int thres) {
    _data = data;
    _prov = data;
    _name = name;
    _thres = thres;
    channelId = -1;
}

MumbleChannelThreasholdProvider::MumbleChannelThreasholdProvider(MumbleData * data, int channel, int thres) {
    _data = data;
    _prov = data;
    _name = "";
    _thres = thres;
    channelId = channel;
}

MumbleChannelThreasholdProvider::MumbleChannelThreasholdProvider(const MumbleData * data, EventProvider * prov, std::string name, int thres) {
    _data = data;
    _prov = prov;
    _name = name;
    _thres = thres;
    channelId = -1;
}

MumbleChannelThreasholdProvider::MumbleChannelThreasholdProvider(const MumbleData * data, EventProvider * prov, int channel, int thres) {
    _data = data;
    _prov = prov;
    _name = "";
    _thres = thres;
    channelId = channel;
}

MumbleChannelThreasholdProvider::~MumbleChannelThreasholdProvider() {
    _prov->remove_Listener(this);
}

bool MumbleChannelThreasholdProvider::connect() {
    _prov->add_Listener(this,ChannelCount);
    if (channelId < 0)
    {    
        channelId = _data->findChannelName(_name.c_str());
    }
    return channelId >= 0;
}


void MumbleChannelThreasholdProvider::notify(int msg) {

    MumbleChannel c =  _data->getChannel(channelId);
    if(c.id == -1) return; //channel not found

    if(c.userCount >= _thres) {
        update(MumbleChannelThreasholdProvider::OVEREQU);
    } else {
        update(MumbleChannelThreasholdProvider::UNDER);
    }
}