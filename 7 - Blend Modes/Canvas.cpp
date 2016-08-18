#include "Canvas.h"
#include "MainWindow.h"
#include "Layer.h"

Canvas::Canvas(MainWindow* mainWindow, int x, int y, int width, int height)
    : _mainWindow(mainWindow), _imgBounds(x, y, width, height)
{
    _image = new QImage(width, height, QImage::Format_ARGB32);
    _imgPixels = _image->bits();
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

void Canvas::drawImageAlphaBlending(unsigned char* pixels, int offset, double alpha)
{
    alpha = static_cast<double>(alpha) / 255.0;
    _imgPixels[offset] = alpha * pixels[offset] + (1.0 - alpha) * _imgPixels[offset];
    _imgPixels[offset + 1] = alpha * pixels[offset + 1] + (1.0 - alpha) * _imgPixels[offset + 1];
    _imgPixels[offset + 2] = alpha * pixels[offset + 2] + (1.0 - alpha) * _imgPixels[offset + 2];
}

void Canvas::drawImage(Layer* layer, BlendMode blendMode, bool firstLayer)
{
    QImage* image = layer->getImage();

    QPainter painter(_mainWindow);
    if (blendMode == BLEND_MODE_NONE) {
        painter.drawImage(_imgBounds.x(), _imgBounds.y(), *image);
        return;
    }

    unsigned char* pixels = image->bits();

    if (firstLayer)
        blendMode = BLEND_MODE_NONE;

    int width = _imgBounds.width();
    for (int y = 0; y < _imgBounds.height(); y++) {
        for (int x = 0; x < width; x++) {
            int offset = width * 4 * y + 4 * x;
            switch (blendMode) {
                case BLEND_MODE_AVERAGE:
                {
                    drawImageAlphaBlending(pixels, offset, layer->getAlpha());
                    _imgPixels[offset] = (_imgPixels[offset] + pixels[offset]) >> 1;
                    _imgPixels[offset + 1] = (_imgPixels[offset + 1] + pixels[offset + 1]) >> 1;
                    _imgPixels[offset + 2] = (_imgPixels[offset + 2] + pixels[offset + 2]) >> 1;
                }
                break;

                case BLEND_MODE_ALPHA_BLENDING:
                {
                    drawImageAlphaBlending(pixels, offset, layer->getAlpha());
                }
                break;

                case BLEND_MODE_MULTIPLY:
                {
                    drawImageAlphaBlending(pixels, offset, layer->getAlpha());
                    _imgPixels[offset] = (_imgPixels[offset] * pixels[offset]) >> 8;
                    _imgPixels[offset + 1] = (_imgPixels[offset + 1] * pixels[offset + 1]) >> 8;
                    _imgPixels[offset + 2] = (_imgPixels[offset + 2] * pixels[offset + 2]) >> 8;

                }
                break;

                case BLEND_MODE_SCREEN:
                {
                    drawImageAlphaBlending(pixels, offset, layer->getAlpha());
                    _imgPixels[offset] = 255 - (((255 - _imgPixels[offset]) * (255 - pixels[offset])) >> 8);
                    _imgPixels[offset + 1] = 255 - (((255 - _imgPixels[offset + 1]) * (255 - pixels[offset + 1])) >> 8);
                    _imgPixels[offset + 2] = 255 - (((255 - _imgPixels[offset + 2]) * (255 - pixels[offset + 2])) >> 8);
                }
                break;

                case BLEND_MODE_DIFFERENCE:
                {
                    drawImageAlphaBlending(pixels, offset, layer->getAlpha());
                    _imgPixels[offset] = abs(_imgPixels[offset] - pixels[offset]);
                    _imgPixels[offset + 1] = abs(_imgPixels[offset + 1] - pixels[offset + 1]);
                    _imgPixels[offset + 2] = abs(_imgPixels[offset + 2] - pixels[offset + 2]);
                }
                break;

                case BLEND_MODE_NEGATION:
                {
                    drawImageAlphaBlending(pixels, offset, layer->getAlpha());
                    _imgPixels[offset] = 255 - abs(255 - _imgPixels[offset] - pixels[offset]);
                    _imgPixels[offset + 1] = 255 - abs(255 - _imgPixels[offset + 1] - pixels[offset + 1]);
                    _imgPixels[offset + 2] = 255 - abs(255 - _imgPixels[offset + 2] - pixels[offset + 2]);
                }
                break;

                default:
                {
                    _imgPixels[offset] = pixels[offset];
                    _imgPixels[offset + 1] = pixels[offset + 1];
                    _imgPixels[offset + 2] = pixels[offset + 2];
                }
                break;
            }
        }
    }
}

void Canvas::clear()
{
    int width = _imgBounds.width();
    for (int y = 0; y < _imgBounds.height(); y++) {
        for (int x = 0; x < width; x++) {
            int offset = width * 4 * y + 4 * x;
            _imgPixels[offset] = 255;
            _imgPixels[offset + 1] = 255;
            _imgPixels[offset + 2] = 255;
            _imgPixels[offset + 3] = 255;
        }
    }
}
