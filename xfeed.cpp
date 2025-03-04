#include "xfeed.h"
#include "./ui_xfeed.h"
#include "addchannel.h"
#include "addfolder.h"

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



void XFeed::on_actionAdd_Channel_triggered()
{

    AddChannel addCh(feedFolders_, this);
    addCh.setFolderList(feedFolders_);
    if(addCh.exec())
    {
        ChannelInfo chInfo = addCh.getChInfo();
        xreader.addChannel(chInfo);
    }

}


void XFeed::on_actionAdd_Folder_triggered()
{
    AddFolder addFolderDlg_(feedFolders_, this);
    if(addFolderDlg_.exec())
    {
        feedFolders_.push_back(std::make_shared<XFeedFolder>(addFolderDlg_.folderName()));
    }

}

