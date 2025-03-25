#include "xfeedxmlreader.h"
XFeedXMLReader::XFeedXMLReader() {}

void XFeedXMLReader::readFeed(QIODevice* feed_data)
{
    xml_.setDevice(feed_data);

}
