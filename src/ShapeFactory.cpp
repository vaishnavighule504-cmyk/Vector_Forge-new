#include "ShapeFactory.h"
#include <QJsonArray>
#include "CircleShape.h"
#include "LineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"

namespace {
QPointF readPoint(const QJsonValue& v) {
    QJsonArray a = v.toArray();
    return QPointF(a.at(0).toDouble(), a.at(1).toDouble());
}
}

std::unique_ptr<Shape> ShapeFactory::fromJson(const QJsonObject& o) {
    const QString type = o["type"].toString();
    std::unique_ptr<Shape> shape;

    if (type == "Circle") {
        shape = std::make_unique<CircleShape>(readPoint(o["center"]), o["radius"].toDouble());
    } else if (type == "Rectangle") {
        QJsonArray r = o["rect"].toArray();
        if (r.size() != 4) return nullptr;
        shape = std::make_unique<RectShape>(
            QRectF(r.at(0).toDouble(), r.at(1).toDouble(),
                   r.at(2).toDouble(), r.at(3).toDouble()));
    } else if (type == "Line") {
        shape = std::make_unique<LineShape>(readPoint(o["p1"]), readPoint(o["p2"]));
    } else if (type == "Polygon") {
        QJsonArray pts = o["points"].toArray();
        if (pts.size() < 3) return nullptr;
        QPolygonF poly;
        for (const QJsonValue& v : pts) poly << readPoint(v);
        shape = std::make_unique<PolygonShape>(poly);
    } else {
        return nullptr;   // unknown type
    }

    QColor stroke(o["stroke"].toString());
    QColor fill(o["fill"].toString());
    if (stroke.isValid()) shape->setStrokeColor(stroke);
    if (fill.isValid()) shape->setFillColor(fill);
    if (o["width"].isDouble()) shape->setStrokeWidth(o["width"].toDouble());
    return shape;
}