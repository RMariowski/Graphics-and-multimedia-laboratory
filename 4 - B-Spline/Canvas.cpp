#include "Canvas.h"
#include "MainWindow.h"

Canvas::Canvas(MainWindow* mainWindow, int x, int y, int width, int height)
    : _mainWindow(mainWindow), _imgBounds(x, y, width, height)
{
    _image = new QImage(width, height, QImage::Format_RGB32);
    _imgPixels = _image->bits();
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
    painter.drawImage(_imgBounds.x(), _imgBounds.y(), *_image);
}

bool Canvas::isPointOn(int x, int y)
{
    return (x >= _imgBounds.x() && y >= _imgBounds.y() &&
            x <= _imgBounds.x() + _imgBounds.width() &&
            y <= _imgBounds.y() + _imgBounds.height());
}
