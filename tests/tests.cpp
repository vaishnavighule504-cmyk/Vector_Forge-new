#include <QtTest>
#include <QJsonArray>
#include <QJsonDocument>
#include <cstddef>
#include <memory>
#include <random>
#include <vector>
#include "CircleShape.h"
#include "Commands.h"
#include "Document.h"
#include "LineShape.h"
#include "PolygonShape.h"
#include "RectShape.h"
#include "UndoStack.h"

class VectorForgeTests : public QObject {
    Q_OBJECT

private slots:
    // ---------- Shapes ----------
    void circleContains() {
        CircleShape c(QPointF(100, 100), 50);
        QVERIFY(c.contains(QPointF(100, 100)));   // center
        QVERIFY(c.contains(QPointF(140, 100)));   // inside
        QVERIFY(!c.contains(QPointF(160, 100)));  // outside
    }

    void rectContains() {
        RectShape r(QRectF(10, 10, 100, 50));
        QVERIFY(r.contains(QPointF(50, 30)));
        QVERIFY(!r.contains(QPointF(5, 5)));
        QVERIFY(!r.contains(QPointF(200, 30)));
    }

    void lineContains() {
        LineShape l(QPointF(0, 0), QPointF(100, 0));
        QVERIFY(l.contains(QPointF(50, 3)));      // within 5 px
        QVERIFY(!l.contains(QPointF(50, 10)));    // too far from the line
        QVERIFY(!l.contains(QPointF(150, 0)));    // beyond the end of the segment
    }

    void polygonContains() {
        QPolygonF tri;
        tri << QPointF(0, 0) << QPointF(100, 0) << QPointF(50, 100);
        PolygonShape p(tri);
        QVERIFY(p.contains(QPointF(50, 30)));
        QVERIFY(!p.contains(QPointF(5, 90)));
    }

    void moveByAllShapes() {
        QPolygonF tri;
        tri << QPointF(0, 0) << QPointF(100, 0) << QPointF(50, 100);
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<CircleShape>(QPointF(100, 100), 50));
        shapes.push_back(std::make_unique<RectShape>(QRectF(10, 10, 100, 50)));
        shapes.push_back(std::make_unique<LineShape>(QPointF(0, 0), QPointF(100, 40)));
        shapes.push_back(std::make_unique<PolygonShape>(tri));

