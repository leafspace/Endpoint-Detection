#ifndef GRAPH_H
#define GRAPH_H

#include "voice.h"
#include <QWidget>
#include <qpainter.h>
#include <QtCore/qmath.h>

class Graph : public QWidget
{
    Q_OBJECT
public:
    explicit Graph(QWidget *parent = 0);

    void setAudio(int audioID);
    void startTime(double timer_t);
    void stopTime();
signals:

public slots:
private:
    float *graph_data;
    int graph_space;
    int timerID;

    void paintEvent(QPaintEvent*);
    void timerEvent(QTimerEvent*);
};

#endif // GRAPH_H
