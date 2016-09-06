#include "Canvas.h"
#include "MainWindow.h"
#include <QPainter>

Canvas::Canvas(MainWindow* mainWindow, const QPoint& pos, const QSize& size)
    : _mainWindow(mainWindow), _pos(pos), _size(size)
{
    _image = new QImage(size.width(), size.height(), QImage::Format_RGB32);
}

Canvas::~Canvas()
{
    delete _image;
}

void Canvas::update()
{
    _mainWindow->update();
}

void Canvas::draw()
{
    QPainter painter(_mainWindow);
    painter.drawImage(_pos.x(), _pos.y(), *_image);
}