        const QPointF delta(10, 20);
        for (auto& s : shapes) {
            QPointF before = s->boundingRect().topLeft();
            s->moveBy(delta);
            QCOMPARE(s->boundingRect().topLeft(), before + delta);
        }
    }

    void rectNormalizesNegativeSize() {
        RectShape r(QRectF(100, 100, -50, -50));  // dragged up and left
        QCOMPARE(r.boundingRect().width(), 50.0);
        QCOMPARE(r.boundingRect().height(), 50.0);
        QVERIFY(r.contains(QPointF(75, 75)));
    }

    // ---------- Document and Quadtree ----------
    void documentTopmostWins() {
        Document doc;
        auto circle = std::make_unique<CircleShape>(QPointF(100, 100), 50);
        auto rect = std::make_unique<RectShape>(QRectF(120, 100, 100, 100));
        Shape* rawRect = rect.get();
        doc.addShape(std::move(circle));
        doc.addShape(std::move(rect));   // added last, so drawn on top
        // (130,110) is inside both shapes
        QCOMPARE(doc.shapeAt(QPointF(130, 110)), rawRect);
    }

    void documentEmptyQuery() {
        Document doc;
        QVERIFY(doc.shapeAt(QPointF(5, 5)) == nullptr);
        QCOMPARE(doc.count(), std::size_t(0));
    }

    void quadtreeMatchesLinear() {
        std::mt19937 rng(7);
        std::uniform_real_distribution<double> pos(0, 2000), size(5, 60);
        Document doc;
        for (int i = 0; i < 500; ++i) {
            double x = pos(rng), y = pos(rng);
            if (i % 2 == 0) doc.addShape(std::make_unique<CircleShape>(QPointF(x, y), size(rng) / 2));
            else            doc.addShape(std::make_unique<RectShape>(QRectF(x, y, size(rng), size(rng))));
        }
        int mismatches = 0;
        for (int i = 0; i < 1000; ++i) {
            QPointF p(pos(rng), pos(rng));
            if (doc.shapeAt(p) != doc.shapeAtLinear(p)) ++mismatches;
        }
        QCOMPARE(mismatches, 0);
    }

    void indexRefreshesAfterMove() {
        Document doc;
        auto c = std::make_unique<CircleShape>(QPointF(100, 100), 10);
        Shape* raw = c.get();
        doc.addShape(std::move(c));
        QCOMPARE(doc.shapeAt(QPointF(100, 100)), raw);   // builds the index

        raw->moveBy(QPointF(500, 0));
        doc.invalidateIndex();                           // what Canvas does after a move
        QVERIFY(doc.shapeAt(QPointF(100, 100)) == nullptr);
        QCOMPARE(doc.shapeAt(QPointF(600, 100)), raw);
    }

    // ---------- Undo / redo ----------
    void addUndoRedo() {
        Document doc;
        UndoStack stack;
        stack.push(std::make_unique<AddCommand>(
            doc, std::make_unique<CircleShape>(QPointF(50, 50), 20)));
        QCOMPARE(doc.count(), std::size_t(1));
        QVERIFY(stack.undo());
        QCOMPARE(doc.count(), std::size_t(0));
        QVERIFY(stack.redo());
        QCOMPARE(doc.count(), std::size_t(1));
    }

    void deleteUndoKeepsZOrder() {
        Document doc;
        std::vector<Shape*> raws;
        for (int i = 0; i < 3; ++i) {
            auto s = std::make_unique<CircleShape>(QPointF(100.0 * i, 0), 10);
            raws.push_back(s.get());
            doc.addShape(std::move(s));
        }
        UndoStack stack;
        stack.push(std::make_unique<DeleteCommand>(doc, raws[1]));   // delete the middle one
        QCOMPARE(doc.count(), std::size_t(2));
        QVERIFY(stack.undo());
        QCOMPARE(doc.count(), std::size_t(3));
        QCOMPARE(doc.indexOf(raws[1]), std::size_t(1));              // back in the same layer
    }

    void moveUndoRedo() {
        Document doc;
        auto c = std::make_unique<CircleShape>(QPointF(100, 100), 10);
        Shape* raw = c.get();
        doc.addShape(std::move(c));
        QPointF start = raw->boundingRect().topLeft();

        UndoStack stack;
        stack.push(std::make_unique<MoveCommand>(raw, QPointF(30, 40)));
        QCOMPARE(raw->boundingRect().topLeft(), start + QPointF(30, 40));
        stack.undo();
        QCOMPARE(raw->boundingRect().topLeft(), start);
        stack.redo();
        QCOMPARE(raw->boundingRect().topLeft(), start + QPointF(30, 40));
    }

    void newActionClearsRedo() {
        Document doc;
        UndoStack stack;
        stack.push(std::make_unique<AddCommand>(
            doc, std::make_unique<CircleShape>(QPointF(0, 0), 5)));
        stack.undo();
        stack.push(std::make_unique<AddCommand>(
            doc, std::make_unique<CircleShape>(QPointF(9, 9), 5)));
        QVERIFY(!stack.redo());   // the old "future" is gone
    }

    void undoOnEmptyStack() {
        UndoStack stack;
        QVERIFY(!stack.undo());
        QVERIFY(!stack.redo());
    }

    // ---------- JSON ----------
    void jsonRoundTrip() {
        QPolygonF tri;
        tri << QPointF(0, 0) << QPointF(100, 0) << QPointF(50, 100);
        Document a;
        auto c = std::make_unique<CircleShape>(QPointF(100, 100), 40);
        c->setFillColor(Qt::red);
        a.addShape(std::move(c));
        a.addShape(std::make_unique<RectShape>(QRectF(10, 20, 30, 40)));
        a.addShape(std::make_unique<LineShape>(QPointF(1, 2), QPointF(3, 4)));
        a.addShape(std::make_unique<PolygonShape>(tri));

        Document b;
        QVERIFY(b.fromJson(a.toJson()));
        QCOMPARE(b.count(), std::size_t(4));
        QCOMPARE(QJsonDocument(b.toJson()).toJson(), QJsonDocument(a.toJson()).toJson());
    }

    void jsonUnknownTypeLeavesDocumentUntouched() {
        Document doc;
        doc.addShape(std::make_unique<CircleShape>(QPointF(1, 1), 5));

        QJsonObject bad;
        QJsonObject s;
        s["type"] = "Banana";
        QJsonArray arr;
        arr.append(s);
        bad["shapes"] = arr;

        QVERIFY(!doc.fromJson(bad));
        QCOMPARE(doc.count(), std::size_t(1));   // original drawing survived
    }

    void jsonMissingShapesKey() {
        Document doc;
        QVERIFY(!doc.fromJson(QJsonObject()));
    }
};

QTEST_APPLESS_MAIN(VectorForgeTests)
#include "tests.moc"