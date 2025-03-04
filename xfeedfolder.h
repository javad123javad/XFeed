#ifndef XFEEDFOLDER_H
#define XFEEDFOLDER_H

#include <QObject>
#include <QUuid>

#include "xfeedch.h"

class XFeedFolder : public QObject
{
    Q_OBJECT
public:
    explicit XFeedFolder(const QString folder_name, QObject *parent = nullptr);
    XFeedFolder(XFeedFolder &folder) {
        this->setFolderName(folder.folderName_);
        this->uuid_ = folder.uuid_;
    }
    void addChannel(ChannelInfo& chInfo);
    QVector<std::shared_ptr<XFeedCh> > channelList() const;

    QString folderName() const;
    void setFolderName(const QString &newFolderName);

    QUuid uuid() const;

private:
    QString folderName_;
    QUuid uuid_;

private:
    QUuid calc_uuid();
    QVector<std::shared_ptr<XFeedCh>> channelList_;


signals:
};
using folder_list_t = std::vector<std::shared_ptr<XFeedFolder>>;

#endif // XFEEDFOLDER_H
