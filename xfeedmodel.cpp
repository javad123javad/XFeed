#include "xfeedmodel.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QXmlStreamReader>
#include "channelfactory.h"
XFeedModel::XFeedModel(QObject *parent)
    : QObject{parent}
{
    net_manager = new QNetworkAccessManager(this);
    connect(net_manager, &QNetworkAccessManager::finished, this, &XFeedModel::net_opr_finished);
}

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
    QList<QStandardItem*> folderItems = model_->findItems(folderName, Qt::MatchExactly | Qt::MatchRecursive, 0);
    return folderItems.isEmpty() ? nullptr : folderItems.first();
}
void XFeedModel::addChannel(const ChannelInfo &channelInfo)
{
    bool itemAdded = false;
    // First find type category
    auto chTypes = model_->findItems(channelInfo.chType());
    QStandardItem * chType = chTypes.first();
    auto* channelItem = ChannelFactory::createChannelItem(channelInfo).release();
    for(int i = 0; i < chType->rowCount(); i++)
    {
        if(chType->hasChildren())
        {
            if(chType->child(i,0)->data().toString() == channelInfo.chFolder())
            {
                // Create a new channel item

                chType->child(i,0)->appendRow(channelItem); // Add to folder
                itemAdded = true;
                break;
            }
        }
    }

    if(!itemAdded)
    {
        QStandardItem *cnNewType = new QStandardItem(channelInfo.chFolder());
        cnNewType->appendRow(channelItem);
        chType->appendRow(cnNewType);
    }

    updateJsonDatabase(channelInfo);
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
    qDebug()<<"Deleting Channel!";
    if(!indx.isValid())
    {
        qDebug()<<"Invalid item idnex";

        throw std::invalid_argument("Invalid item idnex");
    }

    if(!model_)
    {
        qDebug()<<"Invalid Model: model_";

        throw std::invalid_argument("Invalid Model: model_");
    }

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

void XFeedModel::fetchChannel(const QModelIndex &index)
{
    QString url = model_->data(index, Qt::UserRole+1).toString();
    net_manager->get(QNetworkRequest(QUrl(url)));
    // QMediaPlayer *player = new QMediaPlayer(this);
    // QAudioOutput *audioOutput = new QAudioOutput(this);
    // player->setAudioOutput(audioOutput);
    // // connect(player, &QMediaPlayer::positionChanged, this, &MediaExample::positionChanged);
    // player->setSource(QUrl("http://stream.radioparadise.com/mp3-192"));
    // audioOutput->setVolume(50);
    // player->play();


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
    newChannel["type"] = channelInfo.chType();
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
    qDebug()<<"UUID:"<<uuid;
    for(auto it = channelsArray.begin(); it != channelsArray.end(); ++it)
    {
        QJsonObject ch = it->toObject();

        if((ch["uuid"].toString() == uuid))
        {
            qDebug()<<"Channel Found for Deleting! in JSON";
            channelsArray.erase(it);
            break;
        }
    }
    root["Channels"] = channelsArray;
    jsonAdapter_.exportToJson(QJsonDocument(root));
}

void XFeedModel::net_opr_finished(QNetworkReply* netReply)
{
    // char buf[1024]  ={0};
    // while(netReply->read(buf, 1024)!=-1)
    // {
    //     qDebug()<<buf;
    // }
    // QXmlStreamReader xml(netReply);
    xmlReader.parseFeed(netReply, feedDataModel_);
    emit feed_data_ready(feedDataModel_);


}

