#pragma once
#include "Shape.h"

class LineShape : public Shape
{
public:
    LineShape(const QPointF &p1, const QPointF &p2);

    void draw(QPainter &painter) const override;
    bool contains(const QPointF &p) const override;
    void moveBy(const QPointF &delta) override;
    QRectF boundingRect() const override;
    QString typeName() const override { return QStringLiteral("Line"); }
        QJsonObject toJson() const override;
private:
    QPointF m_p1;
    QPointF m_p2;
};