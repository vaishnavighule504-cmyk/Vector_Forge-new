#pragma once
#include <QWidget>
#include <memory>
#include "Document.h"
#include "UndoStack.h"

enum class Tool { Select, Circle, Rect, Line };

class Canvas : public QWidget {
public:
    explicit Canvas(QWidget* parent = nullptr);
    void setTool(Tool t);
    void undo();
    void redo();
       
        bool saveToFile(const QString& path) const;
    bool loadFromFile(const QString& path);
protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void clearSelection();

    Document m_doc;
    UndoStack m_undo;
    Tool m_tool = Tool::Select;
    Shape* m_selected = nullptr;       // borrowed pointer
    bool m_dragging = false;
    QPointF m_start;
    QPointF m_last;
    QPointF m_moveTotal;               // total distance moved during one drag
    std::unique_ptr<Shape> m_preview;
};