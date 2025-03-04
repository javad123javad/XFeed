#ifndef ADDFOLDER_H
#define ADDFOLDER_H

#include <QDialog>

#include "xfeedfolder.h"

namespace Ui {
class AddFolder;
}

class AddFolder : public QDialog
{
    Q_OBJECT

public:
    explicit AddFolder(folder_list_t& folder_list, QWidget *parent = nullptr);
    ~AddFolder();

    QString folderName() const;

private:
    Ui::AddFolder *ui;
    void check_new_folder();
    folder_list_t& folderList_;
    QString folderName_;
protected:
    void accept() override; // Override the accept() method
};

#endif // ADDFOLDER_H
