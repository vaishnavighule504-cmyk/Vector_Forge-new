#include "Document.h"
#include <algorithm>

void Document::addShape(std::unique_ptr<Shape> shape)
{
    m_shapes.push_back(std::move(shape));
}

void Document::draw(QPainter &painter) const
{
    for (const auto &s : m_shapes)
    {
        s->draw(painter);
        if (s->isSelected())
        {
            painter.setPen(QPen(Qt::blue, 1, Qt::DashLine));
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(s->boundingRect().adjusted(-4, -4, 4, 4));
        }
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

void Document::removeShape(Shape *s)
{
    m_shapes.erase(
        std::remove_if(m_shapes.begin(), m_shapes.end(),
                       [s](const std::unique_ptr<Shape> &p)
                       { return p.get() == s; }),
        m_shapes.end());
}