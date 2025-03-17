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
        // treeMenu.addMenu("Delete Channel");
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
    ChannelInfo chInfo;
    AddChannel addCh(feedFolders_, chInfo, this);
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
    qDebug()<<item->data(Qt::UserRole+1).toString();

}


void XFeed::on_xtree_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->xtree->indexAt(pos);
    if(index.isValid())
    {
        if(index.parent().isValid())
        {
            QAction* act_edit = new QAction("Edit Channel", this);
            connect(act_edit, &QAction::triggered, ui->xtree, [this, index]() { onAddChannel(index); });
            treeMenu.addAction(act_edit);
        }
        else
        {

            qDebug()<<"Folder item";
        }
        treeMenu.exec(ui->xtree->viewport()->mapToGlobal(pos));
        treeMenu.clear();
    }
}

void XFeed::onAddChannel(QModelIndex idx)
{
    ChannelInfo chInfo;
    qDebug()<<(model_->itemFromIndex(idx)->data().toString());
    chInfo.setChName(model_->itemFromIndex(idx)->data(Qt::UserRole).toString());
    chInfo.setChAddr(model_->itemFromIndex(idx)->data(Qt::UserRole + 1).toString());

    AddChannel edit_channel(feedFolders_, chInfo, this);
    edit_channel.setWindowTitle("Edit Channel");
    edit_channel.setChannelModel(model_.get());

    if(edit_channel.exec())
    {

        xmodel_.editChannel(idx, edit_channel.getChInfo());
    }
}

