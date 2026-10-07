#include "Document.h"
#include <algorithm>
#include <QJsonArray>
#include "ShapeFactory.h"

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

std::unique_ptr<Shape> Document::takeShape(Shape* s) {
    for (auto it = m_shapes.begin(); it != m_shapes.end(); ++it) {
        if (it->get() == s) {
            std::unique_ptr<Shape> owned = std::move(*it);
            m_shapes.erase(it);
            return owned;
        }
    }
    return nullptr;
}

void Document::insertShape(std::size_t index, std::unique_ptr<Shape> shape) {
    if (index > m_shapes.size()) index = m_shapes.size();
    m_shapes.insert(m_shapes.begin() + index, std::move(shape));
}

std::size_t Document::indexOf(const Shape* s) const {
    for (std::size_t i = 0; i < m_shapes.size(); ++i) {
        if (m_shapes[i].get() == s) return i;
    }
    return m_shapes.size();
}

QJsonObject Document::toJson() const {
    QJsonArray arr;
    for (const auto& s : m_shapes) arr.append(s->toJson());
    QJsonObject o;
    o["version"] = 1;
    o["shapes"] = arr;
    return o;
}

bool Document::fromJson(const QJsonObject& o) {
    if (!o["shapes"].isArray()) return false;
    std::vector<std::unique_ptr<Shape>> loaded;      // build into a temporary first
    for (const QJsonValue& v : o["shapes"].toArray()) {
        auto s = ShapeFactory::fromJson(v.toObject());
        if (!s) return false;                        // bad file: current drawing untouched
        loaded.push_back(std::move(s));
    }
    m_shapes = std::move(loaded);
    return true;
}