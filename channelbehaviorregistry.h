#ifndef CHANNELBEHAVIORREGISTRY_H
#define CHANNELBEHAVIORREGISTRY_H
#include <map>
#include <QString>
#include "displaystrategy.h"
#include "tableviewdisplaystrategy.h"


class ChannelBehaviorRegistry
{
private:
    static ChannelBehaviorRegistry* instance_;
    std::map<QString, std::shared_ptr<DisplayStrategy>> displayStrategies_;

    ChannelBehaviorRegistry() {
        // Register default strategies
        displayStrategies_["RSS"] = std::make_shared<TableViewDisplayStrategy>();
        // displayStrategies_["Radio"] = std::make_shared<MediaPlayerDisplayStrategy>();
        // displayStrategies_["TV"] = std::make_shared<MediaPlayerDisplayStrategy>();
    }

public:
    static ChannelBehaviorRegistry* instance() {
        if (!instance_) {
            instance_ = new ChannelBehaviorRegistry();
        }
        return instance_;
    }

    DisplayStrategy* getDisplayStrategy(const QString& channelType) {
        auto it = displayStrategies_.find(channelType);
        if (it != displayStrategies_.end()) {
            return it->second.get();
        }
        // Return default or nullptr
        return nullptr;
    }

    void registerDisplayStrategy(const QString& channelType, std::shared_ptr<DisplayStrategy> strategy) {
        displayStrategies_[channelType] = strategy;
    }
};

#endif // CHANNELBEHAVIORREGISTRY_H
