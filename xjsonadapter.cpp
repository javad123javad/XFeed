#include "xjsonadapter.h"
#include <QJsonObject>
#include <QJsonArray>

XJSonAdapter::XJSonAdapter(const QString &file, QObject *parent)
    : fileName_(file), QObject{parent}
{

}

/**
 * @brief XJSonAdapter::readJsonFile
 * throws exception if the file is not valid
 */
void XJSonAdapter::readJsonFile()
{

    qDebug()<<"Data reading finished!";
    dbFile_.setFileName(fileName_);

    if(!dbFile_.open(QIODevice::ReadOnly | QIODevice::Text))
        throw std::logic_error("Unable to open db File");
    QString strJson = dbFile_.readAll();
    dbFile_.close();

    jdoc_ = QJsonDocument::fromJson(strJson.toUtf8());

    if(jdoc_.isEmpty())
        throw std::logic_error("Empty Json file");

}

void XJSonAdapter::exportToJson(const QJsonDocument& jdoc)
{

    QFile jfile(fileName_);
    jfile.open(QIODevice::WriteOnly);
    jfile.write(jdoc.toJson());
    jfile.close();

}

std::shared_ptr<QStandardItemModel> XJSonAdapter::createModelFromJson() {
    auto itemModel = std::make_shared<QStandardItemModel>();
    readJsonFile();

    if (jdoc_.isNull()) {
        throw std::logic_error("Invalid JSON Document");
    }
    const QJsonObject root = jdoc_.object();
    const QJsonArray folderArray = root["Folders"].toArray();
    const QJsonArray channelsArray = root["Channels"].toArray();
    // Use a map to group channels by type
    QMap<QString, QStandardItem*> folderMap;
    // Fill map with folder items
    for(const QJsonValue& folderValue:folderArray)
    {
        auto folderName = folderValue.toString();
        if (!folderMap.contains(folderName)) {
            auto folderItem = std::make_unique<QStandardItem>(folderName);
            folderMap[folderName] = folderItem.get(); // Store raw pointer
            itemModel->appendRow(folderItem.release()); // Transfer ownership to the model
        }
    }

    for (const QJsonValue& channelValue : channelsArray) {
        const QJsonObject channelObj = channelValue.toObject();
        const QString folderName = channelObj["folderName"].toString();

        // Add the channel to the folder
        auto channelItem = std::make_unique<QStandardItem>(channelObj["name"].toString());

        channelItem->setData(channelObj["name"].toString(), Qt::UserRole); // Name
        channelItem->setData(channelObj["url"].toString(), Qt::UserRole + 1); // Store url
        channelItem->setData(channelObj["uuid"].toString(), Qt::UserRole + 2); // Store uuid
        channelItem->setData(channelObj["folderName"].toString(), Qt::UserRole + 3);


        folderMap[folderName]->appendRow(channelItem.release()); // Transfer ownership to the model


    }

    return std::move(itemModel);
}

const QJsonDocument XJSonAdapter::getJsonDoc() const
{
    return jdoc_;
}

void XJSonAdapter::setDbFileName(const QString &dBFile)
{
    if(dBFile.isEmpty())
        throw std::invalid_argument("Invalid File Name");

    this->fileName_ = dBFile;
}
