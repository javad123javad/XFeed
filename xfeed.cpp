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
        model_ = xmodel_.getModelFromData("/home/javad/workspace/QtWorkspace/XFeed/db.json");

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
    qDebug()<<"New Channel UUID:"<<chInfo.chUUID().toString();
    AddChannel addCh(model_.get(), chInfo, this);

    if(addCh.exec())
    {
        xmodel_.addChannel(chInfo);
    }

}


void XFeed::on_actionAdd_Folder_triggered()
{
    // Get data from view
    AddFolder addFolderDlg_(feedFolders_, this);
    if(addFolderDlg_.exec())
    {
        xmodel_.addFolder(addFolderDlg_.folderName());

    }
}


void XFeed::on_xtree_customContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->xtree->indexAt(pos);
    if(index.isValid())
    {
        if(index.parent().isValid())
        {
            QAction* act_edit = new QAction("Edit Channel", this);
            connect(act_edit, &QAction::triggered, ui->xtree, [this, index]() { onEditChannel(index); });
            treeMenu.addAction(act_edit);
        }

        treeMenu.exec(ui->xtree->viewport()->mapToGlobal(pos));
        treeMenu.clear();
    }
}

void XFeed::onEditChannel(QModelIndex idx)
{
    ChannelInfo chInfo;
    qDebug()<<(model_->itemFromIndex(idx)->data().toString());
    chInfo.setChName(model_->itemFromIndex(idx)->data(Qt::UserRole).toString());
    chInfo.setChAddr(model_->itemFromIndex(idx)->data(Qt::UserRole + 1).toString());
    chInfo.setChUUID(QUuid::fromString(model_->itemFromIndex(idx)->data(Qt::UserRole + 2).toString()));
    chInfo.setChFolder(model_->itemFromIndex(idx)->data(Qt::UserRole + 3).toString());

    AddChannel edit_channel(model_.get(), chInfo, this);
    edit_channel.setWindowTitle("Edit Channel");
    if(edit_channel.exec())
    {

        xmodel_.editChannel(idx, chInfo);
    }
}

