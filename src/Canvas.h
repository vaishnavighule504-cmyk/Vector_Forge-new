#pragma once
#include <QWidget>
#include "Document.h"

enum class Tool { Select, Circle, Rect, Line };

class Canvas : public QWidget {
public:
    explicit Canvas(QWidget* parent = nullptr);
    void setTool(Tool t);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;

private:
    void clearSelection();

    Document m_doc;
    Tool m_tool = Tool::Select;
    Shape* m_selected = nullptr;   // borrowed pointer, Document owns it
    bool m_dragging = false;
    QPointF m_start;               // where the mouse went down
    QPointF m_last;                // previous mouse position
    std::unique_ptr<Shape> m_preview;  // shape being drawn right now
};