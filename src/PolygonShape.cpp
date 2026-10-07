#include "PolygonShape.h"

PolygonShape::PolygonShape(const QPolygonF &points) : m_points(points) {}

void PolygonShape::draw(QPainter &painter) const
{
    applyStyle(painter);
    painter.drawPolygon(m_points);
}

bool PolygonShape::contains(const QPointF &p) const
{
    return m_points.containsPoint(p, Qt::OddEvenFill);
}

void PolygonShape::moveBy(const QPointF &delta)
{
    m_points.translate(delta);
}

QRectF PolygonShape::boundingRect() const
{
    return m_points.boundingRect();
}

QJsonObject PolygonShape::toJson() const {
    QJsonObject o;
    o["type"] = typeName();
    QJsonArray pts;
    for (const QPointF& p : m_points) {
        pts.append(QJsonArray{p.x(), p.y()});
    }
    o["points"] = pts;
    writeStyle(o);
    return o;
}