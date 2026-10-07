#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QFileDialog>
#include <QMessageBox>
#include "Canvas.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    Canvas* canvas = new Canvas(&window);
    window.setCentralWidget(canvas);

    QToolBar* bar = window.addToolBar("Tools");
    auto* group = new QActionGroup(&window);
    auto addTool = [&](const QString& name, Tool t) {
        QAction* a = bar->addAction(name);
        a->setCheckable(true);
        group->addAction(a);
        QObject::connect(a, &QAction::triggered, [canvas, t] { canvas->setTool(t); });
        return a;
    };
    addTool("Select", Tool::Select)->setChecked(true);
    addTool("Circle", Tool::Circle);
    addTool("Rectangle", Tool::Rect);
    addTool("Line", Tool::Line);

    bar->addSeparator();
    QObject::connect(bar->addAction("Undo"), &QAction::triggered, [canvas] { canvas->undo(); });
    QObject::connect(bar->addAction("Redo"), &QAction::triggered, [canvas] { canvas->redo(); });

    bar->addSeparator();
    QObject::connect(bar->addAction("Save"), &QAction::triggered, [&window, canvas] {
        QString path = QFileDialog::getSaveFileName(&window, "Save drawing", "", "JSON files (*.json)");
        if (path.isEmpty()) return;
        if (!path.endsWith(".json")) path += ".json";
        if (!canvas->saveToFile(path))
            QMessageBox::warning(&window, "Save failed", "Could not write the file.");
    });
    QObject::connect(bar->addAction("Open"), &QAction::triggered, [&window, canvas] {
        QString path = QFileDialog::getOpenFileName(&window, "Open drawing", "", "JSON files (*.json)");
        if (path.isEmpty()) return;
        if (!canvas->loadFromFile(path))
            QMessageBox::warning(&window, "Open failed", "That file is not a valid VectorForge drawing.");
    });

    window.setWindowTitle("VectorForge");
    window.show();
    return app.exec();
}