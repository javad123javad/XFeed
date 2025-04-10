#ifndef ADDFOLDER_H
#define ADDFOLDER_H

#include <QDialog>

#include <QStandardItemModel>
#include <memory.h>

namespace Ui {
class AddFolder;
}

class AddFolder : public QDialog
{
    Q_OBJECT

public:
    explicit AddFolder(std::shared_ptr<QStandardItemModel> model,QWidget *parent = nullptr);
    ~AddFolder();

    QString folderName() const;

private:
    Ui::AddFolder *ui;
    void check_new_folder();
    QString folderName_;
    std::shared_ptr<QStandardItemModel> model_;
protected:
    void accept() override; // Override the accept() method
};

#endif // ADDFOLDER_H
