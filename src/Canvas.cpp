#include "Canvas.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <cmath>
#include "CircleShape.h"
#include "LineShape.h"
#include "RectShape.h"

Canvas::Canvas(QWidget *parent) : QWidget(parent)
{
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus); // needed to receive key presses
}

void Canvas::setTool(Tool t)
{
    m_tool = t;
    clearSelection();
    window()->setWindowTitle(QStringLiteral("VectorForge - tool changed"));
    update();
}

void Canvas::clearSelection()
{
    if (m_selected)
        m_selected->setSelected(false);
    m_selected = nullptr;
}

void Canvas::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::white);
    m_doc.draw(p);
    if (m_preview)
        m_preview->draw(p); // shape being dragged out
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    m_start = m_last = event->position();
    m_dragging = true;

    if (m_tool == Tool::Select)
    {
        clearSelection();
        m_selected = m_doc.shapeAt(m_start);
        if (m_selected)
            m_selected->setSelected(true);
    }
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    if (!m_dragging)
        return;
    QPointF pos = event->position();

    switch (m_tool)
    {
    case Tool::Select:
        if (m_selected)
            m_selected->moveBy(pos - m_last);
        break;
    case Tool::Circle:
    {
        double r = QLineF(m_start, pos).length();
        m_preview = std::make_unique<CircleShape>(m_start, r);
        m_preview->setFillColor(Qt::cyan);
        break;
    }
    case Tool::Rect:
        m_preview = std::make_unique<RectShape>(QRectF(m_start, pos));
        m_preview->setFillColor(QColor("orange"));
        break;
    case Tool::Line:
        m_preview = std::make_unique<LineShape>(m_start, pos);
        m_preview->setStrokeColor(Qt::red);
        break;
    }
    m_last = pos;
    update();
}

void Canvas::mouseReleaseEvent(QMouseEvent *)
{
    m_dragging = false;
    if (m_preview)
    {
        m_doc.addShape(std::move(m_preview)); // preview becomes a real shape
        m_preview = nullptr;
    }
    update();
}

void Canvas::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete && m_selected)
    {
        m_doc.removeShape(m_selected); // Document deletes it
        m_selected = nullptr;          // our borrowed pointer is now dangling, so reset it
        update();
    }
}