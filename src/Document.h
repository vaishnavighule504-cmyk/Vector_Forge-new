#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include "Shape.h"
#include <QJsonObject>
#include "Quadtree.h"

// Owns every shape. When the Document dies, all shapes are freed automatically.
class Document
{
public:

    Shape* shapeAtLinear(const QPointF& p) const;   // old O(n) version, kept for benchmarks
    void invalidateIndex() { m_dirty = true; }

    QJsonObject toJson() const;
    bool fromJson(const QJsonObject& o);   // replaces everything; false = invalid file

    void removeShape(Shape* s);
        void clear() { m_shapes.clear(); m_dirty = true; }

        std::unique_ptr<Shape> takeShape(Shape* s);
    void insertShape(std::size_t index, std::unique_ptr<Shape> shape);
    std::size_t indexOf(const Shape* s) const;

    void addShape(std::unique_ptr<Shape> shape);
    void draw(QPainter &painter) const;
    Shape *shapeAt(const QPointF &p) const; // topmost shape under p, or nullptr
    std::size_t count() const { return m_shapes.size(); }

private:
    std::vector<std::unique_ptr<Shape>> m_shapes;

        void rebuildIndex() const;
    mutable std::unique_ptr<Quadtree> m_index;
    mutable bool m_dirty = true;
};