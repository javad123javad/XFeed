#ifndef XFEEDTEST_CHANNELINFO_H
#define XFEEDTEST_CHANNELINFO_H

#include <QObject>

class XFeedTest_ChannelInfo : public QObject
{
    Q_OBJECT
public:
    explicit XFeedTest_ChannelInfo(QObject *parent = nullptr);
private slots:
    void toUpper();
signals:
};

#endif // XFEEDTEST_CHANNELINFO_H
