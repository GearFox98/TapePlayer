#include "audiostreamer.h"

#include <filesystem>
#include <QFileDialog>

// Object Implementation
AudioStreamer::AudioStreamer(QObject *parent)
    : QMediaPlayer{parent}
{
    // Set variables
    QPlaylist = new QMediaPlaylist();
    this->setMedia(QPlaylist);

    // LOAD PLAYLIST
    if (std::filesystem::exists(playlistSource.toStdString()) && playlistSource != "") {
        QPlaylist->load(QUrl::fromLocalFile(playlistSource), "m3u");
        emit loaded();
        qInfo() << "INFO: Playlist loaded";
    } else {
        qWarning() << "WARN: No playlist, setting an empty one";
    }
}

AudioStreamer::~AudioStreamer() {
    this->stop();
    delete QPlaylist;
}

void AudioStreamer::setPlaylistPath(QString path) {
    this->playlistSource = path;
}

void AudioStreamer::loop(bool looping) {
    if (looping) {
        this->QPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Loop);
    } else {
        this->QPlaylist->setPlaybackMode(QMediaPlaylist::PlaybackMode::Sequential);
    }
}

void AudioStreamer::previous() {
    if (this->playlist()->isEmpty()){
        qCritical() << "FATAL: Playlist is empty!";
    } else {
        this->playlist()->previous();
    }
}

void AudioStreamer::next() {
    if (this->playlist()->isEmpty()){
        qCritical() << "FATAL: Playlist is empty!";
    } else {
        this->playlist()->next();
    }
}

QStringList AudioStreamer::addTracks() {
    QStringList tracks = QFileDialog::getOpenFileNames(
        nullptr,
        tr("Add tracks"),
        "",
        tr("Audio Files (*.mp3 *.wav *.m4a *.ogg *.flac)")
        );

    for(auto track : tracks) {
        QPlaylist->addMedia(QUrl::fromLocalFile(track));
    }

    return tracks;
}

void AudioStreamer::removeTrack(int index) {
    try {
        this->QPlaylist->removeMedia(index);
    } catch (...) {
        qCritical() << "FATAL: Current media erased";
    }
}

void AudioStreamer::savePlaylist() {
    if (!this->playlistSource.isEmpty()) {
        bool saved = this->QPlaylist->save(this->playlistSource, "m3u");
        if (saved) {
            qInfo() << "INFO: List saved at: " << playlistSource;
        }
    } else {
        qCritical() << "FATAL: No playlist defined!";
    }
}
