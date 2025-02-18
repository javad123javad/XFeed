#ifndef XFEEDCH_H
#define XFEEDCH_H
#include "xfeedchannel.h"
#include "channelinfo.h"

class XFeedCh: public XFeedChannel
{
public:
    explicit XFeedCh(ChannelInfo& chInfo): chInfo_(chInfo){}
    virtual ~XFeedCh() = default;

    const ChannelInfo& chInfo() const;
    void setChInfo(const ChannelInfo &newChInfo);

protected:

    ChannelInfo chInfo_;
};

#endif // XFEEDCH_H
