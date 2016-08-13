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

    bool isPointOn(int x, int y);

    QImage* getImage() const {
        return _image;
    }

    unsigned char* getPixels() const {
        return _imgPixels;
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
    unsigned char* _imgPixels;
    QRect _imgBounds;
};

#endif // CANVAS_H
