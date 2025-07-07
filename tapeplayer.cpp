#include "tapeplayer.h"
#include "metaextractor.h"
#include "ui_tapeplayer.h"
#include "tapeconfiguration.h"

TapePlayer::TapePlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::TapePlayer)
{
    ui->setupUi(this);

    // Set internal properties
    this->au = new AudioStreamer(this);
    this->au->setVolume(ui->volumeControl->value());
    ui->seeker->setEnabled(false);

    // Set playlist
    QObject::connect(this->au, SIGNAL(loaded()), this, SLOT(updatePlaylistUI()));

    // Playlist choose
    QObject::connect(this->ui->lwPLaylist, &QListWidget::itemDoubleClicked, [=]() {
        this->playAt(this->selectedIndex);
    });

    // Get Selected item
    QObject::connect(this->ui->lwPLaylist, &QListWidget::currentItemChanged, [=](QListWidgetItem *item) {
        this->selectedIndex = this->ui->lwPLaylist->row(item);
        listItem = item;
    });

    // Connect song label
    QObject::connect(au, &AudioStreamer::mediaStatusChanged, [=](QMediaPlayer::MediaStatus stat) {
        if (stat == QMediaPlayer::BufferedMedia) {
            QString currentSong;
            if (au->availableMetaData().contains("Title")) {
                currentSong = au->metaData("Title").toString();
            } else {
                currentSong = au->currentMedia().request().url().fileName();
            }
            ui->lbTitle->setText(currentSong);

            // Connect notifications
            if (this->notify) {
                this->sendDBUSNotification(currentSong);
            }
        } else if (stat == QMediaPlayer::NoMedia && !isClosing) {
            ui->btPlay->setChecked(false);
            ui->btPlay->setIcon(QIcon(":/assets/icon.png"));
            ui->seeker->setEnabled(false);
            ui->seeker->setValue(0);
        }
    });

    // Connect volume
    QObject::connect(ui->volumeControl, &QSlider::valueChanged, this, &TapePlayer::updateVolumeUI);

    // Connect seeker
    /*QObject::connect(ui->seeker, &QSlider::sliderMoved, [=](qint64 pos) {
        au->setPosition(pos);
    });*/
    QObject::connect(au, &AudioStreamer::durationChanged, ui->seeker, &QSlider::setMaximum);
    QObject::connect(au, &AudioStreamer::positionChanged, ui->seeker, &QSlider::setValue);
}

TapePlayer::~TapePlayer()
{
    delete ui;
}

void TapePlayer::closeEvent(QCloseEvent *event) {
    this->isClosing = true;

    QObject::disconnect(au);
    QObject::disconnect(ui->lwPLaylist);
    QObject::disconnect(ui->volumeControl);
}

// PLAYBACK
void TapePlayer::on_btPlay_clicked(bool checked)
{
    // Check Media
    if (!au->playlist()->isEmpty()) {
        if (checked) {
            au->play();
            ui->btPlay->setIcon(QIcon::fromTheme(tr("media-playback-pause")));
            ui->seeker->setEnabled(true);
        } else {
            au->pause();
            ui->btPlay->setIcon(QIcon::fromTheme(tr("media-playback-start")));
        }
    }
    // If there's no media...
    else {
        ui->btPlay->setChecked(false);
        qWarning() << "WARN: No media, unable to play";
    }
}

void TapePlayer::playAt(int index) {
    this->au->stop();
    ui->seeker->setEnabled(false);
    this->au->playlist()->setCurrentIndex(index);

    this->on_btPlay_clicked(true);
    ui->seeker->setEnabled(true);
    this->ui->btPlay->setChecked(true);
}

void TapePlayer::on_btPrev_clicked()
{
    au->previous();
}

void TapePlayer::on_btNext_clicked()
{
    au->next();
}

