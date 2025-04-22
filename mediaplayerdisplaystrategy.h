#ifndef MEDIAPLAYERDISPLAYSTRATEGY_H
#define MEDIAPLAYERDISPLAYSTRATEGY_H

#include "displaystrategy.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStatusBar>
#include <QLabel>

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
    void displayMetaData(const ChannelInfo& channel) override;



private:
    std::shared_ptr<QMediaPlayer> mediaPlayer;
    std::shared_ptr<QAudioOutput> audioOut;
    QUrl playSource;
    QStatusBar * statusbar;
    QLabel* lbl_play;
    QString currentName;
    ChannelInfo currentChInfo;
};

#endif // MEDIAPLAYERDISPLAYSTRATEGY_H
