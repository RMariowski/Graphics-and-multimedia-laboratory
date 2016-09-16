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
    _pos.setX(x);
    _pos.setY(y);
}

bool Marker::isIntersect(int x, int y)
{
    return (x >= getDrawX() && y >= getDrawY() &&
            x <= getDrawX() + _size && y <= getDrawY() + _size);
}
