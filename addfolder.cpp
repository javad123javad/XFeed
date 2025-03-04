#include "addfolder.h"
#include "ui_addfolder.h"
#include <QMessageBox>

AddFolder::AddFolder(folder_list_t& folder_list, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddFolder)
    , folderList_(folder_list)
{
    ui->setupUi(this);

}

AddFolder::~AddFolder()
{
    delete ui;
}

void AddFolder::check_new_folder()
{
    if(ui->edt_folder_name->text().isEmpty())
    {
        throw std::invalid_argument("Empty folder name is not allowed");
    }

    for(auto& folder: folderList_)
    {
        if(folder->folderName() == ui->edt_folder_name->text())
            throw std::invalid_argument("Folder name already exist");
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
