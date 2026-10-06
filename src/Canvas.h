#pragma once
#include <QWidget>
#include "Document.h"

class Canvas : public QWidget
{
public:
    explicit Canvas(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    Document m_doc;
};