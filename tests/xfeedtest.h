#ifndef XFEEDTEST_H
#define XFEEDTEST_H

#include <QObject>
#include "../channelinfo.h"
#include "../xfeedxmlreader.h"
class XFeedTest : public QObject
{
    Q_OBJECT
public:
    explicit XFeedTest(QObject *parent = nullptr);
private slots:
    void toUpper();
    void test_channelInfo();
    void test_xfeedCh();
    void test_xfolder();
    void test_xfolder_folder_name();
    void test_xreader_addFolder();
    void test_xreader_folderList();
private:
    ChannelInfo chInfo;
    XFeedXMLReader xfeedReader_;

signals:
};

#endif // XFEEDTEST_H
