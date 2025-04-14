#ifndef TABLEVIEWDISPLAYSTRATEGY_H
#define TABLEVIEWDISPLAYSTRATEGY_H

#include "displaystrategy.h"

class TableViewDisplayStrategy : public DisplayStrategy
{
public:
    TableViewDisplayStrategy();
public:
    void display(const ChannelInfo& channel, QWidget* container) override {
        // Create and configure QTableView for this channel

        // Setup model, delegates, etc.
    }
};

#endif // TABLEVIEWDISPLAYSTRATEGY_H
