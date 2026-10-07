#pragma once
#include <memory>
#include <QJsonObject>
#include "Shape.h"

// Factory: builds the correct Shape subclass from a JSON description.
class ShapeFactory {
public:
    // Returns nullptr if the JSON is invalid or the type is unknown.
    static std::unique_ptr<Shape> fromJson(const QJsonObject& o);
};