#include "Canvas.h"
#include "MainWindow.h"

Canvas::Canvas(MainWindow* mainWindow, int x, int y, int width, int height)
    : _mainWindow(mainWindow), _imgBounds(x, y, width, height)
{
    _originalImage = new QImage(":/image.jpg");
    _image = new QImage(width, height, _originalImage->format());

    reset();
}

Canvas::~Canvas()
{
    delete _image;
    delete _originalImage;
}

void Canvas::update()
{
    _mainWindow->update();
}

void Canvas::draw()
{
    QPainter painter(_mainWindow);
    painter.drawImage(_imgBounds.x(), _imgBounds.y(), *_image);
}

void Canvas::reset()
{
    for (int y = 0; y < _originalImage->height(); y++) {
        for (int x = 0; x < _originalImage->width(); x++) {
            QPoint p(x, y);
            QRgb color = _originalImage->pixel(p);
            _image->setPixel(p, color);
        }
    }
}

void Canvas::dilatation(int markerSize)
{
    int halfMarkerSize = static_cast<int>(markerSize / 2.0);

    QList<QPoint> points;

    QRgb blackColor = QColor(0, 0, 0).rgb();

    int width = _image->width();
    int height = _image->height();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QPoint currentPixel(x, y);
            QRgb currentPixelColor = _image->pixel(currentPixel);
            if (currentPixelColor != blackColor) {

                for (int i = y - halfMarkerSize; i <= y + halfMarkerSize; i++) {
                    if (i < 0) continue;
                    if (i >= height) break;

                    bool blackFound = false;
                    for (int j = x - halfMarkerSize; j <= x + halfMarkerSize; j++) {
                        if (j < 0) continue;
                        if (j >= width) break;

                        QRgb color = _image->pixel(QPoint(j, i));
                        if (color == blackColor) {
                            points.push_back(currentPixel);
                            blackFound = true;
                            break;
                        }
                    }

                    if (blackFound) break;
                }

            }
        }
    }

    for (int i = 0; i < points.size(); i++)
        _image->setPixel(points[i], blackColor);
}


void Canvas::erosion(int markerSize)
{
    int halfMarkerSize = static_cast<int>(markerSize / 2.0);

    QList<QPoint> points;

    QRgb whiteColor = QColor(255, 255, 255).rgb();

    int width = _image->width();
    int height = _image->height();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            QPoint currentPixel(x, y);
            QRgb currentPixelColor = _image->pixel(currentPixel);
            if (currentPixelColor != whiteColor) {

                for (int i = y - halfMarkerSize; i <= y + halfMarkerSize; i++) {
                    if (i < 0) continue;
                    if (i >= height) break;

                    bool whiteFound = false;
                    for (int j = x - halfMarkerSize; j <= x + halfMarkerSize; j++) {
                        if (j < 0) continue;
                        if (j >= width) break;

                        QRgb color = _image->pixel(QPoint(j, i));
                        if (color == whiteColor) {
                            points.push_back(currentPixel);
                            whiteFound = true;
                            break;
                        }
                    }

                    if (whiteFound) break;
                }

            }
        }
    }

    for (int i = 0; i < points.size(); i++)
        _image->setPixel(points[i], whiteColor);
}

void Canvas::opening(int markerSize)
{
    erosion(markerSize);
    dilatation(markerSize);
}

void Canvas::closing(int markerSize)
{
    dilatation(markerSize);
    erosion(markerSize);
}
