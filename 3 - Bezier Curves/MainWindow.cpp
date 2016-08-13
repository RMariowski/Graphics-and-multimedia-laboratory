#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <cmath>
#include <vector>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                         _ui->frmCanvas->width(), _ui->frmCanvas->height());
    _pixelPainter = new PixelPainter(_canvas);

    on_btnClear_clicked();
}

MainWindow::~MainWindow()
{
    delete _canvas;
    delete _pixelPainter;
    delete _ui;
}

void MainWindow::addMarker(QPoint* point, int size)
{
    Marker marker(point, size);
    _markers.push_back(marker);
    _pixelPainter->drawMarker(marker);
}

double MainWindow::getDistanceBetweenPoints(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void MainWindow::on_btnClear_clicked()
{
    _pixelPainter->clear();

    _markers.clear();
    _clicks.clear();
    _selectedMarker = 0;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _pixelPainter->getCanvas()->draw();
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QPoint click(event->x() - _canvas->getX(),
                 event->y() - _canvas->getY());

    for (int i = 0; i < _markers.size(); i++)
    {
        if (_markers[i].isIntersect(click.x(), click.y()))
        {
            _selectedMarker = &_markers[i];
            return;
        }
    }

    if (_pixelPainter->getCanvas()->isPointOn(event->x(), event->y()))
        _clicks.push_back(click);
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (_selectedMarker == 0)
        return;

    QRect canvasBounds = _canvas->getBounds();

    int markerHalf = static_cast<int>(_selectedMarker->getSize() / 2.0);
    int mouseX = event->x() - canvasBounds.x();
    int mouseY = event->y() - canvasBounds.y();

    if (mouseX - markerHalf < 0)
        mouseX = markerHalf;

    if (mouseY - markerHalf < 0)
        mouseY = markerHalf;

    if (mouseX + markerHalf > canvasBounds.width())
        mouseX = canvasBounds.width() - markerHalf - 1;

    if (mouseY + markerHalf > canvasBounds.height())
        mouseY = canvasBounds.height() - markerHalf - 1;

    _selectedMarker->moveTo(mouseX, mouseY);
    _pixelPainter->clear();
    _pixelPainter->drawCurve(_clicks);

    for (int i = 0; i < _markers.size(); i++)
        _pixelPainter->drawMarker(_markers[i]);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (_clicks.size() < 1)
        return;

    if (_selectedMarker != 0)
    {
        _selectedMarker = 0;
        return;
    }

    addMarker(&_clicks.last(), 10);
    _pixelPainter->drawCurve(_clicks);
}
