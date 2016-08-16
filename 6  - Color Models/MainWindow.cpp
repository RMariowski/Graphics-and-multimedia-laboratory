#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QPainter>
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),_ui(new Ui::MainWindow)
{
    _ui->setupUi(this);
    _imgBounds.setX(_ui->frmCanvas->x());
    _imgBounds.setY(_ui->frmCanvas->y());
    _imgBounds.setWidth(_ui->frmCanvas->width());
    _imgBounds.setHeight(_ui->frmCanvas->height());

    _image = new QImage(_imgBounds.width(), _imgBounds.height(), QImage::Format_RGB32);
    _imgPixels = _image->bits();

    _h = 360;
    _s = 100;
    _v = 100;
}

MainWindow::~MainWindow()
{
    delete _image;
    delete _ui;
}

void MainWindow::drawPixel(int x, int y, int r, int g, int b)
{
    if (r > 255)
        r = 255;

    if (r < 0)
        r = 0;

    if (g > 255)
        g = 255;

    if (g < 0)
        g = 0;

    if (b > 255)
        b = 255;

    if (b < 0)
        b = 0;

    int width = _imgBounds.width();
    _imgPixels[width * 4 * y + 4 * x] = b;
    _imgPixels[width * 4 * y + 4 * x + 1] = g;
    _imgPixels[width * 4 * y + 4 * x + 2] = r;
}

QColor MainWindow::HSVtoRGB(double h, double s, double v)
{
    if (v == 0)
        return QColor(0, 0, 0);

    if (h < 0.0)    h = 0.0;
    if (h > 359.0)  h = 359.0;
    if (s < 0.0)    s = 0.0;
    if (s > 1.0)    s = 1.0;
    if (v < 0.0)    v = 0.0;
    if (v > 1.0)    v = 1.0;

    h /= 60.0;
    int i = floor(h);
    double f = h - i;
    double p = v * (1.0 - s);
    double q = v * (1.0 - (s * f));
    double t = v * (1.0 - (s * (1.0 - f)));

    float r;
    float g;
    float b;

    if      (i == 0) { r = v; g = t; b = p; }
    else if (i == 1) { r = q; g = v; b = p; }
    else if (i == 2) { r = p; g = v; b = t; }
    else if (i == 3) { r = p; g = q; b = v; }
    else if (i == 4) { r = t; g = p; b = v; }
    else if (i == 5) { r = v; g = p; b = q; }

    return QColor(static_cast<int>(r * 255),
                  static_cast<int>(g * 255),
                  static_cast<int>(b * 255));
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.drawImage(_imgBounds.x(), _imgBounds.y(), *_image);
}

void MainWindow::on_sldR_valueChanged(int value)
{
    int r = value;
    int g = 0;
    int b = 0;

    for (int y = 0; y < _imgBounds.height(); y += 2) {
        g = 0;
        for (int x = 0; x < _imgBounds.width(); x += 2) {
            drawPixel(x, y, r, g, b);
            drawPixel(x + 1, y, r, g, b);
            drawPixel(x, y + 1, r, g, b);
            drawPixel(x + 1, y + 1, r, g, b);
            g++;
        }
        b++;
    }

    update();
}

void MainWindow::on_sldG_valueChanged(int value)
{
    int r = 0;
    int g = value;
    int b = 0;

    for (int y = 0; y < _imgBounds.height(); y += 2) {
        r = 0;
        for (int x = 0; x < _imgBounds.width(); x += 2) {
            drawPixel(x, y, r, g, b);
            drawPixel(x + 1, y, r, g, b);
            drawPixel(x, y + 1, r, g, b);
            drawPixel(x + 1, y + 1, r, g, b);
            r++;
        }
        b++;
    }

    update();
}

void MainWindow::on_sldB_valueChanged(int value)
{
    int r = 0;
    int g = 0;
    int b = value;

    for (int y = 0; y < _imgBounds.height(); y += 2) {
        r = 0;
        for (int x = 0; x < _imgBounds.width(); x += 2) {
            drawPixel(x, y, r, g, b);
            drawPixel(x + 1, y, r, g, b);
            drawPixel(x, y + 1, r, g, b);
            drawPixel(x + 1, y + 1, r, g, b);
            r++;
        }
        g++;
    }

    update();
}

void MainWindow::on_sldH_valueChanged(int value)
{
    double h = value;
    double s = 1.0;
    double v = 1.0;

    for (int y = 0; y < _imgBounds.height(); y += 5) {
        v = 1.0;
        for (int x = 0; x < _imgBounds.width(); x++) {
            QColor c = HSVtoRGB(h, s, v);
            for (int i = 0; i <= 5; i++)
                drawPixel(x, y + i, c.red(), c.green(), c.blue());

            v -= 0.0005;
            if (v < 0.0) v = 0.0;
        }

        s -= 0.008;
        if (s < 0.0) s = 0.0;
    }

    update();
}

void MainWindow::on_sldS_valueChanged(int value)
{
    double h = 0.0;
    double s = value / 10000.0;
    double v = 1.0;

    for (int y = 0; y < _imgBounds.height(); y += 5) {
        h = 0.0;
        for (int x = 0; x < _imgBounds.width(); x++) {
            QColor c = HSVtoRGB(h, s, v);
            for (int i = 0; i <= 5; i++)
                drawPixel(x, y + i, c.red(), c.green(), c.blue());

            h += 0.5;
            if (h > 359.0) h = 359.0;
        }

        v -= 0.008;
        if (v < 0.0) v = 0.0;
    }

    update();
}

void MainWindow::on_sldV_valueChanged(int value)
{
    double h = 0.0;
    double s = 1.0;
    double v = value / 10000.0;

    for (int y = 0; y < _imgBounds.height(); y += 5) {
        h = 0.0;
        for (int x = 0; x < _imgBounds.width(); x++) {
            QColor c = HSVtoRGB(h, s, v);
            for (int i = 0; i <= 5; i++)
                drawPixel(x, y + i, c.red(), c.green(), c.blue());

            h += 0.5;
            if (h > 359.0) h = 359.0;
        }

        s -= 0.008;
        if (s < 0.0) s = 0.0;
    }

    update();
}
