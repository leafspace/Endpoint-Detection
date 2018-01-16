#ifndef READAUDIO_H
#define READAUDIO_H

#include "voice.h"
#include <QThread>

class ReadAudio:public QThread
{
    Q_OBJECT
public:
    ReadAudio();
    void setAudio(int in);
    void stop();

protected:
    int audio_fp;
    void run();

private:
    volatile bool stopped;
};

#endif // READAUDIO_H
