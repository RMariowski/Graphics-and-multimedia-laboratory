#ifndef CUBE_H
#define CUBE_H

#include <QVector4D>

class Cube
{
public:
    Cube();

    void reset();

    QVector4D* getPF(int i) {
        return &_pf[i];
    }

    QVector4D* getPB(int i) {
        return &_pb[i];
    }

    float getSize() const {
        return _size;
    }

private:
    QVector4D _pf[4];
    QVector4D _pb[4];
    float _size;
};

#endif // CUBE_H
