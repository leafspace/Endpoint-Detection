#include "readaudio.h"

ReadAudio::ReadAudio()
{
    stopped = false;
}

void ReadAudio::setAudio(int in)
{
    audio_fp = in;
}

void ReadAudio::run()
{
    qDeleteAll(speech_voice);
    speech_voice.clear();
    ElemType *graph = NULL;
    if(!stopped){
        while(true){
            graph = new ElemType[BUFSIZE];
            int len = ::read(audio_fp, graph, BUFSIZE * sizeof(ElemType));
            if (len != BUFSIZE * sizeof(ElemType)){
                //Todo : Showing warning information !
            }
            speech_voice.push_back(graph);
            paragraph_voice = graph[0];
        }
    }
    stopped = false;
}

void ReadAudio::stop()
{
    stopped = true;
}
