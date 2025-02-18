#ifndef XFEED_H
#define XFEED_H

#include <QMainWindow>

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
};
#endif // XFEED_H
