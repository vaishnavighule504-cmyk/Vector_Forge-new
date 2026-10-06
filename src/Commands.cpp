#include "Commands.h"

// ---- AddCommand ----
AddCommand::AddCommand(Document& doc, std::unique_ptr<Shape> shape)
    : m_doc(doc), m_shape(std::move(shape)), m_ptr(m_shape.get()), m_index(doc.count()) {}

void AddCommand::redo() {
    m_doc.insertShape(m_index, std::move(m_shape));  // ownership goes to the Document
}

void AddCommand::undo() {
    m_shape = m_doc.takeShape(m_ptr);                // ownership comes back to this command
}

// ---- DeleteCommand ----
DeleteCommand::DeleteCommand(Document& doc, Shape* shape)
    : m_doc(doc), m_ptr(shape) {}

void DeleteCommand::redo() {
    m_index = m_doc.indexOf(m_ptr);                  // remember where it was
    m_shape = m_doc.takeShape(m_ptr);
}

void DeleteCommand::undo() {
    m_doc.insertShape(m_index, std::move(m_shape));  // same position, same z-order
}

// ---- MoveCommand ----
MoveCommand::MoveCommand(Shape* shape, const QPointF& delta)
    : m_shape(shape), m_delta(delta) {}

void MoveCommand::redo() { m_shape->moveBy(m_delta); }
void MoveCommand::undo() { m_shape->moveBy(-m_delta); }