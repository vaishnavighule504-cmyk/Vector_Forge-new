#include "Canvas.h"
#include <QKeyEvent>
#include <QKeySequence>
#include <QMouseEvent>
#include <QPainter>
#include "CircleShape.h"
#include "Commands.h"
#include "LineShape.h"
#include "RectShape.h"
#include <QFile> 
 #include <QJsonDocument>

Canvas::Canvas(QWidget* parent) : QWidget(parent) {
    setMinimumSize(800, 600);
    setFocusPolicy(Qt::StrongFocus);
}

void Canvas::setTool(Tool t) {
    m_tool = t;
    clearSelection();
    update();
}

void Canvas::clearSelection() {
    if (m_selected) m_selected->setSelected(false);
    m_selected = nullptr;
}

void Canvas::undo() {
    clearSelection();   // the selected shape might be about to leave the document
    m_undo.undo();
    m_doc.invalidateIndex();
    update();
}

void Canvas::redo() {
    clearSelection();
    m_undo.redo();
    m_doc.invalidateIndex();
    update();
}

void Canvas::paintEvent(QPaintEvent*) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.fillRect(rect(), Qt::white);
    m_doc.draw(p);
    if (m_preview) m_preview->draw(p);
}

void Canvas::mousePressEvent(QMouseEvent* event) {
    setFocus();
    m_start = m_last = event->position();
    m_moveTotal = QPointF(0, 0);
    m_dragging = true;

    if (m_tool == Tool::Select) {
        clearSelection();
        m_selected = m_doc.shapeAt(m_start);
        if (m_selected) m_selected->setSelected(true);
    }
    update();
}

void Canvas::mouseMoveEvent(QMouseEvent* event) {
    if (!m_dragging) return;
    QPointF pos = event->position();

    switch (m_tool) {
    case Tool::Select:
        if (m_selected) {
            m_selected->moveBy(pos - m_last);   // live movement while dragging
            m_doc.invalidateIndex();
            m_moveTotal += pos - m_last;
        }
        break;
    case Tool::Circle: {
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

void Canvas::mouseReleaseEvent(QMouseEvent*) {
    m_dragging = false;

    if (m_preview) {
        m_undo.push(std::make_unique<AddCommand>(m_doc, std::move(m_preview)));
        m_preview = nullptr;
    }
    // The shape already moved during the drag, so record it without re-running it.
    if (m_tool == Tool::Select && m_selected && !m_moveTotal.isNull()) {
        m_undo.push(std::make_unique<MoveCommand>(m_selected, m_moveTotal), false);
    }
    m_moveTotal = QPointF(0, 0);
    update();
}

void Canvas::keyPressEvent(QKeyEvent* event) {
    if (event->matches(QKeySequence::Undo)) { undo(); return; }
    if (event->matches(QKeySequence::Redo)) { redo(); return; }

    if (event->key() == Qt::Key_Delete && m_selected) {
        Shape* s = m_selected;
        clearSelection();   // un-highlight first, so an undone delete comes back clean
        m_undo.push(std::make_unique<DeleteCommand>(m_doc, s));
        update();
    }
}


bool Canvas::saveToFile(const QString& path) const {
    QFile f(path);
    if (!f.open(QIODevice::WriteOnly)) return false;
    f.write(QJsonDocument(m_doc.toJson()).toJson(QJsonDocument::Indented));
    return true;
}

bool Canvas::loadFromFile(const QString& path) {
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) return false;
    QJsonParseError err;
    QJsonDocument d = QJsonDocument::fromJson(f.readAll(), &err);
    if (err.error != QJsonParseError::NoError || !d.isObject()) return false;

    clearSelection();                     // before shapes are destroyed
    if (!m_doc.fromJson(d.object())) return false;
    m_undo.clear();                       // old commands point at destroyed shapes
    m_preview.reset();
    update();
    return true;
}