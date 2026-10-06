#pragma once
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QRectF>
#include <QString>

// Abstract base class: you can't create a Shape directly,
// only its children (Circle, Rect, ...).
class Shape
{
public:
    virtual ~Shape() = default; // virtual destructor: needed for safe delete via Shape*

    virtual void draw(QPainter &painter) const = 0;
    virtual bool contains(const QPointF &p) const = 0;
    virtual void moveBy(const QPointF &delta) = 0;
    virtual QRectF boundingRect() const = 0;
    virtual QString typeName() const = 0;

    void setStrokeColor(const QColor &c) { m_stroke = c; }
    void setFillColor(const QColor &c) { m_fill = c; }
    void setStrokeWidth(double w) { m_width = w; }

protected:
    void applyStyle(QPainter &painter) const
    {
        painter.setPen(QPen(m_stroke, m_width));
        painter.setBrush(m_fill);
    }

    QColor m_stroke = Qt::black;
    QColor m_fill = Qt::white;
    double m_width = 2.0;
};