#ifndef CANVA_H
#define CANVA_H

#include <QFrame>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTimer>
#include <QSet>
#include "../shape/abstractshape.h"

class Canva : public QFrame {
    Q_OBJECT
public:
    explicit Canva(QWidget *parent = nullptr);
    ~Canva();
    void addShape(AbstractShape* s);
    QList<AbstractShape*> shapes;
    QList<AbstractShape*> selectedShapes;

    signals:
        void shapeSelected(AbstractShape* s);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void moveLoop();

private:
    QTimer *moveTimer;
    QSet<int> pressedKeys;
};

#endif