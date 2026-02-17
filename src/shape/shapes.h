//
// Created by poled on 11.02.2026.
//

#ifndef ALIAPAINT_SHAPES_H
#define ALIAPAINT_SHAPES_H


#include <QPainter>

#include "abstractshape.h"
#include <QVector>
#include <QtMath>


class PolygonShape : public AbstractShape {
protected:
    QVector<QPointF> points; // Вершины

public:
    explicit PolygonShape(QPointF c) : AbstractShape(c) {}

    void draw(QPainter *p) override {
        p->drawPolygon(points);
        p->drawEllipse(m_center, 1, 1);
    }

    void move(double dx, double dy) override {
        m_center += QPointF(dx, dy);
        for (QPointF &pt : points) {
            pt += QPointF(dx, dy);
        }
    }

    void rotate(double angleDeg, QPointF pivot) override {
        double rad = qDegreesToRadians(angleDeg);
        double cosA = cos(rad);
        double sinA = sin(rad);

        for (QPointF &pt : points) {
            double dx = pt.x() - pivot.x();
            double dy = pt.y() - pivot.y();
            pt.setX(pivot.x() + dx * cosA - dy * sinA);
            pt.setY(pivot.y() + dx * sinA + dy * cosA);
        }
        // Если крутим не вокруг центра, центр тоже сместится
        if (pivot != m_center) {
             double dx = m_center.x() - pivot.x();
             double dy = m_center.y() - pivot.y();
             m_center.setX(pivot.x() + dx * cosA - dy * sinA);
             m_center.setY(pivot.y() + dx * sinA + dy * cosA);
        }
    }

    void scale(double factor, QPointF pivot) override {
        for (QPointF &pt : points) {
            pt = pivot + (pt - pivot) * factor;
        }
        m_center = pivot + (m_center - pivot) * factor;
    }

    double perimeter() const override {
        if (points.size() < 2) return 0;
        double p = 0;
        for (int i = 0; i < points.size(); ++i) {
            QPointF p1 = points[i];
            QPointF p2 = points[(i + 1) % points.size()];
            double dx = p2.x() - p1.x();
            double dy = p2.y() - p1.y();
            p += std::sqrt(dx*dx + dy*dy);
        }
        return p;
    }

    double area() const override {
        if (points.size() < 3) return 0;
        double a = 0;
        for (int i = 0; i < points.size(); ++i) {
            QPointF p1 = points[i];
            QPointF p2 = points[(i + 1) % points.size()];
            // Формула площади Гаусса
            a += (p1.x() * p2.y()) - (p2.x() * p1.y());
        }
        return std::abs(a) / 2.0;
    }
    QString getName() const override { return "Polygon"; }
};

class RectShape : public PolygonShape {
public:
    RectShape(QPointF c, double w, double h) : PolygonShape(c) {
        // Строим точки относительно центра
        points << QPointF(c.x() - w/2, c.y() - h/2)
               << QPointF(c.x() + w/2, c.y() - h/2)
               << QPointF(c.x() + w/2, c.y() + h/2)
               << QPointF(c.x() - w/2, c.y() + h/2);
    }
    QString getName() const override { return "Прямоугольник"; }
};

class TriangleShape : public PolygonShape {
public:
    TriangleShape(QPointF c, double size) : PolygonShape(c) {
        double h = size * sqrt(3) / 2;
        points << QPointF(c.x(), c.y() - 2*h/3)
               << QPointF(c.x() - size/2, c.y() + h/3)
               << QPointF(c.x() + size/2, c.y() + h/3);
    }
    QString getName() const override { return "Треугольник"; }
};

class CircleShape : public AbstractShape {
    double radius;
public:
    CircleShape(QPointF c, double r) : AbstractShape(c), radius(r) {}

    void draw(QPainter *p) override {
        p->drawEllipse(m_center, radius, radius);
        p->drawEllipse(m_center, 1, 1);
    }
    void move(double dx, double dy) override { m_center += QPointF(dx, dy); }
    
    void rotate(double, QPointF pivot) override {
    } 
    
    void scale(double factor, QPointF) override { radius *= factor; }
    
