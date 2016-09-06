#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this,
                         QPoint(_ui->frmCanvas->x(), _ui->frmCanvas->y()),
                         QSize(_ui->frmCanvas->width(), _ui->frmCanvas->height()));

    _pixelPainter = new PixelPainter(_canvas);

    on_btnCube_clicked();
}

MainWindow::~MainWindow()
{
    delete _pixelPainter;
    delete _canvas;
    delete _ui;
}

void MainWindow::draw()
{
    _pixelPainter->clear();
    _pixelPainter->drawOBJ(&_obj, _pos, _scale, _rotation, _shearing,
                           _wireframeMode, _textureMode, _lightMode);
    _canvas->update();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _pixelPainter->getCanvas()->draw();
}

void MainWindow::on_sldX_valueChanged(int value)
{
    _pos.setX(value);

    char buffer[128];
    sprintf(buffer, "X: %d", value);
    _ui->lblX->setText(buffer);

    draw();
}

void MainWindow::on_sldY_valueChanged(int value)
{
    _pos.setY(value);

    char buffer[128];
    sprintf(buffer, "Y: %d", value);
    _ui->lblY->setText(buffer);

    draw();
}

void MainWindow::on_sldZ_valueChanged(int value)
{
    _pos.setZ(value);

    char buffer[128];
    sprintf(buffer, "Z: %d", value);
    _ui->lblZ->setText(buffer);

    draw();
}

void MainWindow::on_sldScaleX_valueChanged(int value)
{
    _scale.setX(value);

    char buffer[128];
    sprintf(buffer, "Scale X: %.3f", _scale.x());
    _ui->lblScaleX->setText(buffer);

    draw();
}

void MainWindow::on_sldScaleY_valueChanged(int value)
{
    _scale.setY(value);

    char buffer[128];
    sprintf(buffer, "Scale Y: %.3f", _scale.y());
    _ui->lblScaleY->setText(buffer);

    draw();
}

void MainWindow::on_sldScaleZ_valueChanged(int value)
{
    _scale.setZ(value);

    char buffer[128];
    sprintf(buffer, "Scale Z: %.3f", _scale.z());
    _ui->lblScaleZ->setText(buffer);

    draw();
}

void MainWindow::on_sldRotateX_valueChanged(int value)
{
    _rotation.setX(value / 180.0f * M_PI);
    _rotation.setX(static_cast<int>(_rotation.x() * 100.0f));
    _rotation.setX(_rotation.x() / 100.0f);

    char buffer[128];
    sprintf(buffer, "Rotation X: %.2f", _rotation.x());
    _ui->lblRotationX->setText(buffer);

    draw();
}

void MainWindow::on_sldRotateY_valueChanged(int value)
{
    _rotation.setY(value / 180.0f * M_PI);
    _rotation.setY(static_cast<int>(_rotation.y() * 100.0f));
    _rotation.setY(_rotation.y() / 100.0f);

    char buffer[128];
    sprintf(buffer, "Rotation Y: %.2f", _rotation.y());
    _ui->lblRotationY->setText(buffer);

    draw();
}

void MainWindow::on_sldRotateZ_valueChanged(int value)
{
    _rotation.setZ(value / 180.0f * M_PI);
    _rotation.setZ(static_cast<int>(_rotation.z() * 100.0f));
    _rotation.setZ(_rotation.z() / 100.0f);

    char buffer[128];
    sprintf(buffer, "Rotation Z: %.2f", _rotation.z());
    _ui->lblRotationZ->setText(buffer);

    draw();
}

void MainWindow::on_sldShearingX_valueChanged(int value)
{
    _shearing.setX(static_cast<int>(value / 100.0f) / 10.0f);

    char buffer[128];
    sprintf(buffer, "Shearing X: %.1f", _shearing.x());
    _ui->lblShearingX->setText(buffer);

    draw();
}

