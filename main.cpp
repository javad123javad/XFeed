#include "xfeed.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<ChannelInfo>("ChannelInfo");

    QApplication a(argc, argv);
    XFeed w;
    w.show();
    return a.exec();
}
