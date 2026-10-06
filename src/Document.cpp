#include "Document.h"

void Document::addShape(std::unique_ptr<Shape> shape)
{
    m_shapes.push_back(std::move(shape));
}

void Document::draw(QPainter &painter) const
{
    for (const auto &s : m_shapes)
    {
        s->draw(painter); // polymorphism: each shape draws itself
    }
}

Shape *Document::shapeAt(const QPointF &p) const
{
    // Search from the back: the last shape added is drawn on top.
    for (auto it = m_shapes.rbegin(); it != m_shapes.rend(); ++it)
    {
        if ((*it)->contains(p))
        {
            return it->get();
        }
    }
    return nullptr;
}