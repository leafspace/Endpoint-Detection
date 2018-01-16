#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_start, SIGNAL(clicked()), this, SLOT(ClickButton_Start()));
    connect(ui->pushButton_stop, SIGNAL(clicked()), this, SLOT(ClickButton_Stop()));
    connect(ui->pushButton_play, SIGNAL(clicked()), this, SLOT(ClickButton_Play()));
    control_state = false;
    paragraph_voice = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{

}

void MainWindow::ClickButton_Start()
{
    if(control_state == true){
        return ;
    }

    int status;
    if(OpenDrive(&voice_fp, &play_fp) == false){
        QMessageBox::critical(this, tr("Error"), tr("File open fuiled !"), QMessageBox::Yes);
        return ;
    }
    if((status = SetDrive(&voice_fp, &play_fp)) == -1){
        QMessageBox::critical(this, tr("Error"), tr("File set fuiled !"), QMessageBox::Yes);
        return ;
    }else if(status == 0){
        QMessageBox::StandardButton rebutton =
                QMessageBox::warning(this, tr("Warning"), tr("Some set defferent !"), QMessageBox::Yes | QMessageBox::Ignore);
        if(rebutton == QMessageBox::Yes){
            return ;
        }
    }

    readThread.setAudio(voice_fp);
    ui->graph->startTime(GRAPH_SPEED);
    readThread.start();
    control_state = true;
}

void MainWindow::ClickButton_Stop()
{
    if(control_state == false){
        return ;
    }
    ui->graph->stopTime();
    readThread.stop();
    ::close(voice_fp);
    ::close(play_fp);
    control_state = false;
}

void MainWindow::ClickButton_Play()
{
    int len;
    for(QList<char*>::iterator i = speech_voice.begin(); i!=speech_voice.end(); ++i){
        len = ::write(play_fp, (*i), BUFSIZE);
        if (len != BUFSIZE){
            //printf("");
        }
    }
}
