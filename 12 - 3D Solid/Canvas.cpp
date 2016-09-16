#include "Canvas.h"
#include "MainWindow.h"
#include <cmath>
#include <QMatrix4x4>
#include <QVector2D>
#include <QDebug>

QVector3D multiply(const QMatrix3x3& matrix, const QVector3D& vector)
{
    QVector3D vec;
    vec.setX(matrix(0, 0) * vector.x() + matrix(0, 1) * vector.y() + matrix(0, 2) * vector.z());
    vec.setY(matrix(1, 0) * vector.x() + matrix(1, 1) * vector.y() + matrix(1, 2) * vector.z());
    vec.setZ(matrix(2, 0) * vector.x() + matrix(2, 1) * vector.y() + matrix(2, 2) * vector.z());
    return vec;
}

QVector4D multiply(const QMatrix4x4& matrix, const QVector4D& vector)
{
    QVector4D vec;
    vec.setX(matrix(0, 0) * vector.x() + matrix(0, 1) * vector.y() + matrix(0, 2) * vector.z() + matrix(0, 3) * vector.w());
    vec.setY(matrix(1, 0) * vector.x() + matrix(1, 1) * vector.y() + matrix(1, 2) * vector.z() + matrix(1, 3) * vector.w());
    vec.setZ(matrix(2, 0) * vector.x() + matrix(2, 1) * vector.y() + matrix(2, 2) * vector.z() + matrix(2, 3) * vector.w());
    vec.setW(matrix(3, 0) * vector.x() + matrix(3, 1) * vector.y() + matrix(3, 2) * vector.z() + matrix(3, 3) * vector.w());
    return vec;
}

QVector4D multiply(const QMatrix4x4& matrix, QVector4D* vector)
{
    return multiply(matrix, *vector);
}

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

void Canvas::update()
{
    _mainWindow->update();
}

void Canvas::draw()
{
    QPainter painter(_mainWindow);
    painter.drawImage(_x, _y, *_image);
}

void Canvas::drawPixel(int x, int y, const QColor& color)
{
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
        return;
     _image->setPixel(QPoint(x, y), color.rgba());
}

void Canvas::drawLine(const QVector2D& p1, const QVector2D& p2, const QColor& color)
{
    int x1 = p1.x();
    int y1 = p1.y();
    int x2 = p2.x();
    int y2 = p2.y();

    double a = (y1 - static_cast<double>(y2)) / (x1 - static_cast<double>(x2));
    double b = y1 - a * x1;

    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    int x = x1;
    int y = y1;

    if (x1 == x2)
    {
        for (; y <= y2; y++)
            drawPixel(x1, y, color);
    }
    else if (y1 == y2)
    {
        for (; x <= x2; x++)
            drawPixel(x, y1, color);
    }
    else
    {
        for (; x <= x2; x++)
        {
            int y = round(a * x + b);
            drawPixel(x, y, color);
        }

        for (; y <= y2; y++)
        {
            int x = round((y - b) / a);
            drawPixel(x, y, color);
        }
    }
}

void Canvas::drawLine(const QVector4D& p1, const QVector4D& p2, const QColor& color)
{
    drawLine(to2D(p1), to2D(p2), color);
}

float calculateLight(const QVector4D& v0, const QVector4D& v1, const QVector4D& v2)
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

