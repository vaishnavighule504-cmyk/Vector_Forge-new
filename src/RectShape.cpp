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

QJsonObject RectShape::toJson() const {
    QJsonObject o;
    o["type"] = typeName();
    o["rect"] = QJsonArray{m_rect.x(), m_rect.y(), m_rect.width(), m_rect.height()};
    writeStyle(o);
    return o;
}