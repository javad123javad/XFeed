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
