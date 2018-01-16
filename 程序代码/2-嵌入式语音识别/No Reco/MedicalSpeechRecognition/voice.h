#ifndef VOICE_H
#define VOICE_H

#include <QList>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/soundcard.h>

#define SIZE 8
#define LENGTH 3
#define RATE 8000
#define CHANNELS 1
#define BUFSIZE 450
#define GRAPH_SPEED 100

extern int paragraph_voice;
extern QList<char*> speech_voice;

bool OpenDrive(int *in);
bool OpenDrive(int *in, int *out);

int SetDrive(int *in);
int SetDrive(int *in, int *out);

#endif // VOICE_H

