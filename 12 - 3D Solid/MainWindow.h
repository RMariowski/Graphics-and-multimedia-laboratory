#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector3D>
#include "Canvas.h"
#include "Cube.h"

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

    void draw();

    bool isHideFacesModeOn() const;

    bool isTextureModeOn() const;

    bool isLightModeOn() const;

    QImage* getTexture() const {
        return _texture;
    }

    float getD() const {
        return _d;
    }

private slots:
    void on_sldX_valueChanged(int value);

    void on_sldY_valueChanged(int value);

    void on_sldZ_valueChanged(int value);

    void on_sldScaleX_valueChanged(int value);

    void on_sldScaleY_valueChanged(int value);

    void on_sldScaleZ_valueChanged(int value);

    void on_sldRotateX_valueChanged(int value);

    void on_sldRotateY_valueChanged(int value);

    void on_sldRotateZ_valueChanged(int value);

    void on_sldShearingX_valueChanged(int value);

    void on_sldShearingY_valueChanged(int value);

    void on_sldShearingZ_valueChanged(int value);

    void on_btnReset_clicked();

    void on_sldD_valueChanged(int value);

    void on_cbHideFaces_toggled(bool checked);

    void on_cbShowTexture_toggled(bool checked);

    void on_cbLight_toggled(bool checked);

private:
    Ui::MainWindow* _ui;

    Canvas* _canvas;

    QImage* _texture;

    QVector3D _pos;
    QVector3D _scale;
    QVector3D _rotation;
    QVector3D _shearing;
    float _d;

    Cube _cube;
};

#endif // MAINWINDOW_H
