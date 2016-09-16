#ifndef MARKER_H
#define MARKER_H

#include <QPoint>

class Marker
{
public:
    Marker(QPoint pos, int size);

    void moveTo(int x, int y);

    bool isIntersect(int x, int y);

    QPoint getPos() const {
        return _pos;
    }

    int getX() const {
        return _pos.x();
    }

    int getY() const {
        return _pos.y();
    }

    int getSize() const {
        return _size;
    }

private:
    QPoint _pos;
    int _size;
};

#endif // MARKER_H