void MainWindow::on_sldShearingY_valueChanged(int value)
{
    _shearing.setY(static_cast<int>(value / 100.0f) / 10.0f);

    char buffer[128];
    sprintf(buffer, "Shearing Y: %.1f", _shearing.y());
    _ui->lblShearingY->setText(buffer);

    draw();
}

void MainWindow::on_sldShearingZ_valueChanged(int value)
{
    _shearing.setZ(static_cast<int>(value / 100.0f) / 10.0f);

    char buffer[128];
    sprintf(buffer, "Shearing Z: %.1f", _shearing.z());
    _ui->lblShearingZ->setText(buffer);

    draw();
}

void MainWindow::on_sldD_valueChanged(int value)
{
    _d = value;

    char buffer[128];
    sprintf(buffer, "D: %.3f", _d);
    _ui->lblD->setText(buffer);

    draw();
}

void MainWindow::on_btnReset_clicked()
{
    _pos.setX(0.0f);
    _pos.setY(0.0f);
    _pos.setZ(0.0f);

    _scale.setX(11.0f);
    _scale.setY(10.0f);
    _scale.setZ(10.0f);

    _rotation.setX(0.0f);
    _rotation.setY(0.0f);
    _rotation.setZ(0.0f);

    _shearing.setX(0.0f);
    _shearing.setY(0.0f);
    _shearing.setZ(0.0f);

    _d = 1000.0f;

    _wireframeMode = false;
    _textureMode = true;
    _lightMode = false;

    _ui->sldX->setValue(_pos.x());
    _ui->sldY->setValue(_pos.y());
    _ui->sldZ->setValue(_pos.z());

    _ui->sldScaleX->setValue(_scale.x());
    _ui->sldScaleY->setValue(_scale.y());
    _ui->sldScaleZ->setValue(_scale.z());

    _ui->sldRotateX->setValue(_rotation.x());
    _ui->sldRotateY->setValue(_rotation.y());
    _ui->sldRotateZ->setValue(_rotation.z());

    _ui->sldShearingX->setValue(_shearing.x());
    _ui->sldShearingY->setValue(_shearing.y());
    _ui->sldShearingZ->setValue(_shearing.z());

    _ui->sldD->setValue(_d);

    _ui->cbWireframe->setChecked(_wireframeMode);
    _ui->cbTexture->setChecked(_textureMode);
    _ui->cbLight->setChecked(_lightMode);
}

void MainWindow::on_cbWireframe_toggled(bool checked)
{
    _wireframeMode = checked;

    draw();
}

void MainWindow::on_cbTexture_toggled(bool checked)
{
    _textureMode = checked;

    draw();
}

void MainWindow::on_cbLight_toggled(bool checked)
{
    _lightMode = checked;

    draw();
}

void MainWindow::on_btnCube_clicked()
{
    if (!_obj.load(":/cube/", "cube.obj"))
        exit(EXIT_FAILURE);

    on_btnReset_clicked();

    _ui->sldScaleX->setValue(100.0f);
    _ui->sldScaleY->setValue(100.0f);
    _ui->sldScaleZ->setValue(100.0f);

    draw();
}

void MainWindow::on_btnIronman_clicked()
{
    if (!_obj.load(":/ironman/", "Ironman.obj"))
        exit(EXIT_FAILURE);

    on_btnReset_clicked();

    _ui->sldY->setValue(100.0f);

    _ui->sldScaleX->setValue(100.0f);
    _ui->sldScaleY->setValue(100.0f);
    _ui->sldScaleZ->setValue(100.0f);

    _ui->sldRotateX->setValue(3.14f * 180.0f / M_PI);
    _ui->sldRotateY->setValue(3.56f * 180.0f / M_PI);

    draw();
}

void MainWindow::on_btnChair_clicked()
{
    if (!_obj.load(":/wooden_chair/", "Wooden_Chair.obj"))
        exit(EXIT_FAILURE);

    on_btnReset_clicked();

    _ui->sldScaleX->setValue(100.0f);
    _ui->sldScaleY->setValue(100.0f);
    _ui->sldScaleZ->setValue(100.0f);

    draw();
}
