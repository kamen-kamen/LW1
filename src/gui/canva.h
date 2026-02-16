#ifndef CANVA_H
#define CANVA_H

#include <QFrame>
#include <QList>
#include <QMouseEvent>
#include "../shape/abstractshape.h"

class Canva : public QFrame {
    Q_OBJECT
public:
    explicit Canva(QWidget *parent = nullptr);
    ~Canva();

    void addShape(AbstractShape* s);

    QList<AbstractShape*> selectedShapes; // Список всех выделенных фигур
    QList<AbstractShape*> shapes;         // Все фигуры на холсте

    signals:
        void shapeSelected(AbstractShape* s); // Сигнал в MainWindow

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override; // Клик
};
#endif