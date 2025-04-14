#include "channelmodelbuilder.h"
#include <QJsonObject>
#include "channelfactory.h"
#include <QList>
ChannelModelBuilder::ChannelModelBuilder() {
    model_ = std::make_shared<QStandardItemModel>();
}

void ChannelModelBuilder::addFolders(const QJsonArray &folderArray)
{

    // for(const QJsonValue& folderValue:folderArray)
    // {
    //     auto folderName = folderValue.toString();
    //     qDebug()<<"Adding Folder Item: "<<folderName;

    //     if (!folderMap_.contains(folderName)) {
    //         auto folderItem = std::make_unique<QStandardItem>(folderName);
    //         folderItem->setData(folderName);
    //         folderMap_[folderName] = folderItem.get(); // Store raw pointer

    //         // model_->item(0, 0)->appendRow(folderItem.release()); // Transfer ownership to the model
    //     }
    // }


}

void ChannelModelBuilder::addTypes(const QJsonArray &chTypeArray)
{
    for(const QJsonValue& type:chTypeArray)
    {
        QString stype = type["type"].toString();
        qDebug()<<"Adding Type Item: "<<stype;

        if(!folders.contains(stype))
        {
            folders[stype] = QMap<QString, QList<QStandardItem*>>();
            model_->appendRow(folders[stype][""]);

        }

    }
    qDebug()<<"Opps!"<<model_->rowCount()<<":c:"<<model_->columnCount();

    for(auto& item: folders)
    {
        qDebug()<<"Adding Head Item";
    }
}

void ChannelModelBuilder::addChannels(const QJsonArray &channelsArray)
{

    // Step 1: Build type -> folder -> channels hierarchy
    QMap<QString, QMap<QString, QList<ChannelInfo*>>> typeFolderMap;
    for ( auto& channelValue : channelsArray) {
        ChannelInfo *channel = new ChannelInfo(channelValue.toObject());
        typeFolderMap[channel->chType()][channel->chFolder()].append((channel));
    }

    // Step 2: Construct the model
    for (const QString& type : typeFolderMap.keys()) {
        auto typeItem = new QStandardItem(type);
        const auto& folderMap = typeFolderMap[type];
        for (const QString& folderName : folderMap.keys()) {
            auto folderItem = new QStandardItem(folderName);
            folderItem->setData(folderName);
            const auto& channels = folderMap[folderName];
            for (const ChannelInfo* channel : channels) {
                auto channelItem = ChannelFactory::createChannelItem(*channel);
                folderItem->appendRow(channelItem.release());
            }
            typeItem->appendRow(folderItem);
        }
        model_->appendRow(typeItem);
    }

}

void ChannelModelBuilder::fixChannels(QJsonObject &jRoot)
{
    QJsonArray channelsArray = jRoot["Channels"].toArray();

    for(auto it = channelsArray.begin(); it != channelsArray.end(); ++it)
    {
        QJsonObject ch = it->toObject();
        QUuid chUUID(ch["uuid"].toString());
        if(chUUID.isNull())
        {

            ch["uuid"] = QUuid::createUuid().toString();
            qDebug()<<"Fix illegal UUID:"<<ch["uuid"].toString();
            channelsArray.erase(it);
            channelsArray.append(ch);
        }
    }
    jRoot["Channels"] = channelsArray;

}
