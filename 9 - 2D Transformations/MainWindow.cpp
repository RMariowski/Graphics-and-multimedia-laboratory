#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                        _ui->frmCanvas->width(), _ui->frmCanvas->height());

    _image = new QImage(":/image.jpg");
    on_btnReset_clicked();
}

MainWindow::~MainWindow()
{
    delete _image;
    delete _canvas;
    delete _ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _canvas->draw();
}

void MainWindow::drawImage()
{
    _canvas->reset();
    _canvas->drawImage(_image, _position, _scaleX, _scaleY, _rotation, _shearingX, _shearingY);
    update();
}

void MainWindow::on_sldX_valueChanged(int value)
{
    _position.setX(value);

    char buffer[128];
    sprintf(buffer, "X: %d", value);
    _ui->lblX->setText(buffer);

    drawImage();
}

void MainWindow::on_sldY_valueChanged(int value)
{
    _position.setY(value);

    char buffer[128];
    sprintf(buffer, "Y: %d", value);
    _ui->lblY->setText(buffer);

    drawImage();
}

void MainWindow::on_sldScaleX_valueChanged(int value)
{
    _scaleX = value / 1000.0;

    char buffer[128];
    sprintf(buffer, "Scale X: %f", _scaleX);
    _ui->lblScaleX->setText(buffer);

    drawImage();
}

void MainWindow::on_sldScaleY_valueChanged(int value)
{
    _scaleY = value / 1000.0;

    char buffer[128];
    sprintf(buffer, "Scale Y: %f", _scaleY);
    _ui->lblScaleY->setText(buffer);

    drawImage();
}

void MainWindow::on_sldRotate_valueChanged(int value)
{
    _rotation = value / 180.0 * M_PI;
    _rotation = static_cast<int>(_rotation * 100.0);
    _rotation /= 100;

    char buffer[128];
    sprintf(buffer, "Rotation: %f", _rotation);
    _ui->lblRotation->setText(buffer);

    drawImage();
}

void MainWindow::on_sldShearingX_valueChanged(int value)
{
    _shearingX = static_cast<int>(value / 100.0) / 10.0;

    char buffer[128];
    sprintf(buffer, "Shearing X: %f", _shearingX);
    _ui->lblShearingX->setText(buffer);

    drawImage();
}

void MainWindow::on_sldShearingY_valueChanged(int value)
{
    _shearingY = static_cast<int>(value / 100.0) / 10.0;

    char buffer[128];
    sprintf(buffer, "Shearing Y: %f", _shearingY);
    _ui->lblShearingY->setText(buffer);

    drawImage();
}

void MainWindow::on_btnReset_clicked()
{
    _position.setX(_canvas->getWidth() / 2.0 - _image->width() / 2.0);
    _position.setY(_canvas->getHeight() / 2.0 - _image->height() / 2.0);
    _scaleX = 1.0;
    _scaleY = 1.0;
    _rotation = 0.0;
    _shearingX = 0.0;
    _shearingY = 0.0;

    _ui->sldX->setValue(_position.x());
    _ui->sldY->setValue(_position.y());
    _ui->sldScaleX->setValue(_scaleX * 1000);
    _ui->sldScaleY->setValue(_scaleY * 1000);
    _ui->sldRotate->setValue(_rotation);
    _ui->sldShearingX->setValue(_shearingX);
    _ui->sldShearingY->setValue(_shearingY);
}
