#include "RectShape.h"

RectShape::RectShape(const QRectF &rect) : m_rect(rect.normalized()) {}

void RectShape::draw(QPainter &painter) const
{
    applyStyle(painter);
    painter.drawRect(m_rect);
}

bool RectShape::contains(const QPointF &p) const
{
    return m_rect.contains(p);
}

void RectShape::moveBy(const QPointF &delta)
{
    m_rect.translate(delta);
}

QRectF RectShape::boundingRect() const
{
    return m_rect;
}