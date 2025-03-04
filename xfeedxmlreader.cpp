#include "xfeedxmlreader.h"

XFeedXMLReader::XFeedXMLReader() {}

void XFeedXMLReader::addChannel(ChannelInfo& chInfo)
{

    channelList_.push_back(std::make_shared<XFeedCh>(chInfo));
}

void XFeedXMLReader::addFolder(std::shared_ptr<XFeedFolder> folder)
{

    if(!folder)
        throw std::invalid_argument("folder parameter can not be empty");

    folderList_.append(folder);
}


void XFeedXMLReader::readFeed()
{

}

const QVector<std::shared_ptr<XFeedFolder> >& XFeedXMLReader::folderList() const
{
    return folderList_;
}
