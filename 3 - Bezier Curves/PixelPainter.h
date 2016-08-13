#ifndef PIXELPAINTER_H
#define PIXELPAINTER_H

#include "Canvas.h"
#include "Marker.h"

class PixelPainter
{
public:
    PixelPainter(Canvas* canvas);

    void drawPixel(int x, int y, int r = 0, int g = 0, int b = 0);

    void drawLine(int startX, int startY, int endX, int endY);

    void drawSquare(int x, int y, int a);

    void drawMarker(const Marker& marker);

    void drawCurve(QList<QPoint> points);

    void clear();

    Canvas* getCanvas() const {
        return _canvas;
    }

private:
    Canvas* _canvas;
};

#endif // PIXELPAINTER_H
