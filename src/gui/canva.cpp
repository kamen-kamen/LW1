#include "canva.h"
#include <QPainter>

Canva::Canva(QWidget *parent) : QFrame(parent) {}

Canva::~Canva() {
    qDeleteAll(shapes); // Чистим память
}

void Canva::addShape(AbstractShape* s) {
    shapes.append(s);
    selectedShapes.clear();
    selectedShapes.append(s); // При создании выбираем только новую
    update();
}

void Canva::paintEvent(QPaintEvent *event) {
    QFrame::paintEvent(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    for (AbstractShape* shape : shapes) {
        // Если фигура есть в списке выбранных — рисуем красным
        if (selectedShapes.contains(shape)) {
            painter.setPen(QPen(Qt::green, 3));
        } else {
            painter.setPen(QPen(Qt::white, 1));
        }
        shape->draw(&painter);
    }
}

void Canva::mousePressEvent(QMouseEvent *event) {
    AbstractShape* clickedShape = nullptr;

    // Ищем, по чему кликнули
    for (int i = shapes.size() - 1; i >= 0; --i) {
        if (shapes[i]->contains(event->pos())) {
            clickedShape = shapes[i];
            break;
        }
    }

    // Если НЕ зажат Ctrl — сбрасываем старый выбор
    if (!(event->modifiers() & Qt::ControlModifier)) {
        selectedShapes.clear();
    }

    if (clickedShape) {
        if (selectedShapes.contains(clickedShape)) {
            selectedShapes.removeOne(clickedShape); // Убираем, если уже была
        } else {
            selectedShapes.append(clickedShape);    // Добавляем в список
        }
    }

    // Сигнализируем MainWindow (передаем nullptr, если выбор пуст, или последнюю фигуру)
    emit shapeSelected(selectedShapes.isEmpty() ? nullptr : selectedShapes.last());
    update();
}

