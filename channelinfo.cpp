#include "channelinfo.h"


QString ChannelInfo::chFolder() const
{
    return chFolder_;
}

void ChannelInfo::setChFolder(const QString &newChFolder)
{
    chFolder_ = newChFolder;
}
