#include "Triangle.h"
#include <QColor>

Triangle::Triangle()
    : _image(0)
{
}

Triangle::Triangle(QImage* image, const QList<Marker>& markers)
    : _image(image)
{
    for (int i = 0; i < 3; i++)
        _vertices[i] = QVector2D(markers[i].getX(), markers[i].getY());
}

Triangle::Triangle(QVector2D v1, QVector2D v2, QVector2D v3)
    : _image(0)
{
    _vertices[0] = v1;
    _vertices[1] = v2;
    _vertices[2] = v3;
}

void Triangle::createImage(int width, int height)
{
    _image = new QImage(width, height, QImage::Format_ARGB32);
    resetImage();
}

void Triangle::resetImage()
{
    QRgb whiteColor = QColor(255, 0, 0, 0).rgba();
    for (int y = 0; y < _image->height(); y++)
    {
        for (int x = 0; x < _image->width(); x++)
            _image->setPixel(QPoint(x, y), whiteColor);
    }
}

void Triangle::deleteImage()
{
    delete _image;
}
