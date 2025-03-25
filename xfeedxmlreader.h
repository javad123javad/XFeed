#ifndef XFEEDXMLREADER_H
#define XFEEDXMLREADER_H

#include "xfeedreader.h"
#include <memory.h>
#include <QXmlStreamReader>
#include <QIODevice>

class XFeedXMLReader : public XFeedReader
{
public:
    XFeedXMLReader();
    virtual ~XFeedXMLReader() = default;

    void readFeed(QIODevice* feed_data)  override;


private:
    QXmlStreamReader xml_;
};

#endif // XFEEDXMLREADER_H
