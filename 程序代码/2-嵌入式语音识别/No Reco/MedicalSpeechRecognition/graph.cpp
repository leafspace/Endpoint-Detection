#include "graph.h"

Graph::Graph(QWidget *parent) : QWidget(parent)
{
    graph_data = new float[450];
    graph_space = 0;
    for (int i = 0; i < 450; ++i){
        graph_data[i] = 0;
    }
}

void Graph::startTime(double timer_t)
{
    timerID = startTimer(timer_t);
}

void Graph::stopTime()
{
   killTimer(timerID);
}

void Graph::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::gray);
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, 465, 405);

    for (int i = 1; i < 4; ++i){
        painter.drawLine(0, i*101, 465, i*101);
    }

    for (int i = 1; i < 5; ++i){
        painter.drawLine(i*93, 0, i*93, 405);
    }

    painter.setPen(QColor(30, 160, 200));
    int old_y = 0, new_y;
    for (int i = 0; i < 450; ++i){
        new_y = 405 * (1 - graph_data[(graph_space + i) % 450]);
        painter.drawLine(i - 1, old_y, i, new_y);
        old_y = new_y;
    }
}

void Graph::timerEvent(QTimerEvent *)
{
    graph_data[graph_space] = paragraph_voice / qPow(2, SIZE);
    graph_space++;
    update();
}
