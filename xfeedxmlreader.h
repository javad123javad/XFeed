#ifndef XFEEDXMLREADER_H
#define XFEEDXMLREADER_H

#include "xfeedreader.h"
#include "xfeedch.h"
#include <memory.h>
class XFeedXMLReader : public XFeedReader
{
public:
    XFeedXMLReader();
    virtual ~XFeedXMLReader() = default;
    void addChannel(ChannelInfo& chInfo) override;
    void readFeed()  override;

private:
    QVector< std::shared_ptr<XFeedCh>> channelList_;
};

#endif // XFEEDXMLREADER_H
