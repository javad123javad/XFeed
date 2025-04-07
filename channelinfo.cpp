#include "channelinfo.h"
#include <QJsonObject>
#include <QMetaProperty>

QString ChannelInfo::chFolder() const
{
    return chFolder_;
}

void ChannelInfo::setChFolder(const QString &newChFolder)
{
    chFolder_ = newChFolder;
}
QJsonObject ChannelInfo::toJson() const {
    QJsonObject json;
    const QMetaObject* meta = metaObject();  // Get class metadata

    for (int i = 0; i < meta->propertyCount(); ++i) {
        QMetaProperty prop = meta->property(i);
        json[prop.name()] = property(prop.name()).toString();
        qDebug()<<"tojson:"<<
            prop.name()<<"Value:"<<property(prop.name()).toString();
    }
    return json;
}

ChannelInfo::ChannelInfo(const QJsonObject& obj)
{
    chName_ = obj["name"].toString();
    chAddr_ = obj["url"].toString();
    chUUID_ = QUuid(obj["uuid"].toString());
    chFolder_ =  obj["folderName"].toString();
    chType_ = obj["type"].toString();
}

QUuid ChannelInfo::chUUID() const
{
    return chUUID_;
}

void ChannelInfo::setChUUID(const QUuid &newChUUID)
{
    chUUID_ = newChUUID;
}

