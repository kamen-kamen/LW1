#include "mainwindow.h"

#include "canva.h"
#include "ui_mainwindow.h"
#include "../shape/shapes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::mainwindow) {
    ui->setupUi(this);

    animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, &MainWindow::processStep);

    connect(ui->canva, &Canva::shapeSelected, this, &MainWindow::onShapeSelected);
}

MainWindow::~MainWindow() { delete ui; }


void MainWindow::onShapeSelected(AbstractShape* s) {
    auto selected = ui->canva->selectedShapes;
    if (selected.isEmpty()) {
        ui->statusbar->showMessage("Ничего не выбрано");
    } else if (selected.size() == 1) {

        AbstractShape* single = selected.first();
        ui->statusbar->showMessage(QString("%1 | S: %2 | P: %3")
            .arg(single->getName())
            .arg(QString::number(single->area(), 'f', 2))
            .arg(QString::number(single->perimeter(), 'f', 2)));
    } else {

        ui->statusbar->showMessage(QString("Выбрано объектов: %1").arg(selected.size()));
    }
}

void MainWindow::on_btnDelete_clicked() {
    auto &selected = ui->canva->selectedShapes;
    if (selected.isEmpty()) return;


    for (AbstractShape* s : selected) {
        ui->canva->shapes.removeOne(s);
        delete s;
    }

    selected.clear();
    ui->canva->update();
    onShapeSelected(nullptr);
}

void MainWindow::processStep() {
    auto selected = ui->canva->selectedShapes;
    if (selected.isEmpty() || remainingSteps <= 0) {
        animTimer->stop();
        return;
    }

    for (AbstractShape* s : selected) {
        if (dX != 0 || dY != 0) s->move(dX, dY);

        if (stepRotation != 0) s->rotate(stepRotation, s->getCenter());

        if (stepScale != 1.0) s->scale(stepScale, s->getCenter());
    }

    ui->canva->update();
    onShapeSelected(nullptr);
    remainingSteps--;
}


#include <QInputDialog>

void MainWindow::on_btnAddRect_clicked() {
    bool ok;

    double w = QInputDialog::getDouble(this, "Rectangle Settings",
                                       "Enter width:", 100, 1, 2000, 1, &ok);
    if (!ok) return;

    double h = QInputDialog::getDouble(this, "Rectangle Settings",
                                       "Enter height:", 60, 1, 2000, 1, &ok);
    if (!ok) return;
    ui->canva->addShape(new RectShape(QPointF(150, 150), w, h));
}

void MainWindow::on_btnAddTriangle_clicked() {
    bool ok;
    double size = QInputDialog::getDouble(this, "Triangle Settings", "Enter side size:", 100, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new TriangleShape(QPointF(200, 200), size));
    }
}

void MainWindow::on_btnAddCircle_clicked() {
    bool ok;
    double r = QInputDialog::getDouble(this, "Circle Settings", "Enter radius:", 50, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new CircleShape(QPointF(300, 150), r));
    }
}

void MainWindow::on_btnAddSquare_clicked() {
    bool ok;
    double side = QInputDialog::getDouble(this, "Square Settings", "Enter side length:", 80, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new SquareShape(QPointF(200, 200), side));
    }
}

void MainWindow::on_btnAddStar5_clicked() {
    bool ok;
    double outR = QInputDialog::getDouble(this, "Star 5 Settings", "Enter outer radius:", 60, 1, 2000, 1, &ok);
    if (!ok) return;
    double inR = QInputDialog::getDouble(this, "Star 5 Settings", "Enter inner radius:", 25, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new StarShape(QPointF(200, 200), 5, outR, inR));
    }
}

void MainWindow::on_btnAddStar6_clicked() {
    bool ok;
    double outR = QInputDialog::getDouble(this, "Star 6 Settings", "Enter outer radius:", 60, 1, 2000, 1, &ok);
    if (!ok) return;
    double inR = QInputDialog::getDouble(this, "Star 6 Settings", "Enter inner radius:", 30, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new StarShape(QPointF(250, 250), 6, outR, inR));
    }
}

void MainWindow::on_btnAddStar8_clicked() {
    bool ok;
    double outR = QInputDialog::getDouble(this, "Star 8 Settings", "Enter outer radius:", 60, 1, 2000, 1, &ok);
    if (!ok) return;
    double inR = QInputDialog::getDouble(this, "Star 8 Settings", "Enter inner radius:", 35, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new StarShape(QPointF(300, 300), 8, outR, inR));
    }
}

void MainWindow::on_btnAddLShape_clicked() {
    bool ok;
    double size = QInputDialog::getDouble(this, "L-Shape Settings", "Enter size:", 100, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new LShape(QPointF(150, 150), size));
    }
}

void MainWindow::on_btnAddHexagon_clicked() {
    bool ok;
    double side = QInputDialog::getDouble(this, "Hexagon Settings", "Enter side length:", 60, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new HexagonShape(QPointF(200, 200), side));
    }
}

void MainWindow::on_btnAddRhombus_clicked() {
    bool ok;
    double w = QInputDialog::getDouble(this, "Rhombus Settings", "Enter width:", 100, 1, 2000, 1, &ok);
    if (!ok) return;
    double h = QInputDialog::getDouble(this, "Rhombus Settings", "Enter height:", 60, 1, 2000, 1, &ok);

    if (ok) {
        ui->canva->addShape(new RhombusShape(QPointF(200, 200), w, h));
    }
}

void MainWindow::on_btnAddCart_clicked() {
    ui->canva->addShape(new CartShape(QPointF(200, 200), 80, 40));
}

void MainWindow::on_btnScale_clicked() {

    if (ui->canva->selectedShapes.isEmpty() || animTimer->isActive()) return;

    bool ok;
    double totalFactor = QInputDialog::getDouble(this, "Масштаб",
        "Коэффициент (напр. 2.0 - увеличить, 0.5 - уменьшить):", 1.2, 0.1, 10.0, 2, &ok);

    if (ok) {
        remainingSteps = 30;

        stepScale = pow(totalFactor, 1.0 / remainingSteps);

        // Сбрасываем остальные трансформации
        stepRotation = 0; dX = 0; dY = 0;
        animTimer->start(20);
    }
}

void MainWindow::on_btnRotate_clicked() {
    if (ui->canva->selectedShapes.isEmpty() || animTimer->isActive()) return;

    bool ok;
    double totalAngle = QInputDialog::getDouble(this, "Поворот",
        "Угол поворота в градусах:", 90.0, -3600.0, 3600.0, 1, &ok);

    if (ok) {
        remainingSteps = 30;
        stepRotation = totalAngle / remainingSteps;

        stepScale = 1.0; dX = 0; dY = 0;
        animTimer->start(20);
    }
}


void MainWindow::on_btnMove_clicked() {
    if (ui->canva->selectedShapes.isEmpty() || animTimer->isActive()) return;

    bool okX, okY;
    double totalDX = QInputDialog::getDouble(this, "Сдвиг", "По горизонтали (X):", 100, -1000, 1000, 1, &okX);
    if (!okX) return;
    double totalDY = QInputDialog::getDouble(this, "Сдвиг", "По вертикали (Y):", 0, -1000, 1000, 1, &okY);

    if (okY) {
        remainingSteps = 30;
        dX = totalDX / remainingSteps;
        dY = totalDY / remainingSteps;

        stepRotation = 0; stepScale = 1.0;
        animTimer->start(20);
    }
}
