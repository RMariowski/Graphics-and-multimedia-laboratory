#include "PixelPainter.h"
#include "MainWindow.h"

#include <cmath>
#include <vector>
#define PI 3.14159265358979323846

#define INF -999999.0f

PixelPainter::PixelPainter(Canvas* canvas)
    : _canvas(canvas)
{
    _depthBuffer = new float*[canvas->getWidth()];
    for (int x = 0; x < canvas->getWidth(); x++)
        _depthBuffer[x] = new float[canvas->getHeight()];
}

PixelPainter::~PixelPainter()
{
    for (int x = 0; x < _canvas->getWidth(); x++)
        delete[] _depthBuffer[x];
    delete[] _depthBuffer;
}

void PixelPainter::drawPixel(int x, int y, int r, int g, int b, float depth)
{
    int canvasWidth = _canvas->getWidth();
    if (x < 0 || y < 0 || x >= canvasWidth || y >= _canvas->getHeight())
        return;

    if (_depthBuffer[x][y] > depth)
        return;

    _depthBuffer[x][y] = depth;

    unsigned char* ptr = _canvas->getImage()->bits();
    ptr[canvasWidth * 4 * y + 4 * x] = b;
    ptr[canvasWidth * 4 * y + 4 * x + 1] = g;
    ptr[canvasWidth * 4 * y + 4 * x + 2] = r;
}

void PixelPainter::drawLine(int x1, int y1, int x2, int y2)
{
    double a = (y1 - static_cast<double>(y2)) / (x1 - static_cast<double>(x2));
    double b = y1 - a * x1;

    if (x1 > x2)
        std::swap(x1, x2);

    if (y1 > y2)
        std::swap(y1, y2);

    int x = x1;
    int y = y1;

    if (x1 == x2)
    {
        for (; y <= y2; y++)
            drawPixel(x1, y);
    }
    else if (y1 == y2)
    {
        for (; x <= x2; x++)
            drawPixel(x, y1);
    }
    else
    {
        for (; x <= x2; x++)
        {
            int y = round(a * x + b);
            drawPixel(x, y);
        }

        for (; y <= y2; y++)
        {
            int x = round((y - b) / a);
            drawPixel(x, y);
        }
    }
}

void PixelPainter::drawLine(const QVector2D& v1, const QVector2D& v2)
{
    drawLine(v1.x(), v1.y(), v2.x(), v2.y());
}

int clamp(float n, int min, int max)
{
    if (n < min) return min;
    else if (n > max) return max;
    return n;
}

void PixelPainter::drawTexture(Triangle* triangle, QVector2D* vertices, float light)
{
    QVector2D minPos = vertices[0];
    QVector2D maxPos = vertices[0];

    for (int i = 1; i < 3; i++)
    {
        QVector2D pos = vertices[i];
        if (minPos.x() > pos.x()) minPos.setX(pos.x());
        if (minPos.y() > pos.y()) minPos.setY(pos.y());
        if (maxPos.x() < pos.x()) maxPos.setX(pos.x());
        if (maxPos.y() < pos.y()) maxPos.setY(pos.y());
    }

    float width = maxPos.x() - minPos.x();
    float height = maxPos.y() - minPos.y();

    float xa = vertices[0].x();
    float ya = vertices[0].y();
    float xb = vertices[1].x();
    float yb = vertices[1].y();
    float xc = vertices[2].x();
    float yc = vertices[2].y();

    float z1 = triangle->getTransformedVertex(0).z();
    float z2 = triangle->getTransformedVertex(1).z();
    float z3 = triangle->getTransformedVertex(2).z();

    float m = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);

    int textureWidth = triangle->getTexture()->width();
    int textureHeight = triangle->getTexture()->height();

    float xta = triangle->getTexcoords(0).x() * textureWidth;
    float yta = triangle->getTexcoords(0).y() * textureHeight;
    float xtb = triangle->getTexcoords(1).x() * textureWidth;
    float ytb = triangle->getTexcoords(1).y() * textureHeight;
    float xtc = triangle->getTexcoords(2).x() * textureWidth;
    float ytc = triangle->getTexcoords(2).y() * textureHeight;

    for (float y = minPos.y(); y <= minPos.y() + height; y+=0.5f) {
        for (float x = minPos.x(); x <= minPos.x() + width; x+=0.5f)
        {
            float v = ((x - xa) * (yc - ya) - (xc - xa) * (y - ya)) / m;
            float w = ((xb - xa) * (y - ya) - (x - xa) * (yb - ya)) / m;
            float u = 1.0f - v - w;

            if (u < 0.0f || v < 0.0f || w < 0.0f || u > 1.0f || v > 1.0f || w > 1.0f)
                continue;

            float depth = (u * z1 + v * z2 + w * z3);
            int xt = roundf(u * xta + v * xtb + w * xtc);
            int yt = roundf(u * yta + v * ytb + w * ytc);
            if (xt < 0) xt = 0;
            if (yt < 0) yt = 0;
            if (xt >= textureWidth) xt = textureWidth - 1;
            if (yt >= textureHeight) yt = textureHeight - 1;

            QColor color = triangle->getTexture()->pixel(QPoint(xt, yt));
            color.setRed(clamp(color.red() * light , 0, 255));
            color.setGreen(clamp(color.green() * light , 0, 255));
            color.setBlue(clamp(color.blue() * light , 0, 255));
            drawPixel(x, y, color.red(), color.green(), color.blue(), depth);
        }
    }
}

