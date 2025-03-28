#include "addchannel.h"
#include "ui_addchannel.h"
#include <QMessageBox>
#include <QStandardItem>

AddChannel::AddChannel(QAbstractItemModel *model, ChannelInfo& chInfo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddChannel)
    , chInfo_(chInfo)
{
    ui->setupUi(this);

    ui->lin_ch_url->setText(chInfo.getChAddr());
    ui->lin_ch_name->setText(chInfo.getChName());
    ui->cmb_folder->setModel(model);
    qDebug()<<"Folder Name:"<<chInfo.chFolder();
    setCurrentFolder(chInfo.chFolder());
}

AddChannel::~AddChannel()
{
    delete ui;
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

void AddChannel::setCurrentFolder(const QString &folderName)
{
    auto model = ui->cmb_folder->model();
    if (!model || folderName.isEmpty()) {
        qWarning() << "QComboBox model is null! or no folder name specifyed.";
        return;
    }

    qDebug() << "Searching for folder:" << folderName;

    // Loop through the model to find the matching folder name
    for (int i = 0; i < model->rowCount(); ++i) {
        QModelIndex index = model->index(i, 0); // Assuming folder names are in column 0
        QString itemText = model->data(index, Qt::DisplayRole).toString().trimmed();

        qDebug() << "Checking item [" << i << "]:" << itemText;

        if (itemText.compare(folderName.trimmed(), Qt::CaseInsensitive) == 0) {
            ui->cmb_folder->setCurrentIndex(i);  // Set the matching index
            qDebug() << "Set current folder to:" << folderName << "at index:" << i;
            return;
        }
    }

    qDebug() << "Oops, no such folder name found in model!";
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
