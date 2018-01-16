#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T17:04:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedicalSpeechRecognition
TEMPLATE = app

#INCLUDEPATH += include/
#LIBS += -L lib -lmsc -lrt -ldl -lpthread

SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    voice.cpp \
    readaudio.cpp \
    playaudio.cpp \
    recognitiononline.cpp

HEADERS  += mainwindow.h \
    graph.h \
    voice.h \
    readaudio.h \
    playaudio.h \
    recognitiononline.h \
    xfMSC/msp_cmn.h \
    xfMSC/msp_errors.h \
    xfMSC/msp_types.h \
    xfMSC/qisr.h \
    xfMSC/qtts.h

FORMS    += mainwindow.ui

DISTFILES += \
    wav/iflytek01.wav \
    wav/iflytek02.wav \
    wav/weather.pcm



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/release/ -lmsc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/debug/ -lmsc
else:unix: LIBS += -L$$PWD/lib/ -lmsc

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib
