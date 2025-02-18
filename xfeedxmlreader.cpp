#include "xfeedxmlreader.h"

XFeedXMLReader::XFeedXMLReader() {}

void XFeedXMLReader::addChannel(ChannelInfo& chInfo)
{

    channelList_.push_back(std::make_shared<XFeedCh>(chInfo));
}

void XFeedXMLReader::readFeed()
{

}
