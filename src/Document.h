#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "Shape.h"

// Owns every shape. When the Document dies, all shapes are freed automatically.
class Document
{
public:
    void addShape(std::unique_ptr<Shape> shape);
    void draw(QPainter &painter) const;
    Shape *shapeAt(const QPointF &p) const; // topmost shape under p, or nullptr
    std::size_t count() const { return m_shapes.size(); }

private:
    std::vector<std::unique_ptr<Shape>> m_shapes;
};