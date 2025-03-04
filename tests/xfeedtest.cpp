#include "xfeedtest.h"
#include <QTest>
XFeedTest::XFeedTest(QObject *parent)
    : QObject{parent}
{}

void XFeedTest::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");

}

void XFeedTest::test_channelInfo()
{
    QVERIFY_THROWS_NO_EXCEPTION(chInfo.setChAddr("www.testAddr.com"));
    QVERIFY(chInfo.getChAddr() == "www.testAddr.com");

    QVERIFY_THROWS_NO_EXCEPTION(chInfo.setChName("BBCPersian"));
    QVERIFY(chInfo.getChName() == "BBCPersian");

    chInfo.setChComment("Dummy comment");
    QVERIFY(chInfo.getChComment() == "Dummy comment");

    QVERIFY_THROWS_NO_EXCEPTION(ChannelInfo chInfo2);

}

void XFeedTest::test_xfeedCh()
{
    ChannelInfo chInfo;
    chInfo.setChAddr("www.bbc.com");
    chInfo.setChName("BBC Feed");
    chInfo.setChComment("N/A");

    XFeedCh ch(chInfo);

    /* Test 1: Validate Info */
    QVERIFY(ch.chInfo().getChName() == "BBC Feed");
    QVERIFY(ch.chInfo().getChAddr() == "www.bbc.com");
    QVERIFY(ch.chInfo().getChComment() == "N/A");

    /* Test2: Assing by setter */
    ;


}

void XFeedTest::test_xfolder()
{
    XFeedFolder xfolder_("News");

    ChannelInfo chInfo;
    chInfo.setChAddr("www.bbc.com");
    chInfo.setChName("BBC Feed");
    chInfo.setChComment("N/A");

    xfolder_.addChannel(chInfo);



}

void XFeedTest::test_xfolder_folder_name()
{

    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, XFeedFolder xfoler("") );
    QVERIFY_THROWS_NO_EXCEPTION(XFeedFolder xfoler("Test"));

}

void XFeedTest::test_xreader_addFolder()
{

    QVERIFY_THROWS_EXCEPTION(std::invalid_argument, xfeedReader_.addFolder(nullptr));

    auto news_folder = std::make_shared<XFeedFolder>("News");
    ChannelInfo chBBc;
    chBBc.setChAddr("www.bbc.com");
    chBBc.setChName("BBC Feed");
    chBBc.setChComment("N/A");

    news_folder->addChannel(chBBc);

    QVERIFY_THROWS_NO_EXCEPTION(xfeedReader_.addFolder(news_folder));
}

void XFeedTest::test_xreader_folderList()
{
    auto folder_list = xfeedReader_.folderList();
    for(auto& folder: folder_list)
    {
        QVERIFY(folder !=nullptr);
        qDebug()<<folder->folderName();
    }
}

QTEST_MAIN(XFeedTest)
#include "xfeedtest.moc"
