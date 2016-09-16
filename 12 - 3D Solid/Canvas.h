#ifndef CANVAS_H
#define CANVAS_H

#include <QPainter>
#include <QImage>

#include "Cube.h"
#include "Triangle.h"

class MainWindow;

class Canvas
{
public:
    Canvas(MainWindow* mainWindow, int x, int y, int width, int height);

    ~Canvas();

    void update();

    void draw();

    void drawPixel(int x, int y, const QColor& color);

    void drawLine(const QVector2D& p1, const QVector2D& p2, const QColor& color);

    void drawLine(const QVector4D& p1, const QVector4D& p2, const QColor& color);

    void drawCube(Cube* cube, const QVector3D& pos,
                  const QVector3D& scale, const QVector3D& rotation,
                  const QVector3D& shearing);

    void drawTexture(QImage* image, const Triangle& dst, const Triangle& src, float light = 1.0f);

    void reset();

    QVector2D to2D(const QVector3D& vector3D);

    QVector2D to2D(const QVector4D& vector4D);

    bool isFaceVisible(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2);

    bool isFaceVisible(const QVector4D& v0, const QVector4D& v1, const QVector4D& v2);

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
    int _x;
    int _y;
};

#endif // CANVAS_H
