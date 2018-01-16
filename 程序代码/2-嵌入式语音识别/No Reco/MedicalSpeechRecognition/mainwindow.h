#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "readaudio.h"
#include <QMainWindow>

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
    ReadAudio readThread;

public slots:
    void ClickButton_Start();
    void ClickButton_Stop();
    void ClickButton_Play();
};

#endif // MAINWINDOW_H
