#include "xfeed.h"
#include "./ui_xfeed.h"
#include "addchannel.h"
#include "addfolder.h"
#include "channelbehaviorregistry.h"

#include <QMessageBox>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>

XFeed::XFeed(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::XFeed)
{
    ui->setupUi(this);
    try {

        // XJSonAdapter jsonAdapter_();
        refreshModel();
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
    play_btn->setCheckable(true);
    play_btn->setIcon(QIcon(":/toolbar/icons/media/icons/play-button.png"));
    play_btn->setEnabled(false);
    ui->toolBar->addAction(refresh_act);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(play_btn);

    connectMediaControls(ui->toolBar);


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
        if(index.parent().isValid() && (index.parent().data(Qt::UserRole + 1).toString() != "Type"))
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

    chInfo = model_->itemFromIndex(idx)->data(Qt::UserRole+5).value<ChannelInfo>();
    AddChannel edit_channel(model_.get(), chInfo, this);
    edit_channel.setWindowTitle("Edit Channel");
    if(edit_channel.exec())
    {
        xmodel_.editChannel(idx, chInfo);
        refreshModel();
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
    else
    {
        // play the station
        QAction* playAction = findActionByName(ui->toolBar, "Play");
        if(playAction)
        {
            displayChannel(channelInfo);
            if(strategy && strategy->isMediaPlaySupported())
            {
                strategy->play();
            }
            playAction->setEnabled(true);
            playAction->setChecked(true);
        }
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
    strategy = ChannelBehaviorRegistry::instance()->getDisplayStrategy(channel.chType());
    if (strategy) {
        strategy->display(channel, container);

    } else {
        // Fallback display method
    }
}
// Then find them by iterating through actions:
QAction* XFeed::findActionByName(QToolBar* toolbar, const QString& name) {
    for (QAction* action : toolbar->actions()) {
        if (action->text() == name) {
            return action;
        }
    }
    return nullptr;
}

void XFeed::refreshModel()
{
    model_ = xmodel_.getModelFromData(getDatabasePath());

    ui->xtree->setModel(model_.get());
    ui->xtree->expandAll();
}
void XFeed::connectMediaControls(QToolBar *toolbar)
{
    QAction* play_btn = findActionByName(ui->toolBar,"Play");
    if(play_btn)
    {
        connect(play_btn, &QAction::triggered, this, [this](bool toggeled){

            if(strategy && strategy->isMediaPlaySupported())
            {
                if(toggeled)
                    strategy->play();
                else
                    strategy->stop();
            }
        });
    }
}


void XFeed::on_xtree_clicked(const QModelIndex &index)
{
    // if(index.parent().isValid())
    // {
    //     return;
    // }

    /* First check the type of the channel */
    QVariant vData = model_->data(index, Qt::UserRole + 5);
    ChannelInfo channelInfo = vData.value<ChannelInfo>();

    QAction* play_action = findActionByName(ui->toolBar, "Play");
    if (play_action)
    {
        displayChannel(channelInfo, nullptr);

        if(play_action->isChecked())
            play_action->setChecked(false);
        else
        {

            if(channelInfo.chType()=="Radio")
            {
                play_action->setEnabled(true);
            }
            else
                play_action->setEnabled(false);
        }
    }

}

