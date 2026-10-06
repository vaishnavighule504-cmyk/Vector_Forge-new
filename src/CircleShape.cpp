#include "CircleShape.h"

CircleShape::CircleShape(const QPointF &center, double radius)
    : m_center(center), m_radius(radius) {}

void CircleShape::draw(QPainter &painter) const
{
    applyStyle(painter);
    painter.drawEllipse(m_center, m_radius, m_radius);
}

bool CircleShape::contains(const QPointF &p) const
{
    return QLineF(p, m_center).length() <= m_radius;
}

void CircleShape::moveBy(const QPointF &delta)
{
    m_center += delta;
}

QRectF CircleShape::boundingRect() const
{
    return QRectF(m_center.x() - m_radius, m_center.y() - m_radius,
                  2 * m_radius, 2 * m_radius);
}