    double area() const override { return M_PI * radius * radius; }
    double perimeter() const override { return 2 * M_PI * radius; }
    QString getName() const override { return "Круг"; }
};

class RhombusShape : public PolygonShape {
public:
    RhombusShape(QPointF c, double width, double height) : PolygonShape(c) {
        points << QPointF(c.x(), c.y() - height/2)  // Верх
               << QPointF(c.x() + width/2, c.y())   // Право
               << QPointF(c.x(), c.y() + height/2)  // Низ
               << QPointF(c.x() - width/2, c.y());  // Лево
    }
    QString getName() const override { return "Ромб"; }
};

class HexagonShape : public PolygonShape {
public:
    HexagonShape(QPointF c, double radius) : PolygonShape(c) {
        for (int i = 0; i < 6; ++i) {
            double angle = qDegreesToRadians(i * 60.0);
            points << QPointF(c.x() + radius * cos(angle),
                              c.y() + radius * sin(angle));
        }
    }
    QString getName() const override { return "Шестиугольник"; }
};

class SquareShape : public PolygonShape {
public:
    SquareShape(QPointF c, double side) : PolygonShape(c) {
        double s = side / 2.0;
        points << QPointF(c.x() - s, c.y() - s)
               << QPointF(c.x() + s, c.y() - s)
               << QPointF(c.x() + s, c.y() + s)
               << QPointF(c.x() - s, c.y() + s);
    }
    QString getName() const override { return "Квадрат"; }
};

class StarShape : public PolygonShape {
    int m_rays;
public:
    StarShape(QPointF c, int rays, double outR, double inR)
        : PolygonShape(c), m_rays(rays) {
        for (int i = 0; i < 2 * rays; ++i) {
            double r = (i % 2 == 0) ? outR : inR;
            double angle = qDegreesToRadians(i * (360.0 / (2 * rays)) - 90);
            points << QPointF(c.x() + r * cos(angle),
                              c.y() + r * sin(angle));
        }
    }
    QString getName() const override { return QString("Звезда (%1 лучей)").arg(m_rays); }
};

class LShape : public PolygonShape {
public:
    LShape(QPointF c, double size) : PolygonShape(c) {
        double s = size / 2.0;
        points << QPointF(c.x() - s,   c.y() - s)   // Левый верх
               << QPointF(c.x() - s/3, c.y() - s)   // Внутренний угол
               << QPointF(c.x() - s/3, c.y() + s/3) // Внутренний сгиб
               << QPointF(c.x() + s,   c.y() + s/3) // Правый выступ
               << QPointF(c.x() + s,   c.y() + s)   // Правый низ
               << QPointF(c.x() - s,   c.y() + s);  // Левый низ
    }
    QString getName() const override { return "L-фигура (невыпуклая)"; }
};



class CartShape : public RectShape {
public:
    CartShape(QPointF c, double w, double h) : RectShape(c, w, h) {

        double cargoW = w * 0.5;
        double cargoH = h * 0.6;

        points << QPointF(c.x() - cargoW/2, c.y() - h/2 - cargoH)
               << QPointF(c.x() + cargoW/2, c.y() - h/2 - cargoH)
               << QPointF(c.x() + cargoW/2, c.y() - h/2)
               << QPointF(c.x() - cargoW/2, c.y() - h/2);
    }

    QString getName() const override { return "Тележка"; }

    void draw(QPainter *p) override {
        p->setRenderHint(QPainter::Antialiasing);

        p->setBrush(QBrush(Qt::darkCyan));
        QVector<QPointF> bodyPoints = {points[0], points[1], points[2], points[3]};
        p->drawPolygon(bodyPoints);

        p->setBrush(QBrush(Qt::green));
        QVector<QPointF> cargoPoints = {points[4], points[5], points[6], points[7]};
        p->drawPolygon(cargoPoints);

        p->setBrush(Qt::black);

        double wheelR = std::sqrt(std::pow(points[0].x() - points[1].x(), 2) +
                                  std::pow(points[0].y() - points[1].y(), 2)) * 0.1;

        p->drawEllipse(points[3], wheelR, wheelR);
        p->drawEllipse(points[2], wheelR, wheelR);
    }
};

#endif
