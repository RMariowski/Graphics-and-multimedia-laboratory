#include "Triangle.h"
#include <QColor>

Triangle::Triangle()
{

}

Triangle::Triangle(QVector2D v1, QVector2D v2, QVector2D v3)
{
    _vertices[0] = v1;
    _vertices[1] = v2;
    _vertices[2] = v3;
}
