#ifndef XFEEDTEST_H
#define XFEEDTEST_H

#include <QObject>
#include "../channelinfo.h"
#include "../xfeedxmlreader.h"
#include "../xjsonadapter.h"
class XFeedTest : public QObject
{
    Q_OBJECT
public:
    explicit XFeedTest(QObject *parent = nullptr);
private slots:
    void toUpper();
    void test_channelInfo();
#if 0

    void test_xfeedCh();

    void test_xfolder();
    void test_xfolder_folder_name();
    void test_xreader_addFolder();
    void test_xreader_folderList();
#endif
    void test_xjsonadapter_readFile();
    void test_xjsonadapter_readJSonDoc();
    void test_xjsonadapter_createModelFromJson();
private:
    ChannelInfo chInfo;
    XFeedXMLReader xfeedReader_;


signals:
};

#endif // XFEEDTEST_H
