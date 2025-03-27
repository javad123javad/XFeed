#ifndef XFEEDXMLREADER_H
#define XFEEDXMLREADER_H

#include "xfeedreader.h"
#include <memory.h>
#include <QXmlStreamReader>
#include <QIODevice>
#include <QStandardItemModel>
class XFeedXMLReader : public XFeedReader
{
public:
    XFeedXMLReader();
    virtual ~XFeedXMLReader() = default;

    void parseFeed(QIODevice* feed_data, QStandardItemModel& feedDataModel)  override;


private:
    QXmlStreamReader xml_;
};

#endif // XFEEDXMLREADER_H
