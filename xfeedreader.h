#ifndef XFEEDREADER_H
#define XFEEDREADER_H

#include <QObject>
#include "xfeedch.h"
#include "channelinfo.h"
class XFeedReader: public QObject
{
    Q_OBJECT
public:
    XFeedReader() = default;
    virtual ~XFeedReader() = default;

    virtual void addChannel(ChannelInfo& xfeedCh) = 0;
    virtual void readFeed() = 0;

};

#endif // XFEEDREADER_H
