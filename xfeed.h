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
};
#endif // XFEED_H
