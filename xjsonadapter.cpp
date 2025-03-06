#include "xjsonadapter.h"
#include <QJsonObject>
#include <QJsonArray>

XJSonAdapter::XJSonAdapter(const QString &file, QObject *parent)
    : fileName_(file), QObject{parent}
{
    if(fileName_.isEmpty())
        throw std::invalid_argument("Invalid File Name");

    readJsonFile();
}

/**
 * @brief XJSonAdapter::readJsonFile
 * throws exception if the file is not valid
 */
void XJSonAdapter::readJsonFile()
{


    dbFile_.setFileName(fileName_);
    if(!dbFile_.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::logic_error("Unable to open db File");
    QString strJson = dbFile_.readAll();
    dbFile_.close();

    jdoc_ = QJsonDocument::fromJson(strJson.toUtf8());

    if(jdoc_.isEmpty())
        throw std::logic_error("Empty Json file");

}

void XJSonAdapter::exportToJson(const QString file)
{

}

std::unique_ptr<QStandardItemModel> XJSonAdapter::createModelFromJson() {
    auto itemModel = std::make_unique<QStandardItemModel>();

    if (jdoc_.isNull()) {
        throw std::logic_error("Invalid JSON Document");
    }

    const QJsonObject root = jdoc_.object();
    const QJsonArray channelsArray = root["Channels"].toArray();

    // Use a map to group channels by type
    QMap<QString, QStandardItem*> folderMap;

    for (const QJsonValue& channelValue : channelsArray) {
        const QJsonObject channelObj = channelValue.toObject();
        const QString type = channelObj["type"].toString(); // e.g., "News", "General"
        const QString name = channelObj["name"].toString();
        const QString url = channelObj["url"].toString();
        const QString uuid = channelObj["uuid"].toString();

        // Create or find the folder item
        if (!folderMap.contains(type)) {
            auto folderItem = std::make_unique<QStandardItem>(type);
            folderMap[type] = folderItem.get(); // Store raw pointer
            itemModel->appendRow(folderItem.release()); // Transfer ownership to the model
        }

        // Add the channel to the folder
        auto channelItem = std::make_unique<QStandardItem>(name);
        channelItem->setData(uuid, Qt::UserRole); // Store UUID
        channelItem->setData(url, Qt::UserRole); // Url
        folderMap[type]->appendRow(channelItem.release()); // Transfer ownership to the model
    }

    return std::move(itemModel);
}

const QJsonDocument XJSonAdapter::getJsonDoc() const
{
    return jdoc_;
}
