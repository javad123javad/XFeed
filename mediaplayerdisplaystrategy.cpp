#include "mediaplayerdisplaystrategy.h"
#include <QMediaMetaData>

MediaPlayerDisplayStrategy::MediaPlayerDisplayStrategy():
    mediaPlayer(std::make_shared<QMediaPlayer>()),
    audioOut(std::make_shared<QAudioOutput>())
{}

void MediaPlayerDisplayStrategy::display(const ChannelInfo& channel, QWidget* container)
{
    mediaPlayer->setAudioOutput(audioOut.get());
    if(container !=nullptr)
    {
        statusbar= (QStatusBar*)container;

        lbl_play = statusbar->findChild<QLabel*>("lbl_play");
        if(!lbl_play)
        {
            throw std::invalid_argument("No play label provided!");
        }
    }
    currentChInfo = channel;
    currentName = channel.getChName();
    playSource = QUrl(channel.getChAddr());
    if(!mediaPlayer->isPlaying())// Switch only when we are not playing
        mediaPlayer->setSource(playSource);
}

void MediaPlayerDisplayStrategy::play()
{
    mediaPlayer->setSource(playSource);
    lbl_play->setText("Playing \"" + currentChInfo.getChName() +"\"..." );

    mediaPlayer->play();
}

void MediaPlayerDisplayStrategy::stop()
{
    mediaPlayer->setSource(QUrl());

    lbl_play->setText("Stoped");
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
