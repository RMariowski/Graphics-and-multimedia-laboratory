#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                        _ui->frmCanvas->width(), _ui->frmCanvas->height(), false);

    _canvas2 = new Canvas(this, _ui->frmCanvas2->x(), _ui->frmCanvas2->y(),
                        _ui->frmCanvas2->width(), _ui->frmCanvas2->height(), false);

    _canvas3 = new Canvas(this, _ui->frmCanvas3->x(), _ui->frmCanvas3->y(),
                        _ui->frmCanvas3->width(), _ui->frmCanvas3->height(), true);

    _image = new QImage(":/1.jpg");
    _image2 = new QImage(":/2.jpg");

    _activeCanvas = 0;
    _selectedMarker = 0; 

    resetGrid();
    drawAllForCanvas();
    drawAllForCanvas2();
    drawAllForCanvas3();
}

MainWindow::~MainWindow()
{
    delete _image;
    delete _image2;
    delete _canvas3;
    delete _canvas2;
    delete _canvas;
    delete _ui;
}

int MainWindow::getFramesCount() const {
    return _ui->sldMorph->maximum();
}

void MainWindow::resetGrid()
{
    int markerHalfSize = 5;
    int markerSize = markerHalfSize * 2;

    int canvasWidth = _canvas->getWidth() + markerHalfSize;
    int canvasHeight = _canvas->getHeight() + markerHalfSize;
    int canvas2Width = _canvas2->getWidth() + markerHalfSize;
    int canvas2Height = _canvas2->getHeight() + markerHalfSize;

    _markers.clear();
    _markers.push_back(Marker(QPoint(markerHalfSize, markerHalfSize), markerSize));
    _markers.push_back(Marker(QPoint(canvasWidth / 2.0, markerHalfSize), markerSize));
    _markers.push_back(Marker(QPoint(canvasWidth, markerHalfSize), markerSize));

    _markers.push_back(Marker(QPoint(markerHalfSize, canvasHeight / 2.0), markerSize));
    _markers.push_back(Marker(QPoint(canvasWidth, canvasHeight / 2.0), markerSize));

    _markers.push_back(Marker(QPoint(markerHalfSize, canvasHeight), markerSize));
    _markers.push_back(Marker(QPoint(canvasWidth / 2.0, canvasHeight), markerSize));
    _markers.push_back(Marker(QPoint(canvasWidth, canvasHeight), markerSize));

    _markers.push_back(Marker(QPoint(canvasWidth / 2.0 + markerHalfSize, canvasHeight / 2.0 + markerHalfSize), markerSize));


    _markers2.clear();
    _markers2.push_back(Marker(QPoint(markerHalfSize, markerHalfSize), markerSize));
    _markers2.push_back(Marker(QPoint(canvas2Width / 2.0, markerHalfSize), markerSize));
    _markers2.push_back(Marker(QPoint(canvas2Width, markerHalfSize), markerSize));

    _markers2.push_back(Marker(QPoint(markerHalfSize, canvas2Height / 2.0), markerSize));
    _markers2.push_back(Marker(QPoint(canvas2Width, canvas2Height / 2.0), markerSize));

    _markers2.push_back(Marker(QPoint(markerHalfSize, canvas2Height), markerSize));
    _markers2.push_back(Marker(QPoint(canvas2Width / 2.0, canvas2Height), markerSize));
    _markers2.push_back(Marker(QPoint(canvas2Width, canvas2Height), markerSize));

    _markers2.push_back(Marker(QPoint(canvas2Width / 2.0 + markerHalfSize, canvas2Height / 2.0 + markerHalfSize), markerSize));
}

void MainWindow::drawAllForCanvas()
{
    int markersCount = _markers.size();

    _canvas->reset();
    _canvas->drawImage(_image);

    if (!_ui->chkVisibility->isChecked())
    {
        for (int i = 0; i < markersCount; i++)
            _canvas->drawMarker(&_markers[i]);
    }
}

void MainWindow::drawAllForCanvas2()
{
    int markers2Count = _markers2.size();

    _canvas2->reset();
    _canvas2->drawImage(_image2);

    if (!_ui->chkVisibility->isChecked())
    {
        for (int i = 0; i < markers2Count; i++)
            _canvas2->drawMarker(&_markers2[i]);
    }
}

void MainWindow::drawAllForCanvas3()
{
    int markersCount = _markers.size();
    int markers2Count = _markers2.size();

    _canvas3->reset();
    _canvas3->drawImage(_image);

    int framesCount = getFramesCount();
    float animPart = _ui->sldMorph->value() / static_cast<float>(framesCount);

    if (markersCount == 3 && markers2Count == 3)
    {
        _canvas3->morphing(Triangle(_image, _markers), Triangle(_image2, _markers2), animPart);
    }
    else if (markersCount > 3 && markers2Count > 3)
    {
        _canvas3->morphingGrid(_image, _image2, _markers, _markers2, animPart);
    }

    repaint();
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
        mouseX = 0;

    if (mouseY - markerHalf < 0)
        mouseY = 0;

    if (mouseX + markerHalf > _activeCanvas->getWidth())
        mouseX = _activeCanvas->getWidth() - 1;

    if (mouseY + markerHalf > _activeCanvas->getHeight())
        mouseY = _activeCanvas->getHeight() - 1;

    _selectedMarker->moveTo(mouseX, mouseY);

    if (_activeCanvas == _canvas) drawAllForCanvas();
    else if (_activeCanvas == _canvas2) drawAllForCanvas2();
    drawAllForCanvas3();

    repaint();
}

void MainWindow::mouseReleaseEvent(QMouseEvent*)
{
    _activeCanvas = 0;
    _selectedMarker = 0;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _canvas->draw();
    _canvas2->draw();
    _canvas3->draw();
}

void MainWindow::on_chkVisibility_toggled(bool)
{
    drawAllForCanvas();
    drawAllForCanvas2();
    repaint();
}

void MainWindow::on_sldMorph_valueChanged(int)
{
    drawAllForCanvas3();
    repaint();
}

void MainWindow::on_btnPlay_clicked()
{
    _ui->sldMorph->setValue(0);
    play();
}

void MainWindow::play()
{
    int nextFrame = _ui->sldMorph->value() + 1;
    if (nextFrame <= getFramesCount()) {
        _ui->sldMorph->setValue(nextFrame );
    } else return;
    QTimer::singleShot(1, this, SLOT(play()));
}
