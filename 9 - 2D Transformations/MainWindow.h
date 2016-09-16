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

    void drawImage();

private slots:
    void on_sldX_valueChanged(int value);

    void on_sldY_valueChanged(int value);

    void on_sldScaleX_valueChanged(int value);

    void on_sldScaleY_valueChanged(int value);

    void on_sldRotate_valueChanged(int value);

    void on_sldShearingX_valueChanged(int value);

    void on_sldShearingY_valueChanged(int value);

    void on_btnReset_clicked();

private:
    Ui::MainWindow* _ui;

    Canvas* _canvas;

    QImage* _image;

    QPoint _position;
    double _scaleX;
    double _scaleY;
    double _rotation;
    double _shearingX;
    double _shearingY;
};

#endif // MAINWINDOW_H
