#include "Canvas.h"
#include "MainWindow.h"
#include <cmath>

Canvas::Canvas(MainWindow* mainWindow, int x, int y, int width, int height)
    : _mainWindow(mainWindow), _x(x), _y(y)
{
    _image = new QImage(width, height, QImage::Format_ARGB32);

    reset();
}

Canvas::~Canvas()
{
    delete _image;
}

void Canvas::draw()
{
    QPainter painter(_mainWindow);
    painter.drawImage(_x, _y, *_image);
}

void Canvas::drawPixel(int x, int y, QRgb color)
{
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
        return;
    _image->setPixel(QPoint(x, y), color);
}

void Canvas::drawImage(QImage* image)
{
    for (int y = 0; y < image->height(); y++)
    {
        for (int x = 0; x < image->width(); x++)
        {
            QPoint p(x, y);
            _image->setPixel(p, image->pixel(p));
        }
    }
}

void Canvas::drawMarker(Marker* marker)
{
    QRgb color = QColor(255, 0, 255).rgb();
    for (int y = 0; y <  marker->getSize(); y++)
    {
        for (int x = 0; x <  marker->getSize(); x++)
        {
            QPoint p(x + marker->getX(), y + marker->getY());
            _image->setPixel(p, color);
        }
    }
}

void Canvas::drawTexture(QImage* texture, const QList<Marker>& markers, const QList<Marker>& markers2)
{
    QPoint minPos = markers2[0].getPos();
    QPoint maxPos = markers2[0].getPos();

    for (int i = 1; i < markers2.size(); i++)
    {
        QPoint markerPos = markers2[i].getPos();

        if (minPos.x() > markerPos.x())
            minPos.setX(markerPos.x());

        if (minPos.y() > markerPos.y())
            minPos.setY(markerPos.y());

        if (maxPos.x() < markerPos.x())
            maxPos.setX(markerPos.x());

        if (maxPos.y() < markerPos.y())
            maxPos.setY(markerPos.y());
    }

    int width = maxPos.x() - minPos.x();
    int height = maxPos.y() - minPos.y();
    double markerHalfSize = markers[0].getSize() / 2.0;

    double xa = markers2[0].getPos().x();
    double ya = markers2[0].getPos().y();
    double xb = markers2[1].getPos().x();
    double yb = markers2[1].getPos().y();
    double xc = markers2[2].getPos().x();
    double yc = markers2[2].getPos().y();

    double xta = markers[0].getPos().x();
    double yta = markers[0].getPos().y();
    double xtb = markers[1].getPos().x();
    double ytb = markers[1].getPos().y();
    double xtc = markers[2].getPos().x();
    double ytc = markers[2].getPos().y();

    for (int y = minPos.y(); y < minPos.y() + height; y++)
    {
        for (int x = minPos.x(); x < minPos.x() + width; x++)
        {
            double m = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);
            double v = ((x - xa) * (yc - ya) - (xc - xa) * (y - ya)) / m;
            double w = ((xb - xa) * (y - ya) - (x - xa) * (yb - ya)) / m;
            double u = 1.0 - v - w;

            if (u < 0 || v < 0 || w < 0 || u > 1 || v > 1 || w > 1)
                continue;

            double x2 = u * xa + v * xb + w * xc;
            double y2 = u * ya + v * yb + w * yc;
            double xt = u * xta + v * xtb + w * xtc;
            double yt = u * yta + v * ytb + w * ytc;

            QRgb color = texture->pixel(QPoint(xt, yt));
            drawPixel(round(x2 + markerHalfSize), round(y2 + markerHalfSize), color);
        }
    }
}

void Canvas::reset()
{
    QRgb whiteColor = QColor(255, 255, 255).rgb();
    for (int y = 0; y < getHeight(); y++)
    {
        for (int x = 0; x < getWidth(); x++)
            _image->setPixel(QPoint(x, y), whiteColor);
    }
}

bool Canvas::isIntersect(int x, int y)
{
    return (x >= _x && y >= _y &&
            x <= _x + _image->width() &&
            y <= _y + _image->height());
}

