#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QInputDialog>
#include <QTimer>
#include <ui_mainwindow.h>

#include "../shape/abstractshape.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Кнопки создания
    void on_btnAddRect_clicked();
    void on_btnAddTriangle_clicked();
    void on_btnAddCircle_clicked();
    void on_btnAddSquare_clicked();
    void on_btnAddRhombus_clicked();
    void on_btnAddHexagon_clicked();
    void on_btnAddStar5_clicked();
    void on_btnAddStar6_clicked();
    void on_btnAddStar8_clicked();
    void on_btnAddLShape_clicked();

    void on_btnAddCart_clicked();

    // void startQuickMove(double tx, double ty);
    // void on_btnUp_clicked();
    // void on_btnDown_clicked();
    // void on_btnLeft_clicked();
    // void on_btnRight_clicked();
    //
    // void keyPressEvent(QKeyEvent *event);

    // Кнопки анимации
    void on_btnMove_clicked();
    void on_btnRotate_clicked();
    void on_btnScale_clicked();

    void on_btnDelete_clicked();
    
    // Слот таймера
    void processStep();
    
    // Слот от Канвы
    void onShapeSelected(AbstractShape* s);

private:
    Ui::mainwindow *ui;
    QTimer *animTimer;

    int remainingSteps = 0;
    double dX = 0, dY = 0;
    double stepRotation = 0;
    double stepScale = 1.0;
};
#endif