#ifndef XFEEDXMLREADER_H
#define XFEEDXMLREADER_H

#include "xfeedreader.h"
#include "xfeedch.h"
#include "xfeedfolder.h"
#include <memory.h>
class XFeedXMLReader : public XFeedReader
{
public:
    XFeedXMLReader();
    virtual ~XFeedXMLReader() = default;
    void addChannel(ChannelInfo& chInfo) override;
    void addFolder(std::shared_ptr<XFeedFolder> folder);

    void readFeed()  override;

    const QVector<std::shared_ptr<XFeedFolder> >& folderList() const;

private:
    QVector<std::shared_ptr<XFeedFolder>> folderList_;
    QVector< std::shared_ptr<XFeedCh>> channelList_;
};

#endif // XFEEDXMLREADER_H
