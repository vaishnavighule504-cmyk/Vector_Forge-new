#pragma once
#include <array>
#include <cstddef>
#include <memory>
#include <vector>
#include <QPointF>
#include <QRectF>
#include "Shape.h"

class Quadtree {
public:
    struct Entry {
        Shape* shape;       // borrowed, the Document owns it
        QRectF box;         // padded bounding box
        std::size_t order;  // position in the Document (higher = drawn on top)
    };

    explicit Quadtree(const QRectF& bounds, int depth = 0);

    void insert(const Entry& e);
    // Appends every entry whose box contains p. Callers still do the exact check.
    void query(const QPointF& p, std::vector<Entry>& out) const;

private:
    void subdivide();

    static constexpr std::size_t kCapacity = 8;
    static constexpr int kMaxDepth = 8;

    QRectF m_bounds;
    int m_depth;
    bool m_split = false;
    std::vector<Entry> m_entries;
    std::array<std::unique_ptr<Quadtree>, 4> m_children;
};