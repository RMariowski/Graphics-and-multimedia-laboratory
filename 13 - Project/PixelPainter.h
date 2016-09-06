#ifndef PIXELPAINTER_H
#define PIXELPAINTER_H

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include "Canvas.h"
#include "OBJ.h"

class PixelPainter
{
public:
    PixelPainter(Canvas* canvas);

    virtual ~PixelPainter();

    void drawPixel(int x, int y, int r = 0, int g = 0, int b = 0, float depth = 0.0f);

    void drawLine(int x1, int y1, int x2, int y2);

    void drawLine(const QVector2D& v1, const QVector2D& v2);

    void drawTexture(Triangle* triangle, QVector2D* vertices, float light);

    void drawTriangle(Triangle* triangle, bool wireframeMode, bool textureMode,
                      bool lightMode);

    void drawOBJ(OBJ* obj, const QVector3D& pos, const QVector3D& scale,
                 const QVector3D& rotation, const QVector3D& shearing,
                 bool wireframeMode, bool textureMode, bool lightMode);

    void clear();

    QVector2D to2D(const QVector4D& vector4D);

    Canvas* getCanvas() const {
        return _canvas;
    }

private:
    float calculateLight(const QVector4D& v0, const QVector4D& v1, const QVector4D& v2);

private:
    Canvas* _canvas;
    float** _depthBuffer;
};

#endif // PIXELPAINTER_H
