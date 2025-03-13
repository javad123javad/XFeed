#include "addchannel.h"
#include "ui_addchannel.h"
#include <QMessageBox>
#include <QStandardItem>

AddChannel::AddChannel(folder_list_t & folder_list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddChannel)
    , folderList_(folder_list)
{
    ui->setupUi(this);
    for(auto& folder: folderList_)
    {
        ui->cmb_folder->addItem(folder->folderName(), QVariant::fromValue(folder));
    }

}

AddChannel::~AddChannel()
{
    delete ui;
}


void AddChannel::setFolderList(const folder_list_t &newFolderList)
{
    folderList_ = newFolderList;
}

void AddChannel::setChannelModel(QAbstractItemModel *model)
{
    ui->cmb_folder->setModel(model);
}

 ChannelInfo AddChannel::getChInfo()
{
    return chInfo_;
}

bool AddChannel::validateChannelInfo()
{
    bool is_valid = true;

    if(ui->lin_ch_name->text().isEmpty()
        || ui->lin_ch_url->text().isEmpty()
        || ui->cmb_folder->currentText().isEmpty())
    {
        is_valid = false;
    }

    return is_valid;

}

void AddChannel::accept()
{

    if(!validateChannelInfo())
    {
        QMessageBox::critical(this, "Invalid Entry", "Invalid entry. Please check values");
    }
    else
    {
        chName = ui->lin_ch_name->text();
        chInfo_.setChName(ui->lin_ch_name->text());
        chInfo_.setChAddr(ui->lin_ch_url->text());
        chInfo_.setChComment(ui->txt_ch_comment->toPlainText());
        chInfo_.setChFolder(ui->cmb_folder->currentText());
        QDialog::accept();
    }
}

QString AddChannel::getChName() const
{
    return chName;
}


void AddChannel::on_cmb_folder_activated(int index)
{


}
