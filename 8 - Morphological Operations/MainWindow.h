#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Canvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

    void paintEvent(QPaintEvent*);

private slots:
    void on_btnReset_clicked();

    void on_btnDilatation_clicked();

    void on_btnErosion_clicked();

    void on_btnOpening_clicked();

    void on_btnClosing_clicked();

    void on_sldSize_valueChanged(int value);

private:
    Ui::MainWindow* _ui;
    Canvas* _canvas;

    int _markerSize;
};

#endif // MAINWINDOW_H
