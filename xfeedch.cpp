#include "xfeedch.h"


const ChannelInfo& XFeedCh::chInfo() const
{
    return chInfo_;
}

void XFeedCh::setChInfo(const ChannelInfo &newChInfo)
{
    chInfo_ = newChInfo;
}
