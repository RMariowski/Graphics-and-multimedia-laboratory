#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                        _ui->frmCanvas->width(), _ui->frmCanvas->height());

    _canvas2 = new Canvas(this, _ui->frmCanvas2->x(), _ui->frmCanvas2->y(),
                        _ui->frmCanvas2->width(), _ui->frmCanvas2->height());

    _image = new QImage(":/image.jpg");

    _canvas->drawImage(_image);

    _activeCanvas = 0;
    _selectedMarker = 0;
}

MainWindow::~MainWindow()
{
    delete _image;
    delete _canvas2;
    delete _canvas;
    delete _ui;
}

void MainWindow::drawAllForCanvas()
{
    int markersCount = _markers.size();

    _canvas->reset();
    _canvas->drawImage(_image);

    if (!_ui->chkVisibility->isChecked()) {
        for (int i = 0; i < markersCount; i++)
            _canvas->drawMarker(&_markers[i]);
    }
}

void MainWindow::drawAllForCanvas2()
{
    int markersCount = _markers.size();
    int markers2Count = _markers2.size();

    _canvas2->reset();

    if (!_ui->chkVisibility->isChecked()) {
        for (int i = 0; i < markers2Count; i++)
            _canvas2->drawMarker(&_markers2[i]);
    }

    if (markersCount == 3 && markers2Count == 3)
        _canvas2->drawTexture(_image, _markers, _markers2);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (_canvas->isIntersect(event->x(), event->y()))
    {
        _activeCanvas = _canvas;

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

        if (event->button() == Qt::LeftButton && _markers.size() < 3)
        {
            Marker marker(click, 10);
            _markers.push_back(marker);
            _canvas->drawMarker(&marker);

            if (_markers.size() == 3 && _markers2.size() == 3)
                _canvas2->drawTexture(_image, _markers, _markers2);

            update();
        }
    }
    else if (_canvas2->isIntersect(event->x(), event->y()))
    {
        _activeCanvas = _canvas2;

        QPoint click(event->x() - _canvas2->getX(),
                     event->y() - _canvas2->getY());

        for (int i = 0; i < _markers2.size(); i++)
        {
            if (_markers2[i].isIntersect(click.x(), click.y()))
            {
                _selectedMarker = &_markers2[i];
                return;
            }
        }

        if (event->button() == Qt::LeftButton && _markers2.size() < 3)
        {
            Marker marker(click, 10);
            _markers2.push_back(marker);
            _canvas2->drawMarker(&marker);

            if (_markers.size() == 3 && _markers2.size() == 3)
                _canvas2->drawTexture(_image, _markers, _markers2);

            update();
        }
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    if (_activeCanvas == 0 || _selectedMarker == 0)
        return;

    int markerHalf = static_cast<int>(_selectedMarker->getSize() / 2.0);
    int mouseX = event->x() - _activeCanvas->getX();
    int mouseY = event->y() - _activeCanvas->getY();

    if (mouseX - markerHalf < 0)
        mouseX = markerHalf;

    if (mouseY - markerHalf < 0)
        mouseY = markerHalf;

    if (mouseX + markerHalf > _activeCanvas->getWidth())
        mouseX = _activeCanvas->getWidth() - markerHalf - 1;

    if (mouseY + markerHalf > _activeCanvas->getHeight())
        mouseY = _activeCanvas->getHeight() - markerHalf - 1;

    _selectedMarker->moveTo(mouseX, mouseY);

    drawAllForCanvas();
    drawAllForCanvas2();

    update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    (void)event;

    _activeCanvas = 0;
    _selectedMarker = 0;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _canvas->draw();
    _canvas2->draw();
}

void MainWindow::on_chkVisibility_toggled(bool checked)
{
    (void)checked;

    drawAllForCanvas();
    drawAllForCanvas2();
    update();
}
