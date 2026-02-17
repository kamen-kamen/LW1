#include "canva.h"
#include <QPainter>

Canva::Canva(QWidget *parent) : QFrame(parent) {
    setFocusPolicy(Qt::StrongFocus);
    moveTimer = new QTimer(this);
    connect(moveTimer, &QTimer::timeout, this, &Canva::moveLoop);
    moveTimer->start(16);
}

Canva::~Canva() {
    qDeleteAll(shapes);
}

void Canva::addShape(AbstractShape* s) {
    shapes.append(s);
    selectedShapes.clear();
    selectedShapes.append(s);
    this->setFocus();
    update();
}

void Canva::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    for (AbstractShape* shape : shapes) {
        if (selectedShapes.contains(shape)) {
            painter.setPen(QPen(Qt::green, 3));
        } else {
            painter.setPen(QPen(Qt::white, 1));
        }
        shape->draw(&painter);
    }
}

void Canva::mousePressEvent(QMouseEvent *event) {
    this->setFocus();
    AbstractShape* clickedShape = nullptr;
    for (int i = shapes.size() - 1; i >= 0; --i) {
        if (shapes[i]->contains(event->pos())) {
            clickedShape = shapes[i];
            break;
        }
    }
    if (!(event->modifiers() & Qt::ControlModifier)) {
        selectedShapes.clear();
    }
    if (clickedShape) {
        if (!selectedShapes.contains(clickedShape)) {
            selectedShapes.append(clickedShape);
        } else if (event->modifiers() & Qt::ControlModifier) {
            selectedShapes.removeOne(clickedShape);
        }
    }
    emit shapeSelected(selectedShapes.isEmpty() ? nullptr : selectedShapes.last());
    update();
}

void Canva::keyPressEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    pressedKeys.insert(event->key());
}

void Canva::keyReleaseEvent(QKeyEvent *event) {
    if (event->isAutoRepeat()) return;
    pressedKeys.remove(event->key());
}

void Canva::moveLoop() {
    if (selectedShapes.isEmpty() || pressedKeys.isEmpty()) return;

    double dx = 0, dy = 0;
    double step = 2.0;

    if (pressedKeys.contains(Qt::Key_W)) dy -= step;
    if (pressedKeys.contains(Qt::Key_S)) dy += step;
    if (pressedKeys.contains(Qt::Key_A)) dx -= step;
    if (pressedKeys.contains(Qt::Key_D)) dx += step;

    if (dx != 0 || dy != 0) {
        for (AbstractShape* s : selectedShapes) {
            s->move(dx, dy);
        }
        update();
        emit shapeSelected(selectedShapes.last());
    }
}