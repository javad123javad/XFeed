#include "xfeedmodel.h"
#include <QJsonObject>
#include <QJsonArray>
XFeedModel::XFeedModel(QObject *parent)
    : QObject{parent}
{}

/**
 * @brief XFeedModel::getModelFromData Fetch data from file and return it as a standard model
 * @param dbPath
 */
xfeed_model_t XFeedModel::getModelFromData(const QString &dbPath)
{
    qDebug()<<"getModelFromData:"<<dbPath;
    jsonAdapter_.setDbFileName(dbPath);
    model_ =   jsonAdapter_.createModelFromJson();
    return model_;
}

void XFeedModel::addFolder(const QString &folderName)
{
    auto folderItem = std::make_unique<QStandardItem>(folderName);

    model_->appendRow(folderItem.release()); // Transfer ownership to the model
    auto jdoc =jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channelsArray = root["Folders"].toArray();
    channelsArray.append(folderName);
    root["Folders"] = channelsArray;
    jsonAdapter_.exportToJson(QJsonDocument(root));

}
// Helper function to find a folder in the model
QStandardItem* XFeedModel::findFolder(const QString& folderName) const {
    QList<QStandardItem*> folderItems = model_->findItems(folderName, Qt::MatchExactly, 0);
    return folderItems.isEmpty() ? nullptr : folderItems.first();
}
void XFeedModel::addChannel(const ChannelInfo &channelInfo) {

    // Find the corresponding folder
    if (QStandardItem* folderItem = findFolder(channelInfo.chFolder())) {
        // Create a new channel item
        auto* channelItem = new QStandardItem(channelInfo.getChName());
        channelItem->setData(channelInfo.getChName(), Qt::UserRole);  // Store Name
        channelItem->setData(channelInfo.getChAddr(), Qt::UserRole + 1);  // Store URL
        channelItem->setData(channelInfo.chUUID().toString(), Qt::UserRole + 2);  // Store UUID
        channelItem->setData(channelInfo.chFolder(), Qt::UserRole + 3);  // Store folderName

        folderItem->appendRow(channelItem); // Add to folder
        qDebug() << "Added new channel:" << channelInfo.getChName() << "under folder:" << folderItem->text();

        // Update JSON

        updateJsonDatabase(channelInfo);
        qDebug()<<"Json File updated";
    } else {
        qWarning() << "Folder not found in model:" << channelInfo.chFolder();
    }
}

void XFeedModel::editChannel(const QModelIndex& idx, const ChannelInfo &channelInfo)
{
    if (!idx.isValid()) {
        qWarning() << "Invalid index in editChannel";
        return;
    }

    if (!model_) {
        qWarning() << "Model is null in editChannel";
        return;
    }

    QStandardItem* folderItem = findFolder(channelInfo.chFolder());
    if (!folderItem) {
        qWarning() << "Folder not found: " << channelInfo.chFolder();
        return;
    }


    QMap<int, QVariant> itemdata;
    itemdata.insert(Qt::UserRole, channelInfo.getChName());
    itemdata.insert(Qt::UserRole + 1, channelInfo.getChAddr());

    // itemdata.insert(Qt::UserRole + 2, channelInfo.chUUID().toString());

    bool success = model_->setItemData(idx, itemdata);
    if(success)
    {
        QStandardItem* item = model_->itemFromIndex(idx);
        item->setText(channelInfo.getChName());  // Update the visible text!

        emit model_->dataChanged(idx, idx, {Qt::DisplayRole, Qt::EditRole, Qt::UserRole, Qt::UserRole + 1});
        editJsonDatabase(idx);
    }
}

void XFeedModel::deleteChannel(const QModelIndex &indx)
{
    if(!indx.isValid())
    {
        throw std::invalid_argument("Invalid item idnex");
    }

    if(!model_)
    {
        throw std::invalid_argument("Invalid Model: model_");
    }

    // QStandardItem* folderItem = findFolder(channelInfo.chFolder());
    // if (!folderItem) {
    //     qWarning() << "Folder not found: " << channelInfo.chFolder();
    //     return;
    // }
    // model_->removeRows(indx.row(),1);
    deleteItemFromJsonDatabase(indx);
    model_->removeRows(indx.row(),1, indx.parent());

}

void XFeedModel::deleteFolder(const QModelIndex &index)
{
    jsonAdapter_.createModelFromJson();
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channels = root["Channels"].toArray();

    QString folderName = model_->data(index,Qt::UserRole + 1).toString();
    qDebug()<<"Target folder to delete:"<<folderName;
    //First delete all channels in this folder
    for (int i = channels.size() - 1; i >= 0; --i) {
        if (channels[i].toObject()["folderName"].toString() == folderName) {
            channels.removeAt(i);  // Safe removal in reverse order
        }
    }

    root["Channels"] = channels;

    // Now delete the forlder itself
    QJsonArray folders = root["Folders"].toArray();
    for(auto it = folders.begin(); it != folders.end(); ++ it)
    {
        if(it->toString() == folderName)
        {
            folders.erase(it);
            break;
        }
    }
    root["Folders"] = folders;
    jsonAdapter_.exportToJson(QJsonDocument(root));
    model_->removeRows(index.row(),1);
}


// Helper function to update JSON data
void XFeedModel::updateJsonDatabase(const ChannelInfo &channelInfo) {
    jsonAdapter_.createModelFromJson();
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channels = root["Channels"].toArray();

    QJsonObject newChannel;
    newChannel["folderName"] = channelInfo.chFolder();
    newChannel["name"] = channelInfo.getChName();
    newChannel["url"] = channelInfo.getChAddr();
    newChannel["uuid"] = channelInfo.chUUID().toString();
    channels.append(newChannel);

    root["Channels"] = channels;

    jsonAdapter_.exportToJson(QJsonDocument(root));
}

void XFeedModel::editJsonDatabase(const QModelIndex& idx)
{
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channelsArray = root["Channels"].toArray();

    auto uuid = model_->data(idx, Qt::UserRole + 2).toString();


    // Find and update the matching channel
    bool updated = false;
    for (int i = 0; i < channelsArray.size(); ++i) {
        QJsonObject channel = channelsArray[i].toObject();
        if (channel["uuid"].toString() == uuid) {
            channel["name"] = model_->data(idx, Qt::UserRole ).toString();;
            channel["url"] = model_->data(idx, Qt::UserRole + 1).toString();
            channelsArray[i] = channel; // Update the array
            updated = true;
            break;
        }
    }
    if(updated)
    {
        root["Channels"] = channelsArray;

        jsonAdapter_.exportToJson(QJsonDocument(root));
    }
    else
    {
        qErrnoWarning("Ops, item not found!");
    }
}

void XFeedModel::deleteItemFromJsonDatabase(const QModelIndex &idx)
{
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channelsArray = root["Channels"].toArray();

    auto uuid = model_->data(idx, Qt::UserRole + 2).toString();
    for(auto it = channelsArray.begin(); it != channelsArray.end(); ++it)
    {
        QJsonObject ch = it->toObject();
        if((ch["uuid"].toString() == uuid))
        {
            channelsArray.erase(it);
            break;
        }
    }
    root["Channels"] = channelsArray;
    jsonAdapter_.exportToJson(QJsonDocument(root));
}

