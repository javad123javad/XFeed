#include "mediaplayerdisplaystrategy.h"
#include <QToolBar>
MediaPlayerDisplayStrategy::MediaPlayerDisplayStrategy():
    mediaPlayer(std::make_shared<QMediaPlayer>()),
    audioOut(std::make_shared<QAudioOutput>())
{}

void MediaPlayerDisplayStrategy::display(const ChannelInfo& channel, QWidget* container) {
    // Create and configure QTableView for this channel

    QToolBar * toolbar = (QToolBar*)(container);

    mediaPlayer->setAudioOutput(audioOut.get());
    // // connect(player, &QMediaPlayer::positionChanged, this, &MediaExample::positionChanged);
    playSource = QUrl(channel.getChAddr());
    mediaPlayer->setSource(playSource);
    // audioOut->setVolume(50);

    // Setup model, delegates, etc.
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
