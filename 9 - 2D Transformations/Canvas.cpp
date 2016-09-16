#include "Canvas.h"
#include "MainWindow.h"
#include "Matrix.h"
#include <cmath>

struct Point
{
    double x; double y;
    Point(double x, double y) : x(x), y(y) { }
    Point(const QPoint& point) : x(point.x()), y(point.y()) { }
    QPoint toQ() { return QPoint(x, y); }
};

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

void Canvas::drawPixel(int x, int y, QRgb color)
{
    if (x < 0 || y < 0 || x >= getWidth() || y >= getHeight())
        return;
     _image->setPixel(QPoint(x, y), color);
}

void Canvas::drawImage(QImage* image, const QPoint& position,
                       double scaleX, double scaleY, double rotation,
                       double shearingX, double shearingY)
{
    Matrix<double> translation(1.0, 0.0, position.x(),
                               0.0, 1.0, position.y(),
                               0.0, 0.0, 1.0);

    Matrix<double> shearX(1.0, shearingX, 0.0,
                          0.0, 1.0, 0.0,
                          0.0, 0.0, 1.0);

    Matrix<double> shearY(1.0, 0.0, 0.0,
                          shearingY, 1.0, 0.0,
                          0.0, 0.0, 1.0);

    double width = image->width() * scaleX;
    double height = image->height()* scaleY;
    double sinr = sin(rotation);
    double cosr = cos(rotation);
    Point centerPoint(width / 2.0, height / 2.0);

    Matrix<double> rotate(cosr, -sinr, centerPoint.x,
                          sinr, cosr, centerPoint.y,
                          0.0, 0.0, 1.0);

    QVector3D vector(1.0f, 1.0f, 1.0f);

    for (double y = 0.0; y < height; y += 0.6)
    {
        for (double x = 0.0; x < width; x += 0.6)
        {
            vector.setX(x - centerPoint.x);
            vector.setY(y - centerPoint.y);

            vector = rotate * vector;
            vector = shearX * vector;
            vector = shearY * vector;
            vector = translation * vector;

            QRgb color = image->pixel(QPoint(x / scaleX, y / scaleY));
            int cx = ceil(vector.x());
            int fx = floor(vector.x());
            int cy = ceil(vector.y());
            int fy = floor(vector.y());
            drawPixel(cx, cy, color);
            drawPixel(fx, fy, color);
            drawPixel(fx, cy, color);
            drawPixel(cx, fy, color);
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
