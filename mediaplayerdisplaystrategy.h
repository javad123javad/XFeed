#ifndef MEDIAPLAYERDISPLAYSTRATEGY_H
#define MEDIAPLAYERDISPLAYSTRATEGY_H

#include "displaystrategy.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <memory.h>
class MediaPlayerDisplayStrategy : public DisplayStrategy
{
public:
    MediaPlayerDisplayStrategy();

    void display(const ChannelInfo& channel, QWidget* container) override;
    bool isMediaPlaySupported() override{return true;}
    void play() override;
    void stop() override;
    void setMuted(bool muted) override;
    void setVolume(int vol) override;

private:
    std::shared_ptr<QMediaPlayer> mediaPlayer;
    std::shared_ptr<QAudioOutput> audioOut;
    QUrl playSource;
};

#endif // MEDIAPLAYERDISPLAYSTRATEGY_H
