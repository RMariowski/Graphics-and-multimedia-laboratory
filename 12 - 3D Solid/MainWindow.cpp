#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                        _ui->frmCanvas->width(), _ui->frmCanvas->height());

    _texture = new QImage(":/Texture.jpg");
    on_btnReset_clicked();
}

MainWindow::~MainWindow()
{
    delete _texture;
    delete _canvas;
    delete _ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _canvas->draw();
}

void MainWindow::draw()
{
    _canvas->reset();
    _canvas->drawCube(&_cube, _pos, _scale, _rotation, _shearing);
    update();
}

bool MainWindow::isHideFacesModeOn() const
{
    return _ui->cbHideFaces->isChecked();
}

bool MainWindow::isTextureModeOn() const
{
    return _ui->cbShowTexture->isChecked();
}

bool MainWindow::isLightModeOn() const
{
    return _ui->cbLight->isChecked();
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
    _scale.setX(value / 1000.0f);

    char buffer[128];
    sprintf(buffer, "Scale X: %.3f", _scale.x());
    _ui->lblScaleX->setText(buffer);

    draw();
}

void MainWindow::on_sldScaleY_valueChanged(int value)
{
    _scale.setY(value / 1000.0f);

    char buffer[128];
    sprintf(buffer, "Scale Y: %.3f", _scale.y());
    _ui->lblScaleY->setText(buffer);

    draw();
}

void MainWindow::on_sldScaleZ_valueChanged(int value)
{
    _scale.setZ(value / 1000.0f);

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

void MainWindow::on_btnReset_clicked()
{
    _pos.setX(_canvas->getWidth() / 2.0f - _cube.getSize() / 2.0f);
    _pos.setY(_canvas->getHeight() / 2.0f - _cube.getSize() / 2.0f);
    _pos.setZ(10.0f);

    _scale.setX(1.0f);
    _scale.setY(1.0f);
    _scale.setZ(1.0f);

    _rotation.setX(0.0f);
    _rotation.setY(0.0f);
    _rotation.setZ(0.0f);

    _shearing.setX(0.0f);
    _shearing.setY(0.0f);
    _shearing.setZ(0.0f);

    _d = 1000.0f;

    _ui->sldD->setValue(_d);

    _cube.reset();

    _ui->sldX->setValue(_pos.x());
    _ui->sldY->setValue(_pos.y());
    _ui->sldZ->setValue(_pos.z());

    _ui->sldScaleX->setValue(_scale.x() * 1000.0f);
    _ui->sldScaleY->setValue(_scale.y() * 1000.0f);
    _ui->sldScaleZ->setValue(_scale.z() * 1000.0f);

    _ui->sldRotateX->setValue(_rotation.x());
    _ui->sldRotateY->setValue(_rotation.y());
    _ui->sldRotateZ->setValue(_rotation.z());

    _ui->sldShearingX->setValue(_shearing.x());
    _ui->sldShearingY->setValue(_shearing.y());
    _ui->sldShearingZ->setValue(_shearing.z());

    on_cbHideFaces_toggled(false);
    on_cbShowTexture_toggled(false);
    on_cbLight_toggled(false);
}

void MainWindow::on_sldD_valueChanged(int value)
{
    _d = value;

    char buffer[128];
    sprintf(buffer, "D: %.3f", _d);
    _ui->lblD->setText(buffer);

    draw();
}

void MainWindow::on_cbHideFaces_toggled(bool checked)
{
    _ui->cbShowTexture->setEnabled(checked);
    _ui->cbLight->setEnabled(checked);
    draw();
}

void MainWindow::on_cbShowTexture_toggled(bool checked)
{
    _ui->cbLight->setEnabled(checked);
    draw();
}

void MainWindow::on_cbLight_toggled(bool checked)
{
    (void)checked;
    draw();
}
