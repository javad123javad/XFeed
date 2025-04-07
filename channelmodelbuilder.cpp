#include "channelmodelbuilder.h"
#include <QJsonObject>
#include "channelfactory.h"
ChannelModelBuilder::ChannelModelBuilder() {
    model_ = std::make_shared<QStandardItemModel>();
}

void ChannelModelBuilder::addFolders(const QJsonArray &folderArray)
{

    for(const QJsonValue& folderValue:folderArray)
    {
        auto folderName = folderValue.toString();
        if (!folderMap_.contains(folderName)) {
            auto folderItem = std::make_unique<QStandardItem>(folderName);
            folderItem->setData(folderName);
            folderMap_[folderName] = folderItem.get(); // Store raw pointer
            model_->item(0, 0)->appendRow(folderItem.release()); // Transfer ownership to the model
        }
    }

}

void ChannelModelBuilder::addTypes(const QJsonArray &chTypeArray)
{
    for(const QJsonValue& type:chTypeArray)
    {
        model_->appendRow(new QStandardItem(type.toString()));
    }
}

void ChannelModelBuilder::addChannels(const QJsonArray &channelsArray)
{

    for (const QJsonValue& channelValue : channelsArray) {
        const QJsonObject channelObj = channelValue.toObject();
        const QString folderName = channelObj["folderName"].toString();

        // Add the channel to the folder
        auto channelItem = ChannelFactory::createChannelItem(ChannelInfo(channelObj));


        folderMap_[folderName]->appendRow(channelItem.release()); // Transfer ownership to the model


    }
}
