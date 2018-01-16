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
    srand((unsigned)time(NULL));
    char *graph;
    if(!stopped){
        while(true){
            graph = new char[BUFSIZE];
            int len = ::read(audio_fp, graph, BUFSIZE);
            if (len != BUFSIZE){
                //printf("");
            }
            speech_voice.append(graph);
            paragraph_voice = graph[0];
        }
    }
    stopped = false;
}

void ReadAudio::stop()
{
    stopped = true;
}
