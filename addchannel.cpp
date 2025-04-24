#include "addchannel.h"
#include "ui_addchannel.h"
#include <QMessageBox>
#include <QStandardItem>

AddChannel::AddChannel(QStandardItemModel *model, ChannelInfo& chInfo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddChannel)
    , chInfo_(chInfo)
{
    ui->setupUi(this);

    ui->lin_ch_url->setText(chInfo.getChAddr());
    ui->lin_ch_name->setText(chInfo.getChName());

    ui->cmb_ch_type->setModel(model);
    qDebug()<<"Folder to search:"<<chInfo.chFolder();
    auto items = model->findItems(chInfo.chFolder(), Qt::MatchRecursive);
    QModelIndex idx;
    for(auto & item : items)
    {
        if(item->parent()->text()== chInfo.chType())
        {

            idx = item->parent()->index();
            qDebug()<<"Current Type:"<<chInfo.chType();
            ui->cmb_ch_type->setEditText(chInfo.chType());

            break;
        }
    }
    if(idx.isValid())
    {
        for(int i =0 ; i < model->item(idx.row(),0)->rowCount(); i++)
        {
            auto itemData = model->item(idx.row(),0)->child(i,0)->data();
            ui->cmb_folder->addItem(itemData.toString(),itemData);
        }
    }
    setCurrentFolder(chInfo.chFolder());
    auto indx = ui->cmb_ch_type->findText(chInfo.chType());
    ui->cmb_ch_type->setCurrentIndex(indx);
}

AddChannel::~AddChannel()
{
    delete ui;
}


bool AddChannel::validateChannelInfo()
{
    QString urlText = ui->lin_ch_url->text().trimmed();
    QUrl url(urlText, QUrl::StrictMode); // Enforce strict parsing

    if (ui->lin_ch_name->text().isEmpty()
        || !url.isValid()
        || url.scheme().isEmpty() // Ensure the scheme is present
        || (url.scheme() != "http" && url.scheme() != "https") // Restrict to web URLs
        || url.host().isEmpty() // Ensure a valid host is present
        || ui->cmb_folder->currentText().isEmpty()
        || ui->cmb_ch_type->currentText().isEmpty()
        )
    {
        return false;
    }

    return true;
}

void AddChannel::setCurrentFolder(const QString &folderName)
{
    auto model = ui->cmb_folder->model();
    if (!model || folderName.isEmpty()) {
        qWarning() << "QComboBox model is null! or no folder name specifyed.";
        return;
    }

    // Loop through the model to find the matching folder name
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i, 0); // Assuming folder names are in column 0
        QString itemText = model->data(index, Qt::DisplayRole).toString().trimmed();


        if (itemText.compare(folderName.trimmed(), Qt::CaseInsensitive) == 0) {
            ui->cmb_folder->setCurrentIndex(i);  // Set the matching index
            return;
        }
    }

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
        chInfo_.setChType(ui->cmb_ch_type->currentText());
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
