#include "xfeed.h"
#include "./ui_xfeed.h"

XFeed::XFeed(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XFeed)
{
    ui->setupUi(this);
}

XFeed::~XFeed()
{
    delete ui;
}
