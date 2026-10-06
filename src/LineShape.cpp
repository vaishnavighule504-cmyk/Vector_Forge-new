#include "LineShape.h"
#include <algorithm>

LineShape::LineShape(const QPointF &p1, const QPointF &p2)
    : m_p1(p1), m_p2(p2) {}

void LineShape::draw(QPainter &painter) const
{
    painter.setPen(QPen(m_stroke, m_width));
    painter.drawLine(m_p1, m_p2);
}

// A line is thin, so "clicked on it" means "within 5 pixels of the segment".
bool LineShape::contains(const QPointF &p) const
{
    QPointF d = m_p2 - m_p1;
    double lenSq = d.x() * d.x() + d.y() * d.y();
    double t = 0.0;
    if (lenSq > 0.0)
    {
        t = ((p.x() - m_p1.x()) * d.x() + (p.y() - m_p1.y()) * d.y()) / lenSq;
        t = std::clamp(t, 0.0, 1.0); // stay on the segment, not the infinite line
    }
    QPointF closest = m_p1 + t * d;
    return QLineF(p, closest).length() <= 5.0;
}

void LineShape::moveBy(const QPointF &delta)
{
    m_p1 += delta;
    m_p2 += delta;
}

QRectF LineShape::boundingRect() const
{
    return QRectF(m_p1, m_p2).normalized();
}