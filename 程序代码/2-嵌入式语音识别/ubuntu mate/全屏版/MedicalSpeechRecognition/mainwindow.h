#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "readaudio.h"
#include "playaudio.h"
#include <QMainWindow>
#include "recognitiononline.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private:
	Ui::MainWindow *ui;

	void paintEvent(QPaintEvent*);
	QPainter *paint;
	bool control_state;
	int voice_fp;
	int play_fp;

	bool drive_ready;
	ReadAudio readThread;
	PlayAudio playThread;
	RecognitionOnline recognitiononlineThread;

	bool connectDrive();
	void createVoice();
	public slots:
	void ClickButton_Conc();
	void ClickButton_Start();
	void ClickButton_Stop();
	void ClickButton_Play();
	void ClickButton_Reco();
	void ClickButton_Exit();
};

#endif // MAINWINDOW_H
