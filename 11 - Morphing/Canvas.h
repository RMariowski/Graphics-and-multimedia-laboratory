#ifndef CANVAS_H
#define CANVAS_H

#include <QPainter>
#include <QImage>

#include "Triangle.h"

class MainWindow;

class Canvas
{
public:
    Canvas(MainWindow* mainWindow, int x, int y, int width, int height, bool morph);

    ~Canvas();

    void draw();

    void drawPixel(int x, int y, const QColor& color);

    void drawPixel(QImage* dst, int x, int y, const QColor& color);

    void drawImage(QImage* image);

    void drawMarker(Marker* marker);

    void drawTexture(const Triangle& dst, const Triangle& src);

    void drawTextureWithMorphing(const Triangle& dst, const Triangle& src, const Triangle& src2, float animPart);

    void drawTexture(QImage* texture, QImage* texture2, const QList<Marker>& markers, const QList<Marker>& markers2, double animPart);

    void drawTexturedGridHelper(QImage* texture, QImage* texture2,
                                const QList<Marker>& markers, const QList<Marker>& markers2,
                                int id01, int id11, int id02, int id12,
                                const Marker* center1, const Marker* center2, double animPart);

    void morphing(const Triangle& abc, const Triangle& abcp, float animPart);

    void morphingGrid(QImage* texture, QImage* texture2,
                      const QList<Marker>& markers, const QList<Marker>& markers2,
                      float animPart);

    void createBlendedImage(const Triangle& abc, const Triangle& abcp, float animPart);

    void copyImage(QImage* dst, QImage* src);

    void reset();

    bool isIntersect(int x, int y);

    QImage* getImage() const {
        return _image;
    }

    int getX() const {
        return _x;
    }

    int getY() const {
        return _y;
    }

    int getWidth() const {
        return _image->width();
    }

    int getHeight() const {
        return _image->height();
    }

private:
    MainWindow* _mainWindow;
    QImage* _image;
    QImage* _blendedImage;
    int _x;
    int _y;
    bool _morph;

    Triangle _abcn;
    Triangle _morphTriangle;
};

#endif // CANVAS_H
