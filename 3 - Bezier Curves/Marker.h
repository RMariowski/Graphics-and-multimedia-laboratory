#ifndef MARKER_H
#define MARKER_H

#include <QPoint>

class Marker
{
public:
    Marker(QPoint* point, int size);

    void moveTo(int x, int y);

    bool isIntersect(int x, int y);

    QPoint* getPoint() const {
        return _point;
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
    QPoint* _point;
    QPoint _pos;
    int _size;
};

#endif // MARKER_H
