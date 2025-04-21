#include "mediaplayerdisplaystrategy.h"
#include <QToolBar>
#include <QMediaMetaData>

MediaPlayerDisplayStrategy::MediaPlayerDisplayStrategy():
    mediaPlayer(std::make_shared<QMediaPlayer>()),
    audioOut(std::make_shared<QAudioOutput>())
{}

void MediaPlayerDisplayStrategy::display(const ChannelInfo& channel, QWidget* container)
{
    mediaPlayer->setAudioOutput(audioOut.get());

    playSource = QUrl(channel.getChAddr());
    if(!mediaPlayer->isPlaying())// Switch only when we are not playing
        mediaPlayer->setSource(playSource);
}

void MediaPlayerDisplayStrategy::play()
{
    mediaPlayer->setSource(playSource);
    mediaPlayer->play();
}

void MediaPlayerDisplayStrategy::stop()
{
    mediaPlayer->setSource(QUrl());
    mediaPlayer->stop();
}

void MediaPlayerDisplayStrategy::setMuted(bool muted)
{
    audioOut->setMuted(muted);
}

void MediaPlayerDisplayStrategy::setVolume(int vol)
{
    audioOut->setVolume(vol);
}

void MediaPlayerDisplayStrategy::displayMetaData(const ChannelInfo &channel)
{
}
