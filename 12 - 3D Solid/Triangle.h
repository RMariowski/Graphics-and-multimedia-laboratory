#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <QVector2D>
#include <QList>
#include <QImage>

class Triangle
{
public:
    Triangle();

    Triangle(QVector2D v1, QVector2D v2, QVector2D v3);

    QVector2D getVertex(int i) const {
        return _vertices[i];
    }

    void setVertex(int i, const QVector2D& v) {
        _vertices[i] = v;
    }

private:
    QVector2D _vertices[3];
};

#endif // TRIANGLE_H
