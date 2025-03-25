#ifndef XFEEDREADER_H
#define XFEEDREADER_H

#include <QObject>
#include <QIODevice>
class XFeedReader: public QObject
{
    Q_OBJECT
public:
    XFeedReader() = default;
    virtual ~XFeedReader() = default;

    virtual void readFeed(QIODevice* feed_data) = 0;

};

#endif // XFEEDREADER_H
