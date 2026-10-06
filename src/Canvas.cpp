#include "Canvas.h"
#include <QMouseEvent>
#include <QPainter>
#include <memory>
#include <utility>
#include "CircleShape.h"
#include "LineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(800, 600);

    auto circle = std::make_unique<CircleShape>(QPointF(200, 200), 80);
    circle->setFillColor(Qt::cyan);
    m_doc.addShape(std::move(circle));

    auto rect = std::make_unique<RectShape>(QRectF(400, 120, 200, 130));
    rect->setFillColor(QColor("orange"));
    m_doc.addShape(std::move(rect));

    QPolygonF tri;
    tri << QPointF(200, 450) << QPointF(320, 560) << QPointF(80, 560);
    auto poly = std::make_unique<PolygonShape>(tri);
    poly->setFillColor(QColor("lightgreen"));
    m_doc.addShape(std::move(poly));

    auto line = std::make_unique<LineShape>(QPointF(420, 350), QPointF(700, 500));
    line->setStrokeColor(Qt::red);
    line->setStrokeWidth(4);
    m_doc.addShape(std::move(line));
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::white);
    m_doc.draw(p);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    Shape *s = m_doc.shapeAt(event->position());
    if (s)
    {
        setWindowTitle(QStringLiteral("Clicked: ") + s->typeName());
    }
    else
    {
        setWindowTitle(QStringLiteral("VectorForge - nothing here"));
    }
}