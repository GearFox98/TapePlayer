QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    audiostreamer.cpp \
    tapeconfiguration.cpp \
    tapeplayer.cpp

HEADERS += \
    audiostreamer.h \
    metaextractor.h \
    tapeconfiguration.h \
    tapeplayer.h

FORMS += \
    tapeconfiguration.ui \
    tapeplayer.ui

TRANSLATIONS += \
    TapePlayer_es_ES.ts
#CONFIG += lrelease
CONFIG += embed_translations

LIBS += -ltag

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    README.md
