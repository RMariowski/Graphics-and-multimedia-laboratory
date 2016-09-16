#include "Canvas.h"
#include "MainWindow.h"
#include <cmath>
#include <QDebug>

Canvas::Canvas(MainWindow* mainWindow, int x, int y, int width, int height, bool morph)
    : _mainWindow(mainWindow), _x(x), _y(y), _morph(morph)
{
    _image = new QImage(width, height, QImage::Format_ARGB32);
    _abcn.createImage(width, height);
    _morphTriangle.createImage(width, height);
    _blendedImage = new QImage(width, height, QImage::Format_ARGB32);
    reset();
}

Canvas::~Canvas()
{
    _abcn.deleteImage();
    _morphTriangle.deleteImage();
    delete _blendedImage;
    delete _image;
}

void Canvas::draw()
{
    QPainter painter(_mainWindow);
    painter.drawImage(_x, _y, *_image);
    if (_morph) painter.drawImage(_x, _y, *_morphTriangle.getImage());
}

void Canvas::drawPixel(int x, int y, const QColor& color)
{
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
        return;
    _image->setPixel(QPoint(x, y), color.rgba());
}

void Canvas::drawPixel(QImage* dst, int x, int y, const QColor& color)
{
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
        return;
    dst->setPixel(QPoint(x, y), color.rgba());
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
            drawPixel(x + marker->getDrawX(), y + marker->getDrawY(), color);
    }
}

void Canvas::drawTexture(const Triangle& dst, const Triangle& src)
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

    int textureWidth = src.getImage()->width();
    int textureHeight = src.getImage()->height();
    QColor workColor = QColor(255, 0, 0);

    for (float y = minPos.y(); y < minPos.y() + height; y++)
    {
        for (float x = minPos.x(); x < minPos.x() + width; x++)
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

            QPoint p(xt, yt);
            QColor color = src.getImage()->pixel(p);
            if (color == workColor)
                color = _blendedImage->pixel(p);
            drawPixel(dst.getImage(), x, y, color);
        }
    }
}

void Canvas::drawTextureWithMorphing(const Triangle& dst, const Triangle& src, const Triangle& src2, float animPart)
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

    float xt2a = src2.getVertex(0).x();
    float yt2a = src2.getVertex(0).y();
    float xt2b = src2.getVertex(1).x();
    float yt2b = src2.getVertex(1).y();
    float xt2c = src2.getVertex(2).x();
    float yt2c = src2.getVertex(2).y();

    int textureWidth = src.getImage()->width();
    int textureHeight = src.getImage()->height();
    int texture2Width = src.getImage()->width();
    int texture2Height = src.getImage()->height();

    for (float y = minPos.y(); y < minPos.y() + height; y++)
    {
        for (float x = minPos.x(); x < minPos.x() + width; x++)
        {
            float v = ((x - xa) * (yc - ya) - (xc - xa) * (y - ya)) / m;
            float w = ((xb - xa) * (y - ya) - (x - xa) * (yb - ya)) / m;
            float u = 1.0f - v - w;

            if (u <= 0.0f || v <= 0.0f || w <= 0.0f || u >= 1.0f || v >= 1.0f || w >= 1.0f)
                continue;

            float xt = roundf(u * xta + v * xtb + w * xtc);
            float yt = roundf(u * yta + v * ytb + w * ytc);
            if (xt >= textureWidth) xt = textureWidth - 1;
            if (yt >= textureHeight) yt = textureHeight - 1;

            float xt2 = roundf(u * xt2a + v * xt2b + w * xt2c);
            float yt2 = roundf(u * yt2a + v * yt2b + w * yt2c);
            if (xt2 >= texture2Width) xt2 = texture2Width - 1;
            if (yt2 >= texture2Height) yt2 = texture2Height - 1;

            QColor color1 = src.getImage()->pixel(QPoint(xt, yt));
            QColor color2 = src2.getImage()->pixel(QPoint(xt2, yt2));
            int outR = (1.0f - animPart) * color1.red() + animPart * color2.red();
            int outG = (1.0f - animPart) * color1.green() + animPart * color2.green();
            int outB = (1.0f - animPart) * color1.blue() + animPart * color2.blue();

            drawPixel(dst.getImage(), x, y, QColor(outR, outG, outB));
        }
    }
}

