#include "Quadtree.h"

Quadtree::Quadtree(const QRectF& bounds, int depth)
    : m_bounds(bounds), m_depth(depth) {}

void Quadtree::insert(const Entry& e) {
    if (m_split) {
        for (auto& c : m_children) {
            if (c->m_bounds.contains(e.box)) {   // fits fully inside one child
                c->insert(e);
                return;
            }
        }
        m_entries.push_back(e);                  // straddles a boundary: keep here
        return;
    }

    m_entries.push_back(e);
    if (m_entries.size() > kCapacity && m_depth < kMaxDepth) {
        subdivide();
    }
}

void Quadtree::subdivide() {
    m_split = true;
    const double hw = m_bounds.width() / 2.0;
    const double hh = m_bounds.height() / 2.0;
    const double x = m_bounds.x();
    const double y = m_bounds.y();

    m_children[0] = std::make_unique<Quadtree>(QRectF(x,      y,      hw, hh), m_depth + 1);
    m_children[1] = std::make_unique<Quadtree>(QRectF(x + hw, y,      hw, hh), m_depth + 1);
    m_children[2] = std::make_unique<Quadtree>(QRectF(x,      y + hh, hw, hh), m_depth + 1);
    m_children[3] = std::make_unique<Quadtree>(QRectF(x + hw, y + hh, hw, hh), m_depth + 1);

    std::vector<Entry> old = std::move(m_entries);
    m_entries.clear();
    for (const Entry& e : old) insert(e);        // redistribute into children
}

void Quadtree::query(const QPointF& p, std::vector<Entry>& out) const {
    for (const Entry& e : m_entries) {
        if (e.box.contains(p)) out.push_back(e);
    }
    if (m_split) {
        for (const auto& c : m_children) {
            if (c->m_bounds.contains(p)) c->query(p, out);
        }
    }
}