#ifndef CHANNELFACTORY_H
#define CHANNELFACTORY_H
#include <QStandardItem>
#include "channelinfo.h"
class ChannelFactory
{

public:
    static std::unique_ptr<QStandardItem> createChannelItem(const ChannelInfo& channel) {
        auto item = std::make_unique<QStandardItem>(channel.getChName());
        item->setData(channel.getChName(), Qt::UserRole);
        item->setData(channel.getChAddr(), Qt::UserRole + 1);
        item->setData(channel.chUUID(), Qt::UserRole + 2);
        item->setData(channel.chFolder(), Qt::UserRole + 3);
        item->setData(channel.chType(), Qt::UserRole + 4);  // Store Type

        return item;
    }


};

#endif // CHANNELFACTORY_H
