#ifndef CANVAS_H
#define CANVAS_H

#include <QPainter>
#include <QImage>

class MainWindow;

class Canvas
{
public:
    Canvas(MainWindow* mainWindow, int x, int y, int width, int height);

    ~Canvas();

    void update();

    void draw();

    void reset();

    void dilatation(int markerSize);

    void erosion(int markerSize);

    void opening(int markerSize);

    void closing(int markerSize);

    QImage* getImage() const {
        return _image;
    }

    int getX() const {
        return _imgBounds.x();
    }

    int getY() const {
        return _imgBounds.y();
    }

    int getWidth() const {
        return _imgBounds.width();
    }

    int getHeight() const {
        return _imgBounds.height();
    }

    QRect getBounds() const {
        return _imgBounds;
    }

private:
    MainWindow* _mainWindow;

    QImage* _image;
    QRect _imgBounds;

    QImage* _originalImage;
};

#endif // CANVAS_H
