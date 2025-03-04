#ifndef XFEED_H
#define XFEED_H

#include <QMainWindow>
#include <memory>
#include "xfeedxmlreader.h"
#include "xfeedfolder.h"

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
    folder_list_t feedFolders_;

private slots:
    void on_actionAdd_Channel_triggered();
    void on_actionAdd_Folder_triggered();
};
#endif // XFEED_H
