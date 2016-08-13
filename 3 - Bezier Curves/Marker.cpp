#include "Marker.h"
#include "PixelPainter.h"

Marker::Marker(QPoint* point, int size)
    : _point(point), _size(size)
{
    int h = static_cast<int>(size / 2.0);
    _pos.setX(_point->x() - h);
    _pos.setY(_point->y() - h);
}

void Marker::moveTo(int x, int y)
{
    int h = static_cast<int>(_size / 2.0);

    _pos.setX(x - h);
    _pos.setY(y - h);
    _point->setX(x);
    _point->setY(y);
}

bool Marker::isIntersect(int x, int y)
{
    return (x >= _pos.x() && y >= _pos.y() &&
            x <= _pos.x() + _size && y <= _pos.y() + _size);
}
