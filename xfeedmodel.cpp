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

// Helper function to update JSON data
void XFeedModel::updateJsonDatabase(const ChannelInfo &channelInfo) {
    auto jdoc = jsonAdapter_.getJsonDoc();
    QJsonObject root = jdoc.object();
    QJsonArray channels = root["Channels"].toArray();

    QJsonObject newChannel;
    newChannel["folderName"] = channelInfo.chFolder();
    newChannel["name"] = channelInfo.getChName();
    newChannel["url"] = channelInfo.getChAddr();

    channels.append(newChannel);
    root["Channels"] = channels;

    jsonAdapter_.exportToJson(QJsonDocument(root));
}