void TapePlayer::on_btStop_clicked()
{
    au->stop();
    ui->seeker->setEnabled(false);
    ui->seeker->setValue(0);
    if (ui->btPlay->isChecked()) {
        ui->btPlay->setChecked(false);
        ui->btPlay->setIcon(QIcon::fromTheme(tr("media-playback-start")));
    }
}

// VOLUME
void TapePlayer::on_btVolume_clicked(bool checked)
{
    if (checked) {
        ui->btVolume->setIcon(QIcon::fromTheme(tr("audio-volume-muted")));
        ui->volumeControl->setEnabled(false);
        au->setMuted(true);
    } else {
        this->updateVolumeUI(ui->volumeControl->value()); // Set volume icon based on the volume level
        ui->volumeControl->setEnabled(true);
        au->setMuted(false);
    }
}

// PLAYLIST
void TapePlayer::on_btAddTrack_clicked()
{
    QStringList items = au->addTracks();

    for (auto track : items) {
        this->ui->lwPLaylist->addItem(getAudioMetadata(track));
        qInfo() << "INFO: Added:" << track;
    }
}

void TapePlayer::on_btRemTrack_clicked()
{
    if (this->selectedIndex != -1) {
        this->au->QPlaylist->removeMedia(this->selectedIndex);
        this->ui->lwPLaylist->removeItemWidget(listItem);

        qInfo() << "INFO: Removed:" << listItem->text();

        this->selectedIndex = -1;
        delete listItem;
    }
}

void TapePlayer::on_btLoop_toggled(bool checked)
{
    this->au->loop(checked);
}

void TapePlayer::on_btSave_clicked()
{
    au->savePlaylist();
}

// Options
void TapePlayer::on_btConfig_clicked()
{
    TapeConfiguration *conf = new TapeConfiguration();
    conf->show();
}

// UIDATE UI
// Volume
void TapePlayer::updateVolumeUI(int volume) {
    if (volume == 0) {
        ui->btVolume->setIcon(QIcon::fromTheme(tr("audio-volume-muted")));
    } else if (volume > 0 && volume <= 30) {
        ui->btVolume->setIcon(QIcon::fromTheme(tr("audio-volume-low")));
    } else if (volume > 30 && volume <= 60) {
        ui->btVolume->setIcon(QIcon::fromTheme(tr("audio-volume-medium")));
    } else {
        ui->btVolume->setIcon(QIcon::fromTheme(tr("audio-volume-high")));
    }

    this->au->setVolume(volume);
}

// Playlist
void TapePlayer::updatePlaylistUI() {
    QMediaPlayer tempPlayer;
    QStringList items;

    tempPlayer.setMedia(this->au->QPlaylist);
    while (tempPlayer.mediaStatus() != QMediaPlayer::NoMedia) {
        if (tempPlayer.availableMetaData().contains("Title")) {
            items.append(tempPlayer.metaData("Title").toString());
        } else {
            items.append(tempPlayer.currentMedia().request().url().fileName());
        }
        tempPlayer.playlist()->next();
    }

    ui->lwPLaylist->clear();
    ui->lwPLaylist->addItems(items);
}

// Notification
void TapePlayer::sendDBUSNotification(QString title) {
    QDBusInterface Notifications("org.freedesktop.Notifications",
                                 "/org/freedesktop/Notifications",
                                 "org.freedesktop.Notifications",
                                 QDBusConnection::sessionBus());

    if (!Notifications.isValid()) {
        qCritical() << "FATAL: Failed to connect to D-Bus notification service.";
        return;
    }

    QString appName = "Desktop Agent - Tape";
    uint processId = 0;
    QString appIcon = "media-tape";
    QString summary = "Now listening";
    QStringList actions;
    QVariantMap hints;
    int timeout = 5000;

    QDBusReply<uint> reply = Notifications.call(
        "Notify",
        appName,
        processId,
        appIcon,
        summary,
        title,
        actions,
        hints,
        timeout);

    if (!reply.isValid()) {
        qCritical() << "FATAL: D-Bus call failed:" << reply.error().message();
    }
}
