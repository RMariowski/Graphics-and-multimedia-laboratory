#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Marker.h"
#include <QVector2D>
#include <QList>
#include <QImage>

class Triangle
{
public:
    Triangle();

    Triangle(QImage* image, const QList<Marker>& markers);

    Triangle(QVector2D v1, QVector2D v2, QVector2D v3);

    void createImage(int width, int height);

    void resetImage();

    void deleteImage();

    QImage* getImage() const {
        return _image;
    }

    QVector2D getVertex(int i) const {
        return _vertices[i];
    }

    void setVertex(int i, const QVector2D& v) {
        _vertices[i] = v;
    }

    void setImage(QImage* image) {
        _image = image;
    }

private:
    QImage* _image;
    QVector2D _vertices[3];
};

#endif // TRIANGLE_H
