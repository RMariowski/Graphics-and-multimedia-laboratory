#include "Cube.h"

Cube::Cube()
    : _size(100.0f)
{

}

void Cube::reset()
{
    _pf[0] = QVector4D(0.0f, 0.0f, 0.0f, 1.0f);
    _pf[1] = QVector4D(_size, 0.0f, 0.0f, 1.0f);
    _pf[2] = QVector4D(_size, _size, 0.0f, 1.0f);
    _pf[3] = QVector4D(0.0f, _size, 0.0f, 1.0f);

    _pb[0] = QVector4D(0.0f, 0.0f, _size, 1.0f);
    _pb[1] = QVector4D(_size, 0.0f, _size, 1.0f);
    _pb[2] = QVector4D(_size, _size, _size, 1.0f);
    _pb[3] = QVector4D(0.0f, _size, _size, 1.0f);
}
