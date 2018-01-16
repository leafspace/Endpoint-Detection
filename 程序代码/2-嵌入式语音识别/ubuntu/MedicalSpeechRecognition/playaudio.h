#ifndef PLAYAUDIO_H
#define PLAYAUDIO_H

#include "voice.h"
#include <QThread>

class PlayAudio:public QThread
{
    Q_OBJECT
public:
    PlayAudio();
    void setAudio(int out);
    void stop();

protected:
    int audio_fp;
    void run();

private:
    volatile bool stopped;
};

#endif // PLAYAUDIO_H
