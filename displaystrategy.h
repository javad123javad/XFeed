#ifndef DISPLAYSTRATEGY_H
#define DISPLAYSTRATEGY_H
#include "channelinfo.h"

class DisplayStrategy
{
public:
    virtual ~DisplayStrategy() = default;
    virtual void display(const ChannelInfo& channel, QWidget* container = nullptr) = 0;
};

#endif // DISPLAYSTRATEGY_H
