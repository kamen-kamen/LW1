#ifndef ABSTRACTSHAPE_H
#define ABSTRACTSHAPE_H

#include <QPointF>
#include <QString>


class AbstractShape {
protected:
    QPointF m_center;

public:
    AbstractShape(QPointF c) : m_center(c) {}
    virtual ~AbstractShape() {}

    virtual QString getName() const = 0;
    virtual void draw(QPainter *painter) = 0;
    virtual double area() const = 0;
    virtual double perimeter() const = 0;

    virtual void move(double dx, double dy) = 0;
    virtual void rotate(double angle, QPointF pivot) = 0;
    virtual void scale(double factor, QPointF pivot) = 0;

    virtual QPointF getCenter() const { return m_center; }

    virtual void setCenter(QPointF newPos) {
        double dx = newPos.x() - m_center.x();
        double dy = newPos.y() - m_center.y();
        move(dx, dy);
    }

    virtual bool contains(QPointF point) {
        double dx = point.x() - m_center.x();
        double dy = point.y() - m_center.y();
        return (dx*dx + dy*dy) < 900;
    }
};


#endif