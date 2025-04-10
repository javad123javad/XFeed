#include "addfolder.h"
#include "ui_addfolder.h"
#include <QMessageBox>

AddFolder::AddFolder(std::shared_ptr<QStandardItemModel> model, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFolder),
    model_(model)
{
    ui->setupUi(this);

    // Misusing QQomboBox in displaying only root items ;)
    ui->cmb_ch_type->setModel(model.get());
}

AddFolder::~AddFolder()
{
    delete ui;
}

void AddFolder::check_new_folder()
{
    if(ui->edt_folder_name->text().isEmpty()
        || ui->cmb_ch_type->currentIndex() < 0)
    {
        throw std::invalid_argument("Invalid forlder name/type.");
    }
}

QString AddFolder::folderName() const
{
    return folderName_;
}

void AddFolder::accept()
{
    try{
        check_new_folder();
        folderName_ = ui->edt_folder_name->text();
        QDialog::accept();
    }
    catch(std::invalid_argument e)
    {

        QMessageBox::critical(this, "Invalid Folder name", e.what());
    }
}
