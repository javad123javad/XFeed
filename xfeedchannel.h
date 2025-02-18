#ifndef XFEEDCHANNEL_H
#define XFEEDCHANNEL_H

#include <QObject>
class XFeedChannel : public QObject
{
    Q_OBJECT
public:
    explicit XFeedChannel(QObject *parent = nullptr);
    ~XFeedChannel() = default;

signals:
};

#endif // XFEEDCHANNEL_H
