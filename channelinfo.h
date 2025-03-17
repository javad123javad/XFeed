#ifndef CHANNELINFO_H
#define CHANNELINFO_H
#include <QObject>
#include <QMap>
struct ChannelInfo: public QObject
{
    Q_OBJECT
public:
    ChannelInfo(){};
    ~ChannelInfo() override{}
    ChannelInfo(ChannelInfo& chInfo)
    {
        chName_ = chInfo.chName_;
        chAddr_ = chInfo.chAddr_;
        chComment_ = chInfo.chComment_;
        chFolder_ = chInfo.chFolder_;
    }

    ChannelInfo & operator=(const ChannelInfo & other)
    {
        if(this != &other)
        {
            this->setChAddr(other.chAddr_);
            this->setChName(other.chName_);
            this->setChComment(other.chComment_);
            this->setChFolder(other.chFolder_);
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
        if(newChName.isEmpty())
            throw std::invalid_argument("Invalid channel name, channel name cannot be empty.");

        chName_ = newChName;
    }

    QString getChAddr() const
    {
        return chAddr_;
    }

    void setChAddr(const QString &newChAddr)
    {
        if(newChAddr.isEmpty())
            throw std::invalid_argument("Invalid channel address, channel address cannot be empty.");
        chAddr_ = newChAddr;
    }
    QString chFolder() const;
    void setChFolder(const QString &newChFolder);
    QJsonObject toJson() const;


private:
    QString chName_;
    QString chAddr_;
    QString chComment_;
    QString chFolder_;

};

#endif // CHANNELINFO_H
