#ifndef DISPLAYSTRATEGY_H
#define DISPLAYSTRATEGY_H
#include "channelinfo.h"

class DisplayStrategy
{
public:
    virtual ~DisplayStrategy() = default;
    virtual void display(const ChannelInfo& channel, QWidget* container = nullptr) = 0;
    virtual bool isMediaPlaySupported() = 0;


    /* Media controls */
    virtual void play(){};
    virtual void stop(){};
    virtual void setMuted(bool muted){};
    virtual void setVolume(int vol){};
    virtual void displayMetaData(const ChannelInfo& channel){};
};

#endif // DISPLAYSTRATEGY_H
