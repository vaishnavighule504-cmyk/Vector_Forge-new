#pragma once
#include "Shape.h"

class CircleShape : public Shape
{
public:
    CircleShape(const QPointF &center, double radius);

    void draw(QPainter &painter) const override;
    bool contains(const QPointF &p) const override;
    void moveBy(const QPointF &delta) override;
    QRectF boundingRect() const override;
    QString typeName() const override { return QStringLiteral("Circle"); }

private:
    QPointF m_center;
    double m_radius;
};