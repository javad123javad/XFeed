#ifndef XFEED_H
#define XFEED_H

#include <QMainWindow>
#include <memory>
#include "xfeedxmlreader.h"
#include "xfeedfolder.h"
#include "xfeedmodel.h"
#include <QStandardItemModel>
#include <QMenu>
#include <QTextBrowser>
#include <QMediaPlayer>
#include <QLabel>
#include "displaystrategy.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class XFeed;
}
QT_END_NAMESPACE


class XFeed : public QMainWindow
{
    Q_OBJECT

public:
    XFeed(QWidget *parent = nullptr);
    ~XFeed();

private:
    Ui::XFeed *ui;
    XFeedXMLReader xreader;
    std::shared_ptr<QStandardItemModel> model_;
    XFeedModel xmodel_;
    QMenu treeMenu;
    void fetchAndSetImage(QString imageUrl, QTextBrowser* textBrowser);
    void fill_tool_bar();
    QString getDatabasePath();
    void displayChannel(const ChannelInfo& channel, QWidget* container = nullptr);
    void connectMediaControls(QToolBar* toolbar);
    DisplayStrategy* strategy;
    QAction* findActionByName(QToolBar* toolbar, const QString& name);
    QLabel * lbl_radio;
    void refreshModel();
    QString polishHtmlData(QString& htmlIn);

private slots:
    void on_actionAdd_Channel_triggered();
    void on_actionAdd_Folder_triggered();
    void on_xtree_customContextMenuRequested(const QPoint &pos);
    void onEditChannel(QModelIndex);
    void onDeleteChannel(QModelIndex);
    void onDeleteFolder(QModelIndex&indx);

    void on_xtree_doubleClicked(const QModelIndex &index);
    void on_feed_data_ready(QStandardItemModel& data);
    void on_tableView_clicked(const QModelIndex &index);
    void on_tableView_doubleClicked(const QModelIndex &index);
    void on_action_Open_Feed_Database_triggered();
    void on_action_Exit_triggered();
    void on_xtree_clicked(const QModelIndex &index);
};
#endif // XFEED_H
