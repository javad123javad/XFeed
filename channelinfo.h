#ifndef CHANNELINFO_H
#define CHANNELINFO_H
#include <QObject>

struct ChannelInfo: public QObject
{
    Q_OBJECT
public:
    ChannelInfo(ChannelInfo& chInfo)
    {
        chName_ = chInfo.chName_;
        chAddr_ = chInfo.chAddr_;
        chComment_ = chInfo.chComment_;
    }

    ChannelInfo & operator=(const ChannelInfo & other)
    {
        if(this != &other)
        {
            this->chAddr_ = other.chAddr_;
            this->chName_ = other.chName_;
            this->chComment_ = other.chComment_;
        }

        return *this;
    }

    QString getChComment() const
    {
        return chComment_;
    }

    void setChComment(const QString &newChComment)
    {
        chComment_ = newChComment;
    }

    QString getChName() const
    {
        return chName_;
    }

    void setChName(const QString &newChName)
    {
        chName_ = newChName;
    }

    QString chAddr() const
    {
        return chAddr_;
    }

    void setChAddr(const QString &newChAddr)
    {
        chAddr_ = newChAddr;
    }
private:
    QString chName_;
    QString chAddr_;
    QString chComment_;
};

#endif // CHANNELINFO_H
