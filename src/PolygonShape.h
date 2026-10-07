#pragma once
#include <QPolygonF>
#include "Shape.h"

class PolygonShape : public Shape
{
public:
    explicit PolygonShape(const QPolygonF &points);

    void draw(QPainter &painter) const override;
    bool contains(const QPointF &p) const override;
    void moveBy(const QPointF &delta) override;
    QRectF boundingRect() const override;
    QString typeName() const override { return QStringLiteral("Polygon"); }
    QJsonObject toJson() const override;
private:
    QPolygonF m_points;
};