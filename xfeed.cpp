#include "xfeed.h"
#include "./ui_xfeed.h"
#include "addchannel.h"
#include "addfolder.h"
#include "xjsonadapter.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include "channelbehaviorregistry.h"
XFeed::XFeed(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XFeed)
{
    ui->setupUi(this);
    try {

        // XJSonAdapter jsonAdapter_();
        model_ = xmodel_.getModelFromData(getDatabasePath());

        ui->xtree->setModel(model_.get());
        ui->xtree->expandAll();
        connect(&xmodel_, &XFeedModel::feed_data_ready, this, &XFeed::on_feed_data_ready);
        // Setup toolbar
        fill_tool_bar();
    } catch (std::logic_error e) {
        QMessageBox::critical(this,"Database error",e.what());
        qDebug()<<e.what();
    }
}

XFeed::~XFeed()
{
    delete ui;
}

void XFeed::fill_tool_bar()
{
    QAction *refresh_act = new QAction("Refresh Channel", this);
    refresh_act->setIcon(QIcon(":/toolbar/icons/media/icons/refresh.png"));
    connect(refresh_act, &QAction::triggered, this, [this](){
        ui->textBrowser->clear();
        xmodel_.fetchChannel(ui->xtree->currentIndex());
    });
    /* Media player buttons */
    QAction *play_btn = new QAction("Play", this);
    play_btn->setIcon(QIcon(":/toolbar/icons/media/icons/play-button.png"));


    ui->toolBar->addAction(refresh_act);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(play_btn);
}



void XFeed::on_actionAdd_Channel_triggered()
{
    ChannelInfo chInfo;

    AddChannel addCh(model_.get(), chInfo, this);

    if(addCh.exec())
    {
        xmodel_.addChannel(chInfo);
    }

}


void XFeed::on_actionAdd_Folder_triggered()
{
    // Get data from view
    AddFolder addFolderDlg_(model_, this);
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
            connect(act_edit, &QAction::triggered, this, [this, index]() { onEditChannel(index); });
            treeMenu.addAction(act_edit);
            /** Delete Channel **/

            QAction *act_del = new QAction("Delete Channel", this);
            connect(act_del, &QAction::triggered, this, [this, index](){onDeleteChannel(index);});
            treeMenu.addAction(act_del);
        }
        else
        {
            QAction *act_add_ch = new QAction("Add Channel",this);
            connect(act_add_ch, &QAction::triggered, this, &XFeed::on_actionAdd_Channel_triggered);
            treeMenu.addAction(act_add_ch);
            QAction *act_del = new QAction("Delete Folder", this);
            connect(act_del, &QAction::triggered, this, [this, &index](){onDeleteFolder(index);});
            treeMenu.addAction(act_del);
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
    chInfo.setChType(model_->itemFromIndex(idx)->data(Qt::UserRole + 4).toString());
    AddChannel edit_channel(model_.get(), chInfo, this);
    edit_channel.setWindowTitle("Edit Channel");
    if(edit_channel.exec())
    {

        xmodel_.editChannel(idx, chInfo);
    }
}

void XFeed::onDeleteChannel(QModelIndex indx)
{
    xmodel_.deleteChannel(indx);
}

void XFeed::onDeleteFolder(QModelIndex& indx)
{
    xmodel_.deleteFolder(indx);
    ui->xtree->update();
}


void XFeed::on_xtree_doubleClicked(const QModelIndex &index)
{
    /* First check the type of the channel */
    QVariant vData = model_->data(index, Qt::UserRole + 5);
    ChannelInfo channelInfo = vData.value<ChannelInfo>();

    if(channelInfo.chType() == "RSS")
    {
        ui->textBrowser->clear();
        xmodel_.fetchChannel(index);
    }
    else if (channelInfo.chType() == "Radio")
    {
        // play the station
        displayChannel(channelInfo);
    }
}

void XFeed::on_feed_data_ready(QStandardItemModel &data)
{

    ui->tableView->setModel(&data);
    ui->tableView->resizeColumnsToContents();

}


void XFeed::on_tableView_clicked(const QModelIndex &index)
{

    if (!index.isValid()) {
        ui->textBrowser->clear();
        return;
    }

    // Retrieve the description stored in Qt::UserRole from the Title column (column 0)
    QString descriptionHtml = index.sibling(index.row(), 0).data(Qt::UserRole).toString();
    if (descriptionHtml.isEmpty()) {
        ui->textBrowser->setText("No content available for this item.");
        return;
    }

    // Replace <img> tags with their src URL or alt text
    QRegularExpression imgRegex("<img\\s+[^>]*src=\"([^\"]+)\"[^>]*alt=\"([^\"]*)\"[^>]*>",
                                QRegularExpression::CaseInsensitiveOption);
    QString modifiedHtml = descriptionHtml;

    // Replace with clickable link (src URL)
    modifiedHtml.replace(imgRegex, "<a href=\"\\1\">Image Link: \\1</a>");

    // Configure QTextBrowser
    ui->textBrowser->setOpenExternalLinks(true);  // Enable clickable links
    ui->textBrowser->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
    ui->textBrowser->setSearchPaths({});          // Clear any local search paths
    ui->textBrowser->setHtml(modifiedHtml);       // Set the modified HTML
}


void XFeed::on_tableView_doubleClicked(const QModelIndex &index)
{
    QUrl descriptionHtml = index.sibling(index.row(), 1).data(Qt::UserRole).toUrl();

    QDesktopServices::openUrl(descriptionHtml);
}


QString XFeed::getDatabasePath()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataPath); // Ensure the directory exists

    return dataPath + "/db.json"; // Full path to db.json
}


void XFeed::on_action_Open_Feed_Database_triggered()
{
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open XFeed Database"), dataPath, tr("JSon Files (*.json)"));

    try {

        model_->clear();
        model_ = xmodel_.getModelFromData(fileName);

        ui->xtree->setModel(model_.get());
        ui->xtree->expandAll();
        connect(&xmodel_, &XFeedModel::feed_data_ready, this, &XFeed::on_feed_data_ready);
    } catch (std::logic_error e) {
        qDebug()<<e.what();
    }
}


void XFeed::on_action_Exit_triggered()
{
    QApplication::quit();

}

void XFeed::displayChannel(const ChannelInfo &channel, QWidget *container)
{
    auto strategy = ChannelBehaviorRegistry::instance()->getDisplayStrategy(channel.chType());
    if (strategy) {
        strategy->display(channel, container);
    } else {
        // Fallback display method
    }
}

