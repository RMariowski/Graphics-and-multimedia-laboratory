#ifndef CANVAS_H
#define CANVAS_H

#include <QImage>
#include <QPoint>
#include <QSize>

class MainWindow;

class Canvas
{
public:
    Canvas(MainWindow* mainWindow, const QPoint& pos, const QSize& size);

    ~Canvas();

    void update();

    void draw();

    MainWindow* getMainWindow() const {
        return _mainWindow;
    }

    QImage* getImage() const {
        return _image;
    }

    QPoint& getPos() {
        return _pos;
    }

    int getX() const {
        return _pos.x();
    }

    int getY() const {
        return _pos.y();
    }

    QSize& getSize() {
        return _size;
    }

    int getWidth() const {
        return _size.width();
    }

    int getHeight() const {
        return _size.height();
    }

private:
    MainWindow* _mainWindow;

    QPoint _pos;
    QSize _size;
    QImage* _image;
};

#endif // CANVAS_H
