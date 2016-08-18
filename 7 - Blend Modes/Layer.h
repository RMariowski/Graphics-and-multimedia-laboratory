#ifndef IMAGEITEM_H
#define IMAGEITEM_H

#include <QWidget>

class Layer : public QWidget
{
public:
    Layer(QImage* image, bool visible = true, int alpha = 255);

    bool isVisible() const {
        return _visible;
    }

    QImage* getImage() const {
        return _image;
    }

    QWidget* getWidget() const {
        return _widget;
    }

    int getAlpha() const {
        return _alpha;
    }

public slots:
    void toggled(bool checked);

    void valueChanged(int value);

private:
    QImage* _image;
    QWidget* _widget;
    bool _visible;
    int _alpha;
};

#endif // IMAGEITEM_H
