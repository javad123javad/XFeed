#ifndef CHANNELMODELBUILDER_H
#define CHANNELMODELBUILDER_H

#include <QStandardItemModel>
#include <QJsonArray>

class ChannelModelBuilder
{
public:
    ChannelModelBuilder();

    void addFolders(const QJsonArray& folderArray);
    void addTypes(const QJsonArray& chTypeArray);
    void addChannels(const QJsonArray& channelsArray);

    std::shared_ptr<QStandardItemModel> build(){
        return std::move(model_);
    }

private:
    std::shared_ptr<QStandardItemModel> model_;
    QMap<QString, QStandardItem*> folderMap_;
};

#endif // CHANNELMODELBUILDER_H