void Canvas::morphing(const Triangle& abc, const Triangle& abcp, float animPart)
{
    for (int i = 0; i < 3; i++)
    {
        QVector2D abcPos = abc.getVertex(i);
        QVector2D abcpPos = abcp.getVertex(i);
        QVector2D pos(roundf((1.0f - animPart) * abcPos.x() + animPart * abcpPos.x()),
                      roundf((1.0f - animPart) * abcPos.y() + animPart * abcpPos.y()));
        _abcn.setVertex(i, pos);
    }

    createBlendedImage(abc, abcp, animPart);

    Triangle pqr = _abcn;
    Triangle pqrp = _abcn;

    pqr.setImage(abc.getImage());
    pqrp.setImage(abcp.getImage());
    drawTextureWithMorphing(_abcn, pqr, pqrp, animPart);

    for (int i = 0; i < 3; i++)
        _morphTriangle.setVertex(i, abc.getVertex(i));
    drawTexture(_morphTriangle, _abcn);
}

void Canvas::drawTexturedGridHelper(QImage* texture, QImage* texture2,
                                    const QList<Marker>& markers, const QList<Marker>& markers2,
                                    int id01, int id11, int id02, int id12,
                                    const Marker* center1, const Marker* center2,
                                    double animPart)
{
    QList<Marker> m1;
    QList<Marker> m2;

    m1.push_back(markers[id01]);
    m1.push_back(markers[id11]);
    m1.push_back(*center1);

    m2.push_back(markers2[id02]);
    m2.push_back(markers2[id12]);
    m2.push_back(*center2);

    morphing(Triangle(texture, m1), Triangle(texture2, m2), animPart);
}

void Canvas::morphingGrid(QImage* texture, QImage* texture2,
                          const QList<Marker>& markers, const QList<Marker>& markers2,
                          float animPart)
{
    const Marker* center1 = &markers[8];
    const Marker* center2 = &markers2[8];

    drawTexturedGridHelper(texture, texture2, markers, markers2, 0, 1, 0, 1, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 1, 2, 1, 2, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 2, 4, 2, 4, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 4, 7, 4, 7, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 6, 7, 6, 7, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 5, 6, 5, 6, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 3, 5, 3, 5, center1, center2, animPart);
    drawTexturedGridHelper(texture, texture2, markers, markers2, 0, 3, 0, 3, center1, center2, animPart);
}

void Canvas::createBlendedImage(const Triangle& abc, const Triangle& abcp, float animPart)
{
    for (int y = 0; y < abc.getImage()->height(); y++)
    {
        for (int x = 0; x < abc.getImage()->width(); x++)
        {
            QPoint p(x, y);
            QColor color1 = abc.getImage()->pixel(p);
            QColor color2 = abcp.getImage()->pixel(p);
            int outR = (1.0f - animPart) * color1.red() + animPart * color2.red();
            int outG = (1.0f - animPart) * color1.green() + animPart * color2.green();
            int outB = (1.0f - animPart) * color1.blue() + animPart * color2.blue();
            drawPixel(_blendedImage, x, y, QColor(outR, outG, outB));
        }
    }
}

void Canvas::copyImage(QImage* dst, QImage* src)
{
    for (int y = 0; y < src->height(); y++)
    {
        for (int x = 0; x < src->width(); x++)
        {
            QPoint p(x, y);
            dst->setPixel(p, src->pixel(p));
        }
    }
}

void Canvas::reset()
{
    QRgb whiteColor = QColor(255, 255, 255).rgb();
    for (int y = 0; y < getHeight(); y++)
    {
        for (int x = 0; x < getWidth(); x++)
        {
            QPoint p(x, y);
            _image->setPixel(p, whiteColor);
        }
    }
    _abcn.resetImage();
    _morphTriangle.resetImage();
}

bool Canvas::isIntersect(int x, int y)
{
    return (x >= _x && y >= _y &&
            x <= _x + _image->width() &&
            y <= _y + _image->height());
}
