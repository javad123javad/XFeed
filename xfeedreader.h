#ifndef XFEEDREADER_H
#define XFEEDREADER_H

#include <QObject>
#include <QIODevice>
#include <QStandardItemModel>
class XFeedReader: public QObject
{
    Q_OBJECT
public:
    XFeedReader() = default;
    virtual ~XFeedReader() = default;

    virtual void parseFeed(QIODevice* feed_data, QStandardItemModel& feedDataModel) = 0;

};

#endif // XFEEDREADER_H
