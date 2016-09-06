#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Canvas.h"
#include "PixelPainter.h"
#include "OBJ.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

    void draw();

    float getD() const {
        return _d;
    }

private slots:
    void paintEvent(QPaintEvent*);

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

    void on_sldD_valueChanged(int value);

    void on_btnReset_clicked();

    void on_cbWireframe_toggled(bool checked);

    void on_cbTexture_toggled(bool checked);

    void on_cbLight_toggled(bool checked);

    void on_btnCube_clicked();

    void on_btnIronman_clicked();

    void on_btnChair_clicked();

private:
    Ui::MainWindow* _ui;
    Canvas* _canvas;
    PixelPainter* _pixelPainter;
    OBJ _obj;

    QVector3D _pos;
    QVector3D _scale;
    QVector3D _rotation;
    QVector3D _shearing;
    float _d;

    bool _wireframeMode;
    bool _textureMode;
    bool _lightMode;
};

#endif // MAINWINDOW_H
