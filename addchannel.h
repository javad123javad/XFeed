#ifndef ADDCHANNEL_H
#define ADDCHANNEL_H

#include <QDialog>
#include <QAbstractItemModel>

#include "channelinfo.h"
#include "xfeedfolder.h"
#include <QStandardItemModel>
namespace Ui {
class AddChannel;
}

class AddChannel : public QDialog
{
    Q_OBJECT

public:
    explicit AddChannel(QAbstractItemModel *model, ChannelInfo& chInfo,QWidget *parent = nullptr);
    ~AddChannel();

    QString getChName() const;

private slots:
    void on_cmb_folder_activated(int index);

private:
    Ui::AddChannel *ui;
    QString chName;
    ChannelInfo& chInfo_;

private: /* methods */
    bool validateChannelInfo();
    void setCurrentFolder(const QString &folderName);
protected:
     void accept() override; // Override the accept() method
};

#endif // ADDCHANNEL_H
