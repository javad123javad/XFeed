#include "xfeed.h"
#include "./ui_xfeed.h"
#include "addchannel.h"
#include "addfolder.h"
#include "xjsonadapter.h"
XFeed::XFeed(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XFeed)
{
    ui->setupUi(this);
    try {

        // XJSonAdapter jsonAdapter_();
        model_ = xmodel_.getModelFromData("/home/javad/workspace/qt_workspace/XMLFeedReader/db.json");

        ui->xtree->setModel(model_.get());
    } catch (std::logic_error e) {
        qDebug()<<e.what();
    }
}

XFeed::~XFeed()
{
    delete ui;
}



void XFeed::on_actionAdd_Channel_triggered()
{

    AddChannel addCh(feedFolders_, this);
    addCh.setFolderList(feedFolders_);
    addCh.setChannelModel(model_.get());

    if(addCh.exec())
    {
        ChannelInfo chInfo = addCh.getChInfo();
        xmodel_.addChannel(chInfo);
        xreader.addChannel(chInfo);
    }

}


void XFeed::on_actionAdd_Folder_triggered()
{
    // Get data from view
    AddFolder addFolderDlg_(feedFolders_, this);
    if(addFolderDlg_.exec())
    {
        feedFolders_.push_back(std::make_shared<XFeedFolder>(addFolderDlg_.folderName()));
    }
    // Send data to the Model for further processing
    //XFeedModel->addFolder(FolderName);
    xmodel_.addFolder(addFolderDlg_.folderName());

}


void XFeed::on_xtree_clicked(const QModelIndex &index)
{
    auto item = model_->itemFromIndex(index);

    qDebug()<<item->data(Qt::UserRole).toString();
}

