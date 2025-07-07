#ifndef TAPEPLAYER_H
#define TAPEPLAYER_H

#include "audiostreamer.h"

#include <QtDBus>
#include <QMainWindow>
#include <QMouseEvent>
#include <QListWidget>

namespace Ui {
class TapePlayer;
}

class TapePlayer : public QMainWindow
{
    Q_OBJECT

public:
    explicit TapePlayer(QWidget *parent = nullptr);
    ~TapePlayer();

protected:
    void closeEvent(QCloseEvent *event) override;

signals:
    void stopped();

private slots:
    // PLAYBACK
    void on_btPlay_clicked(bool checked);
    void playAt(int index);
    void on_btPrev_clicked();
    void on_btNext_clicked();
    void on_btStop_clicked();

    // VOLUME
    void on_btVolume_clicked(bool checked);

    // PLAYLIST
    void on_btLoop_toggled(bool checked);
    void on_btAddTrack_clicked();
    void on_btSave_clicked();

private:
    Ui::TapePlayer *ui;
    AudioStreamer *au;
    QListWidgetItem *listItem;

    bool isClosing = false;
    bool playing = false;
    bool notify = true;
    int selectedIndex = -1;

    // Notification (D-BUS)
    void sendDBUSNotification(QString title);

private slots:
    void updateVolumeUI(int volume);
    void updatePlaylistUI();
    void on_btRemTrack_clicked();
    void on_btConfig_clicked();
};

#endif // TAPEPLAYER_H
