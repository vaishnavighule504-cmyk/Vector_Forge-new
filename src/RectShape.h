#pragma once
#include "Shape.h"

class RectShape : public Shape
{
public:
    explicit RectShape(const QRectF &rect);

    void draw(QPainter &painter) const override;
    bool contains(const QPointF &p) const override;
    void moveBy(const QPointF &delta) override;
    QRectF boundingRect() const override;
    QString typeName() const override { return QStringLiteral("Rectangle"); }
    QJsonObject toJson() const override;
private:
    QRectF m_rect;
};