void Canvas::drawCube(Cube* cube, const QVector3D& pos,
                      const QVector3D& scale, const QVector3D& rotation,
                      const QVector3D& shearing)
{
    float sinx = sinf(rotation.x());
    float siny = sinf(rotation.y());
    float sinz = sinf(rotation.z());
    float cosx = cosf(rotation.x());
    float cosy = cosf(rotation.y());
    float cosz = cosf(rotation.z());

    float originX = cube->getSize() / 2.0f * scale.x();
    float originY = cube->getSize() / 2.0f * scale.y();
    float originZ = cube->getSize() / 2.0f * scale.z();

    QMatrix4x4 translation(1.0f, 0.0f, 0.0f, pos.x(),
                           0.0f, 1.0f, 0.0f, pos.y(),
                           0.0f, 0.0f, 1.0f, pos.z(),
                           0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 translationO(1.0f, 0.0f, 0.0f, originX,
                            0.0f, 1.0f, 0.0f, originY,
                            0.0f, 0.0f, 1.0f, originZ,
                            0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 translationOM(1.0f, 0.0f, 0.0f, -originX,
                            0.0f, 1.0f, 0.0f, -originY,
                            0.0f, 0.0f, 1.0f, -originZ,
                            0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 scaleM(scale.x(), 0.0f, 0.0f, 0.0f,
                      0.0f, scale.y(), 0.0f, 0.0f,
                      0.0f, 0.0f, scale.z(), 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 rotationX(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, cosx, -sinx, 0.0f,
                         0.0f, sinx, cosx, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 rotationY(cosy, 0.0f, siny, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         -siny, 0.0f, cosy, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 rotationZ(cosz, -sinz, 0.0f, 0.0f,
                         sinz, cosz, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 shearX(1.0f, 0.0f, 0.0f, 0.0f,
                      shearing.y(), 1.0f, 0.0f, 0.0f,
                      shearing.z(), 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 shearY(1.0f, shearing.x(), 0.0f, 0.0f,
                      0.0f, 1.0f, 0.0f, 0.0f,
                      0.0f, shearing.z(), 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QMatrix4x4 shearZ(1.0f, 0.0f, shearing.x(), 0.0f,
                      0.0f, 1.0f, shearing.y(), 0.0f,
                      0.0f, 0.0f, 1.0f, 0.0f,
                      0.0f, 0.0f, 0.0f, 1.0f);

    QVector4D pf[4];
    QVector4D pb[4];

    for (int i = 0; i < 4; i++)
    {
        pf[i] = multiply(scaleM, cube->getPF(i));
        pb[i] = multiply(scaleM, cube->getPB(i));

        pf[i] = multiply(shearX, pf[i]);
        pb[i] = multiply(shearX, pb[i]);

        pf[i] = multiply(shearY, pf[i]);
        pb[i] = multiply(shearY, pb[i]);

        pf[i] = multiply(shearZ, pf[i]);
        pb[i] = multiply(shearZ, pb[i]);

        pf[i] = multiply(translationOM, pf[i]);
        pb[i] = multiply(translationOM, pb[i]);

        pf[i] = multiply(rotationX, pf[i]);
        pb[i] = multiply(rotationX, pb[i]);

        pf[i] = multiply(rotationY, pf[i]);
        pb[i] = multiply(rotationY, pb[i]);

        pf[i] = multiply(rotationZ, pf[i]);
        pb[i] = multiply(rotationZ, pb[i]);

        pf[i] = multiply(translationO, pf[i]);
        pb[i] = multiply(translationO, pb[i]);

        pf[i] = multiply(translation, pf[i]);
        pb[i] = multiply(translation, pb[i]);
    }

    QRgb black = QColor(0, 0, 0).rgb();
    QRgb red = QColor(255, 0, 0).rgb();
    QRgb blue = QColor(0, 0, 255).rgb();


    // Order of vertices:
    // 0   1
    //
    // 3   2

    QImage* image = _mainWindow->getTexture();
    float imageWidth = image->width();
    float imageHeight = image->height();

    // TODO: Refactor this ugly code.
    if (_mainWindow->isHideFacesModeOn())
    {
        if (isFaceVisible(pf[3], pb[3], pf[0]))
        {
            if (_mainWindow->isTextureModeOn())
            {
                float light = _mainWindow->isLightModeOn() ?
                                calculateLight(pb[0], pb[3], pf[3]) : 1.0f;
                Triangle src(QVector2D(imageWidth, 0.0f),
                             QVector2D(imageWidth, imageHeight),
                             QVector2D(0.0f, imageHeight));
                Triangle dst(to2D(pb[0]), to2D(pb[3]), to2D(pf[3]));
                drawTexture(image, dst, src, light);

                light = _mainWindow->isLightModeOn() ?
                            calculateLight(pf[0], pb[0], pf[3]) : 1.0f;
                src.setVertex(0, QVector2D(0.0f, 0.0f));
                src.setVertex(1, QVector2D(imageWidth, 0.0f));
                src.setVertex(2, QVector2D(0.0f, imageHeight));
                dst.setVertex(0, to2D(pf[0]));
                dst.setVertex(1, to2D(pb[0]));
                dst.setVertex(2, to2D(pf[3]));
                drawTexture(image, dst, src, light);
            }
            else
            {
                drawLine(pf[0], pb[0], black);
                drawLine(pf[3], pb[3], black);
                drawLine(pb[0], pb[3], black);
                drawLine(pf[0], pf[3], black);
            }
        }

        if (isFaceVisible(pf[2], pf[1], pb[2]))
        {
            if (_mainWindow->isTextureModeOn())
            {
                float light = _mainWindow->isLightModeOn() ?
                                calculateLight(pb[1], pb[2], pf[2]) : 1.0f;
                Triangle src(QVector2D(imageWidth, 0.0f),
                             QVector2D(imageWidth, imageHeight),
                             QVector2D(0.0f, imageHeight));
                Triangle dst(to2D(pb[1]), to2D(pb[2]), to2D(pf[2]));
                drawTexture(image, dst, src, light);

                light = _mainWindow->isLightModeOn() ?
                            calculateLight(pf[1], pb[1], pf[2]) : 1.0f;
                src.setVertex(0, QVector2D(0.0f, 0.0f));
                src.setVertex(1, QVector2D(imageWidth, 0.0f));
                src.setVertex(2, QVector2D(0.0f, imageHeight));
                dst.setVertex(0, to2D(pf[1]));
                dst.setVertex(1, to2D(pb[1]));
                dst.setVertex(2, to2D(pf[2]));
                drawTexture(image, dst, src, light);
            }
            else
            {
                drawLine(pf[1], pf[2], black);
                drawLine(pb[1], pb[2], black);
                drawLine(pf[2], pb[2], black);
                drawLine(pf[1], pb[1], black);
            }
        }

        if (isFaceVisible(pf[1], pf[0], pb[1]))
        {
            if (_mainWindow->isTextureModeOn())
            {
                float light = _mainWindow->isLightModeOn() ?
                                calculateLight(pb[0], pb[1], pf[1]) : 1.0f;
                Triangle src(QVector2D(imageWidth, 0.0f),
                             QVector2D(imageWidth, imageHeight),
                             QVector2D(0.0f, imageHeight));
                Triangle dst(to2D(pb[0]), to2D(pb[1]), to2D(pf[1]));
                drawTexture(image, dst, src, light);

                light = _mainWindow->isLightModeOn() ?
                            calculateLight(pf[0], pb[0], pf[1]) : 1.0f;
                src.setVertex(0, QVector2D(0.0f, 0.0f));
                src.setVertex(1, QVector2D(imageWidth, 0.0f));
                src.setVertex(2, QVector2D(0.0f, imageHeight));
                dst.setVertex(0, to2D(pf[0]));
                dst.setVertex(1, to2D(pb[0]));
                dst.setVertex(2, to2D(pf[1]));
                drawTexture(image, dst, src, light);
            }
            else
            {
                drawLine(pf[0], pf[1], black);
                drawLine(pb[0], pb[1], black);
                drawLine(pf[0], pb[0], black);
                drawLine(pf[1], pb[1], black);
            }
        }

        if (isFaceVisible(pf[2], pb[2], pf[3]))
        {
            if (_mainWindow->isTextureModeOn())
            {
                float light = _mainWindow->isLightModeOn() ?
                                calculateLight(pb[2], pb[3], pf[3]) : 1.0f;
                Triangle src(QVector2D(imageWidth, 0.0f),
                             QVector2D(imageWidth, imageHeight),
                             QVector2D(0.0f, imageHeight));
                Triangle dst(to2D(pb[2]), to2D(pb[3]), to2D(pf[3]));
                drawTexture(image, dst, src, light);

                light = _mainWindow->isLightModeOn() ?
                            calculateLight(pf[2], pb[2], pf[3]) : 1.0f;
                src.setVertex(0, QVector2D(0.0f, 0.0f));
                src.setVertex(1, QVector2D(imageWidth, 0.0f));
                src.setVertex(2, QVector2D(0.0f, imageHeight));
                dst.setVertex(0, to2D(pf[2]));
                dst.setVertex(1, to2D(pb[2]));
                dst.setVertex(2, to2D(pf[3]));
                drawTexture(image, dst, src, light);
            }
            else
            {
                drawLine(pf[2], pf[3], black);
                drawLine(pb[2], pb[3], black);
                drawLine(pf[3], pb[3], black);
                drawLine(pf[2], pb[2], black);
            }
        }

        if (isFaceVisible(pf[2], pf[3], pf[1]))
        {
            if (_mainWindow->isTextureModeOn())
            {
                float light = _mainWindow->isLightModeOn() ?
                                calculateLight(pf[1], pf[2], pf[3]) : 1.0f;
                Triangle src(QVector2D(imageWidth, 0.0f),
                             QVector2D(imageWidth, imageHeight),
                             QVector2D(0.0f, imageHeight));
                Triangle dst(to2D(pf[1]), to2D(pf[2]), to2D(pf[3]));
                drawTexture(image, dst, src, light);

                light = _mainWindow->isLightModeOn() ?
                            calculateLight(pf[0], pf[1], pf[3]) : 1.0f;
                src.setVertex(0, QVector2D(0.0f, 0.0f));
                src.setVertex(1, QVector2D(imageWidth, 0.0f));
                src.setVertex(2, QVector2D(0.0f, imageHeight));
                dst.setVertex(0, to2D(pf[0]));
                dst.setVertex(1, to2D(pf[1]));
                dst.setVertex(2, to2D(pf[3]));
                drawTexture(image, dst, src, light);
            }
            else
            {
                drawLine(pf[0], pf[1], red);
                drawLine(pf[2], pf[3], red);
                drawLine(pf[2], pf[1], red);
                drawLine(pf[0], pf[3], red);

            }
        }

        if (isFaceVisible(pb[2], pb[1], pb[3])) {
            if (_mainWindow->isTextureModeOn())
            {
                float light = _mainWindow->isLightModeOn() ?
                                calculateLight(pb[1], pb[2], pb[3]) : 1.0f;
                Triangle src(QVector2D(imageWidth, 0.0f),
                             QVector2D(imageWidth, imageHeight),
                             QVector2D(0.0f, imageHeight));
                Triangle dst(to2D(pb[1]), to2D(pb[2]), to2D(pb[3]));
                drawTexture(image, dst, src, light);

                light = _mainWindow->isLightModeOn() ?
                            calculateLight(pb[0], pb[1], pb[3]) : 1.0f;
                src.setVertex(0, QVector2D(0.0f, 0.0f));
                src.setVertex(1, QVector2D(imageWidth, 0.0f));
                src.setVertex(2, QVector2D(0.0f, imageHeight));
                dst.setVertex(0, to2D(pb[0]));
                dst.setVertex(1, to2D(pb[1]));
                dst.setVertex(2, to2D(pb[3]));
                drawTexture(image, dst, src, light);
            }
            else
            {
                drawLine(pb[0], pb[1], blue);
                drawLine(pb[2], pb[3], blue);
                drawLine(pb[2], pb[1], blue);
                drawLine(pb[0], pb[3], blue);
            }
        }
    }
    else
    {
        drawLine(pf[0], pb[0], black);
        drawLine(pf[1], pb[1], black);
        drawLine(pf[2], pb[2], black);
        drawLine(pf[3], pb[3], black);
        drawLine(pf[0], pf[1], red);
        drawLine(pf[2], pf[3], red);
        drawLine(pf[2], pf[1], red);
        drawLine(pf[0], pf[3], red);
        drawLine(pb[0], pb[1], blue);
        drawLine(pb[2], pb[3], blue);
        drawLine(pb[2], pb[1], blue);
        drawLine(pb[0], pb[3], blue);
    }
}

int clamp(float n, int min, int max)
{
    if (n < min) return min;
    else if (n > max) return max;
    return n;
}

void Canvas::drawTexture(QImage* image, const Triangle& dst, const Triangle& src, float light)
{
    QVector2D minPos = dst.getVertex(0);
    QVector2D maxPos = dst.getVertex(0);

    for (int i = 1; i < 3; i++)
    {
        QVector2D pos = dst.getVertex(i);

        if (minPos.x() > pos.x())
            minPos.setX(pos.x());

        if (minPos.y() > pos.y())
            minPos.setY(pos.y());

        if (maxPos.x() < pos.x())
            maxPos.setX(pos.x());

        if (maxPos.y() < pos.y())
            maxPos.setY(pos.y());
    }

    float width = maxPos.x() - minPos.x();
    float height = maxPos.y() - minPos.y();

    float xa = dst.getVertex(0).x();
    float ya = dst.getVertex(0).y();
    float xb = dst.getVertex(1).x();
    float yb = dst.getVertex(1).y();
    float xc = dst.getVertex(2).x();
    float yc = dst.getVertex(2).y();

    float m = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);

    float xta = src.getVertex(0).x();
    float yta = src.getVertex(0).y();
    float xtb = src.getVertex(1).x();
    float ytb = src.getVertex(1).y();
    float xtc = src.getVertex(2).x();
    float ytc = src.getVertex(2).y();

    int textureWidth = image->width();
    int textureHeight = image->height();

    for (float y = minPos.y(); y <= minPos.y() + height; y += 0.5f)
    {
        for (float x = minPos.x(); x <= minPos.x() + width; x += 0.5f)
        {
            float v = ((x - xa) * (yc - ya) - (xc - xa) * (y - ya)) / m;
            float w = ((xb - xa) * (y - ya) - (x - xa) * (yb - ya)) / m;
            float u = 1.0f - v - w;

            if (u < 0.0f || v < 0.0f || w < 0.0f || u > 1.0f || v > 1.0f || w > 1.0f)
                continue;

            int xt = roundf(u * xta + v * xtb + w * xtc);
            int yt = roundf(u * yta + v * ytb + w * ytc);
            if (xt >= textureWidth) xt = textureWidth - 1;
            if (yt >= textureHeight) yt = textureHeight - 1;

            QColor color = image->pixel(QPoint(xt, yt));
            color.setRed(clamp(color.red() * light , 0, 255));
            color.setGreen(clamp(color.green() * light , 0, 255));
            color.setBlue(clamp(color.blue() * light , 0, 255));

            drawPixel(x, y, color);
        }
    }
}

void Canvas::reset()
{
    QRgb whiteColor = QColor(255, 255, 255).rgb();
    for (int y = 0; y < getWidth(); y++)
    {
        for (int x = 0; x < getHeight(); x++)
            _image->setPixel(QPoint(x, y), whiteColor);
    }
}

QVector2D Canvas::to2D(const QVector3D& vector3D)
{
    // d = dobrac
    // y' = y/(1+z/d)
    // x' = x/(1+z/d)
    // z' = 0

    float d = _mainWindow->getD();
    float m = (1.0f + vector3D.z() / d);
    return QVector2D(vector3D.x() / m, vector3D.y() / m);
}

QVector2D Canvas::to2D(const QVector4D& vector4D)
{
    // d = dobrac
    // y' = y/(1+z/d)
    // x' = x/(1+z/d)
    // z' = 0

    float d = _mainWindow->getD();
    float m = (1.0f + vector4D.z() / d);
    return QVector2D(vector4D.x() / m, vector4D.y() / m);
}

bool Canvas::isFaceVisible(const QVector3D& v0, const QVector3D& v1, const QVector3D& v2)
{
    // http://www.cubic.org/docs/backcull.htm
    float d = _mainWindow->getD();
    float m0 = (1.0f + v0.z() / d);
    float m1 = (1.0f + v1.z() / d);
    float m2 = (1.0f + v2.z() / d);

    QVector2D v0d = QVector2D(v0.x() / m0, v0.y() / m0);
    QVector2D v1d = QVector2D(v1.x() / m1, v1.y() / m1);
    QVector2D v2d = QVector2D(v2.x() / m2, v2.y() / m2);

    QVector2D va(v1d.x() - v0d.x(), v1d.y() - v0d.y());
    QVector2D vb(v2d.x() - v0d.x(), v2d.y() - v0d.y());
    float dot = (va.x() * vb.y()) - (va.y() * vb.x());
    return (dot >= 0.0f);
}

bool Canvas::isFaceVisible(const QVector4D& v0, const QVector4D& v1, const QVector4D& v2)
{
    return isFaceVisible(QVector3D(v0.x(), v0.y(), v0.z()),
                         QVector3D(v1.x(), v1.y(), v1.z()),
                         QVector3D(v2.x(), v2.y(), v2.z()));
}
