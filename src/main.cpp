#include <QApplication>
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include "Canvas.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    Canvas* canvas = new Canvas(&window);  // Qt deletes it with the window
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

    window.setWindowTitle("VectorForge");
    window.show();
    return app.exec();
}