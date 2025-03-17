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

        folderItem->appendRow(channelItem); // Add to folder
        qDebug() << "Added new channel:" << channelInfo.getChName() << "under folder:" << folderItem->text();

        // Update JSON
        updateJsonDatabase(channelInfo);
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

    // QMap<int, QVariant> itemdata;
    // itemdata.insert(Qt::UserRole, channelInfo.getChName());
    // itemdata.insert(Qt::UserRole + 1, channelInfo.getChAddr());

    // model_->setItemData(idx, itemdata);
    qDebug() << "Before edit: Name =" << model_->data(idx, Qt::UserRole).toString();
    qDebug() << "Before edit: Address =" << model_->data(idx, Qt::UserRole + 1).toString();

    // model_->setData(idx, channelInfo.getChName(), Qt::UserRole);
    // model_->setData(idx, channelInfo.getChAddr(), Qt::UserRole + 1);

    QMap<int, QVariant> itemdata;
    itemdata.insert(Qt::UserRole, channelInfo.getChName());
    itemdata.insert(Qt::UserRole + 1, channelInfo.getChAddr());

    bool success = model_->setItemData(idx, itemdata);
    QStandardItem* item = model_->itemFromIndex(idx);
    item->setText(channelInfo.getChName());  // Update the visible text!

    emit model_->dataChanged(idx, idx, {Qt::DisplayRole, Qt::EditRole, Qt::UserRole, Qt::UserRole + 1});
    editJsonDatabase(idx);

}


// Helper function to update JSON data
void XFeedModel::updateJsonDatabase(const ChannelInfo &channelInfo) {
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channels = root["Channels"].toArray();

    QJsonObject newChannel;
    newChannel["folderName"] = channelInfo.chFolder();
    newChannel["name"] = channelInfo.getChName();
    newChannel["url"] = channelInfo.getChAddr();

    root["Channels"] = channels;

    jsonAdapter_.exportToJson(QJsonDocument(root));
}

void XFeedModel::editJsonDatabase(const QModelIndex& idx)
{
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channelsArray = root["Channels"].toArray();
    auto uuid = model_->data(idx, Qt::UserRole + 2).toString();
    qDebug()<<uuid;


    // Find and update the matching channel
    bool updated = false;
    for (int i = 0; i < channelsArray.size(); ++i) {
        QJsonObject channel = channelsArray[i].toObject();
        if (channel["uuid"].toString() == uuid) {
            channel["name"] = model_->data(idx, Qt::UserRole ).toString();;
            channel["url"] = model_->data(idx, Qt::UserRole + 1).toString();;
            channelsArray[i] = channel; // Update the array
            updated = true;
            break;
        }
    }

    root["Channels"] = channelsArray;

    jsonAdapter_.exportToJson(QJsonDocument(root));
}

