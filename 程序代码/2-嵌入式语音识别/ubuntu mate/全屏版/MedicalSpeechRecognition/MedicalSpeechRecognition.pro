#-------------------------------------------------
#
# Project created by QtCreator 2017-01-27T10:41:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedicalSpeechRecognition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    playaudio.cpp \
    readaudio.cpp \
    recognitiononline.cpp \
    voice.cpp

HEADERS  += mainwindow.h \
    graph.h \
    playaudio.h \
    readaudio.h \
    recognitiononline.h \
    voice.h \
    xfMSC/msp_cmn.h \
    xfMSC/msp_errors.h \
    xfMSC/msp_types.h \
    xfMSC/qisr.h \
    xfMSC/qtts.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/pi/release/ -lmsc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/pi/debug/ -lmsc
else:unix: LIBS += -L$$PWD/lib/pi/ -lmsc

INCLUDEPATH += $$PWD/lib/pi
DEPENDPATH += $$PWD/lib/pi
