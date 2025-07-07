#ifndef AUDIOSTREAMER_H
#define AUDIOSTREAMER_H

#include "qicon.h"
#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>

class AudioStreamer : public QMediaPlayer
{
    Q_OBJECT
public:
    explicit AudioStreamer(QObject *parent = nullptr);
    ~AudioStreamer();
    void loadMusic();

    // Attributes
    QMediaPlaylist *QPlaylist;

    // PLAYING METHODS
    void previous();
    void next();
    void loop(bool looping = false);    
    // Playlist
    QStringList addTracks();
    void removeTrack(int index);

    // FILESYSTEM METHODS
    void savePlaylist();

    // PROPRIETIES METHODS
    // Setters
    // Playlist
    void setPlaylistPath(QString path);

    // Getters
    QString getSongName();

signals:
    void stopped();
    void loaded();

private:
    // Playback vars
    QString currentSong;
    QString playlistSource = "";
};

#endif // AUDIOSTREAMER_H
