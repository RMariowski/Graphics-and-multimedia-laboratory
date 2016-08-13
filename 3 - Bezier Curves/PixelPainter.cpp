#include "PixelPainter.h"

#include <cmath>
#include <vector>
#define PI 3.14159265358979323846

PixelPainter::PixelPainter(Canvas* canvas)
    : _canvas(canvas)
{

}

void PixelPainter::drawPixel(int x, int y, int r, int g, int b)
{
    unsigned char* pixels = _canvas->getPixels();

    int canvasWidth = _canvas->getWidth();
    pixels[canvasWidth * 4 * y + 4 * x] = b;
    pixels[canvasWidth * 4 * y + 4 * x + 1] = g;
    pixels[canvasWidth * 4 * y + 4 * x + 2] = r;
}

void PixelPainter::drawLine(int startX, int startY, int endX, int endY)
{
    double a = (startY - static_cast<double>(endY)) / (startX - static_cast<double>(endX));
    double b = startY - a * startX;

    if (startX > endX)
        std::swap(startX, endX);

    if (startY > endY)
        std::swap(startY, endY);

    int x = startX;
    int y = startY;

    if (startX == endX)
    {
        for (; y <= endY; y++)
            drawPixel(startX, y, 0, 0, 0);
    }
    else if (startY == endY)
    {
        for (; x <= endX; x++)
            drawPixel(x, startY);
    }
    else
    {
        for (; x <= endX; x++)
        {
            int y = round(a * x + b);
            drawPixel(x, y, 0, 0, 0);
        }

        for (; y <= endY; y++)
        {
            int x = round((y - b) / a);
            drawPixel(x, y, 0, 0, 0);
        }
    }

    _canvas->update();
}

void PixelPainter::drawSquare(int x, int y, int a)
{
    drawLine(x, y, x + a, y);
    drawLine(x + a, y, x + a, y + a);
    drawLine(x + a, y + a, x, y + a);
    drawLine(x, y + a, x, y);
}

void PixelPainter::drawMarker(const Marker& marker)
{
    drawSquare(marker.getX(), marker.getY(), marker.getSize());
}

void PixelPainter::drawCurve(QList<QPoint> points)
{
    int n = points.size();

    if (n < 4)
        return;

    if (((n - 4) % 3) != 0)
        return;

    QPoint* p0 = &points[0];
    QPoint* p1 = &points[1];
    QPoint* p2 = &points[2];
    QPoint* p3 = &points[3];

    for (double t = 0 ; t < 1 ; t += 0.0001)
    {
        double t2 = pow(t, 2);
        double t3 = pow(t, 3);
        double nt3 = pow((1 - t), 3);
        double nt2 = pow((1 - t), 2);

        double x = nt3 * p0->x() + 3 * nt2 * t * p1->x() +
                    3 * (1 - t) * t2 * p2->x() + t3 * p3->x();
        double y = nt3 * p0->y() + 3 * nt2 * t * p1->y() +
                    3 * (1 - t) * t2 * p2->y() + t3 * p3->y();
        drawPixel(round(x), round(y));
    }

    for (int i = 0; i < 3; i++)
        points.pop_front();
    drawCurve(points);

    _canvas->update();
}

void PixelPainter::clear()
{
    for (int y = 0; y < _canvas->getHeight(); y++)
    {
        for (int x = 0; x < _canvas->getWidth(); x++)
            drawPixel(x, y, 255, 255, 255);
    }
    _canvas->update();
}


