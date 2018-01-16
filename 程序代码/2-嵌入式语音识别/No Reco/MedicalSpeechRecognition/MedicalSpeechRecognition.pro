#-------------------------------------------------
#
# Project created by QtCreator 2017-01-19T17:04:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MedicalSpeechRecognition
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    graph.cpp \
    voice.cpp \
    readaudio.cpp

HEADERS  += mainwindow.h \
    graph.h \
    voice.h \
    readaudio.h

FORMS    += mainwindow.ui
