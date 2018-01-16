#include "playaudio.h"
#include <QMessageBox>
PlayAudio::PlayAudio()
{
    stopped = false;
}

void PlayAudio::setAudio(int out)
{
    audio_fp = out;
}

void PlayAudio::run()
{
    int len;
    if(!stopped){
        for(int i = 0; i < speech_voice.size(); ++i){
            len = ::write(audio_fp, speech_voice[i], BUFSIZE * sizeof(ElemType));
            if (len != BUFSIZE * sizeof(ElemType)){
                //Todo : Showing warning information !
            }

        }
        stopped = true;
    }
}

void PlayAudio::stop()
{
    stopped = true;
}
