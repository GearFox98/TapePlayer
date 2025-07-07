#ifndef METAEXTRACTOR_H
#define METAEXTRACTOR_H

#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2.h>
#include <QUrl>

QString getAudioMetadata(const QString &filepath) {
    QString metadata;
    TagLib::FileRef file(filepath.toUtf8().constData());

    if (!file.isNull() && file.tag()) {
        TagLib::Tag *tag = file.tag();

        metadata = QString::fromStdWString(tag->title().toWString());
        if (metadata == "") {
            metadata = QUrl::fromLocalFile(filepath).fileName();
        }
    }

    return metadata;
}

#endif // METAEXTRACTOR_H
