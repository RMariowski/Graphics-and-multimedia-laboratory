#ifndef CANVAS_H
#define CANVAS_H

#include <QPainter>
#include <QImage>

class Layer;

enum BlendMode
{
    BLEND_MODE_NONE,
    BLEND_MODE_ALPHA_BLENDING,
    BLEND_MODE_AVERAGE,
    BLEND_MODE_MULTIPLY,
    BLEND_MODE_SCREEN,
    BLEND_MODE_DIFFERENCE,
    BLEND_MODE_NEGATION
};
class MainWindow;

class Canvas
{
public:
    Canvas(MainWindow* mainWindow, int x, int y, int width, int height);

    void update();

    void draw();

    void drawImageAlphaBlending(unsigned char* pixels, int offset, double alpha);

    void drawImage(Layer* layer, BlendMode blendMode, bool firstLayer);

    void clear();

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
    unsigned char* _imgPixels;
    QRect _imgBounds;
};

#endif // CANVAS_H
