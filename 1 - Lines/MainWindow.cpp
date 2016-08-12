#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _imgBounds.setX(_ui->frmCanvas->x());
    _imgBounds.setY(_ui->frmCanvas->y());
    _imgBounds.setWidth(_ui->frmCanvas->width());
    _imgBounds.setHeight(_ui->frmCanvas->height());

    _img = new QImage(_imgBounds.width(), _imgBounds.height(), QImage::Format_RGB32);
    _imgPixels = _img->bits();

    _pressPos = QPoint(-1, -1);

    on_btnClear_clicked();
}

MainWindow::~MainWindow()
{
    delete _img;
    delete _ui;
}

void MainWindow::drawPixel(int x, int y, int r, int g, int b)
{
    int width = _imgBounds.width();
    _imgPixels[width * 4 * y + 4 * x] = b;
    _imgPixels[width * 4 * y + 4 * x + 1] = g;
    _imgPixels[width * 4 * y + 4 * x + 2] = r;
}

void MainWindow::drawLine(int x1, int y1, int x2, int y2)
{
    double a = (y1 - static_cast<double>(y2)) / (x1 - static_cast<double>(x2));
    double b = y1 - a * x1;

    if (x1 > x2)
        std::swap(x1, x2);

    if (y1 > y2)
        std::swap(y1, y2);

    int x = x1;
    int y = y1;

    if (x1 == x2)
    {
        for (; y <= y2; y++)
            drawPixel(x1, y, 0, 0, 0);
    }
    else
    {
        for (; x <= x2; x++)
        {
            int y = round(a * x + b);
            drawPixel(x, y, 0, 0, 0);
        }

        for (; y <= y2; y++)
        {
            int x = round((y - b) / a);
            drawPixel(x, y, 0, 0, 0);
        }
    }

    update();
}

void MainWindow::on_btnClear_clicked()
{
    for (int y = 0; y < _imgBounds.height(); y++)
    {
        for (int x = 0; x < _imgBounds.width(); x++)
            drawPixel(x, y, 255, 255, 255);
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(_imgBounds.x(), _imgBounds.y(), *_img);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    _pressPos.setX(event->x() - _imgBounds.x());
    _pressPos.setY(event->y() - _imgBounds.y());

    if (_pressPos.x() < 0 || _pressPos.y() < 0 || _pressPos.x() >= _imgBounds.width() || _pressPos.y() >= _imgBounds.height())
        _pressPos = QPoint(-1, -1);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (_pressPos.x() < 0 || _pressPos.y() < 0)
        return;

    int x = event->x() - _imgBounds.x();
    int y = event->y() - _imgBounds.y();

    if (x >= 0 && y >= 0)
    {
        if (x < _imgBounds.width() && y < _imgBounds.height())
            drawLine(_pressPos.x(), _pressPos.y(), x, y);
    }
}
