#include "xfeedfolder.h"
#include <QDebug>
#include <stdexcept>

XFeedFolder::XFeedFolder(const QString folder_name, QObject *parent)
    : QObject{parent}, uuid_(calc_uuid())
{
    setFolderName(folder_name);
    qDebug()<<uuid_;
}

void XFeedFolder::addChannel(ChannelInfo &chInfo)
{
    channelList_.push_back(std::make_shared<XFeedCh>(chInfo));
}

QUuid XFeedFolder::calc_uuid()
{
    return QUuid::createUuid();
}

QVector<std::shared_ptr<XFeedCh> > XFeedFolder::channelList() const
{
    return channelList_;
}

QString XFeedFolder::folderName() const
{
    return folderName_;
}

void XFeedFolder::setFolderName(const QString &newFolderName)
{
    if(newFolderName.isEmpty())
    {
        qDebug()<<"Empty f name???";
        throw std::invalid_argument("Folder name cannot be empty");
    }
    folderName_ = newFolderName;
}

QUuid XFeedFolder::uuid() const
{
    return uuid_;
}