void PixelPainter::drawTriangle(Triangle* triangle, bool wireframeMode, bool textureMode,
                                bool lightMode)
{
    QVector4D v[3];
    for (int i = 0; i < 3; i++)
        v[i] = triangle->getTransformedVertex(i);

    float light = lightMode ? calculateLight(v[0], v[1], v[2]) : 1.0f;

    QVector2D v2D[3];
    for (int i = 0; i < 3; i++)
        v2D[i] = to2D(v[i]);

    if (wireframeMode)
    {
        drawLine(v2D[0], v2D[1]);
        drawLine(v2D[1], v2D[2]);
        drawLine(v2D[2], v2D[0]);
    }

    if (textureMode)
        drawTexture(triangle, v2D, light);
}

void PixelPainter::drawOBJ(OBJ* obj, const QVector3D& pos, const QVector3D& scale,
                           const QVector3D& rotation, const QVector3D& shearing,
                           bool wireframeMode, bool textureMode, bool lightMode)
{
    QVector3D center(_canvas->getWidth() / 2.0f,
                     _canvas->getWidth() / 2.0f,
                     0.0f);

    std::vector<Triangle> triangles = obj->getTriangles();
    for (size_t i = 0; i < triangles.size(); i++) {
        triangles[i].applyTransforms(center + pos, scale, rotation, shearing);
        drawTriangle(&triangles[i], wireframeMode, textureMode, lightMode);
    }
}

void PixelPainter::clear()
{
    for (int y = 0; y < _canvas->getHeight(); y++)
    {
        for (int x = 0; x < _canvas->getWidth(); x++)
        {
            _depthBuffer[x][y] = INF;
            drawPixel(x, y, 255, 255, 255, INF + 1);
        }
    }
}

QVector2D PixelPainter::to2D(const QVector4D& vector4D)
{
    float d = _canvas->getMainWindow()->getD();
    float m = (vector4D.z() + d);
    return QVector2D(vector4D.x() * d / m, vector4D.y() * d / m);
}

float PixelPainter::calculateLight(const QVector4D& v0, const QVector4D& v1, const QVector4D& v2)
{
    QVector3D a(v1.x() - v0.x(),
                v1.y() - v0.y(),
                v1.z() - v0.z());

    QVector3D b(v2.x() - v0.x(),
                v2.y() - v0.y(),
                v2.z() - v0.z());

    QVector3D N((a.y() * b.z()) - (a.z() * b.y()),
                (a.z() * b.x()) - (a.x() * b.z()),
                (a.x() * b.y()) - (a.y() * b.x()));

    QVector3D L(-250.0f, -500.0f, -1000.0f);

    float lenN = sqrtf((N.x() * N.x()) + (N.y() * N.y()) + (N.z() * N.z()));
    float lenL = sqrtf((L.x() * L.x()) + (L.y() * L.y()) + (L.z() * L.z()));

    float res = ((N.x() * L.x()) + (N.y() * L.y()) + (N.z() * L.z())) / (lenN * lenL);
    if (res < 0.0f)
        res = -res;
    return res;
}
