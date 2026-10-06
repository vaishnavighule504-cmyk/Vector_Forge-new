#include "Canvas.h"
#include <QPainter>

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(800, 600);
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::white);
    p.setPen(QPen(Qt::darkBlue, 2));
    p.setBrush(Qt::cyan);
    p.drawEllipse(QPointF(400, 300), 80, 80);
}