#ifndef ADDCHANNEL_H
#define ADDCHANNEL_H

#include <QDialog>
#include "channelinfo.h"
#include "xfeedfolder.h"
namespace Ui {
class AddChannel;
}

class AddChannel : public QDialog
{
    Q_OBJECT

public:
    explicit AddChannel(folder_list_t & folder_list, QWidget *parent = nullptr);
    ~AddChannel();

    QString getChName() const;

    ChannelInfo getChInfo() ;

    void setFolderList(const folder_list_t &newFolderList);

private slots:
    void on_cmb_folder_activated(int index);

private:
    Ui::AddChannel *ui;
    QString chName;
    ChannelInfo chInfo_;
    folder_list_t folderList_;

private: /* methods */
    bool validateChannelInfo();
protected:
     void accept() override; // Override the accept() method
};

#endif // ADDCHANNEL_H
