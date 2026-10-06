#pragma once
#include <cstddef>
#include <memory>
#include <QPointF>
#include "Command.h"
#include "Document.h"

class AddCommand : public Command {
public:
    AddCommand(Document& doc, std::unique_ptr<Shape> shape);
    void redo() override;
    void undo() override;

private:
    Document& m_doc;
    std::unique_ptr<Shape> m_shape;  // owns the shape only while it is NOT in the document
    Shape* m_ptr;                    // borrowed pointer, used to find the shape again
    std::size_t m_index;
};

class DeleteCommand : public Command {
public:
    DeleteCommand(Document& doc, Shape* shape);
    void redo() override;
    void undo() override;

private:
    Document& m_doc;
    std::unique_ptr<Shape> m_shape;  // owns the shape while it is deleted
    Shape* m_ptr;
    std::size_t m_index = 0;
};

class MoveCommand : public Command {
public:
    MoveCommand(Shape* shape, const QPointF& delta);
    void redo() override;
    void undo() override;

private:
    Shape* m_shape;
    QPointF m_delta;
};