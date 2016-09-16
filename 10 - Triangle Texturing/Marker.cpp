#include "Marker.h"

Marker::Marker(QPoint pos, int size)
    : _pos(pos), _size(size)
{
    int h = static_cast<int>(size / 2.0);
    _pos.setX(_pos.x() - h);
    _pos.setY(_pos.y() - h);
}

void Marker::moveTo(int x, int y)
{
    int h = static_cast<int>(_size / 2.0);

    _pos.setX(x - h);
    _pos.setY(y - h);
}

bool Marker::isIntersect(int x, int y)
{
    return (x >= _pos.x() && y >= _pos.y() &&
            x <= _pos.x() + _size && y <= _pos.y() + _size);
}
