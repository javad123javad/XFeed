#include "xfeedtest_channelinfo.h"
#include <QTest>
XFeedTest_ChannelInfo::XFeedTest_ChannelInfo(QObject *parent)
    : QObject{parent}
{}

void XFeedTest_ChannelInfo::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");

}

QTEST_MAIN(XFeedTest_ChannelInfo)
#include "xfeedtest_channelinfo.moc"
