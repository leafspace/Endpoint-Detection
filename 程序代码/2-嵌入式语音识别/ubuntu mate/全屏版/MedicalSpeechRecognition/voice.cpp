#include "voice.h"

int paragraph_voice;
QVector<ElemType*> speech_voice;

bool OpenDrive(int *in)
{
	*in = ::open("/dev/audio1", O_RDONLY);
	if (*in == -1) {
		return false;
	}
	return true;
}

bool OpenDrive(int *in, int *out)
{
	*in = ::open("/dev/audio1", O_RDONLY);
	*out = ::open("/dev/audio", O_WRONLY);

	if (*in == -1 || *out == -1) {
		return false;
	}
	return true;
}

int SetDrive(int *in)
{
	int arg;
	bool flag = true;
	int status;

	arg = SIZE;
	status = ioctl(*in, SOUND_PCM_WRITE_BITS, &arg);
	if (status == -1) {
		return -1;
	}
	if (arg != SIZE) {
		status = false;
	}

	arg = CHANNELS;
	status = ioctl(*in, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1) {
		return -1;
	}
	if (arg != CHANNELS) {
		status = false;
	}

	arg = RATE;
	status = ioctl(*in, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1) {
		return -1;
	}
	if (arg != RATE) {
		status = false;
	}

	if (flag) {
		return 1;
	}
	else {
		return 0;
	}
}

int SetDrive(int *in, int *out)
{
	int f_in = SetDrive(in);
	int f_out = SetDrive(out);

	if (f_in == -1 || f_out == -1) {
		return -1;
	}
	else if (f_in == 0 || f_out == 0) {
		return 0;
	}
	else {
		return 1;
	}

}
