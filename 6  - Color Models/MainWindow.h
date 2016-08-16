#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

    void drawPixel(int x, int y, int r, int g, int b);

    void HSV();

    QColor HSVtoRGB(double h, double s, double v);

private slots:
    void paintEvent(QPaintEvent*);

    void on_sldR_valueChanged(int value);

    void on_sldG_valueChanged(int value);

    void on_sldB_valueChanged(int value);

    void on_sldH_valueChanged(int value);

    void on_sldS_valueChanged(int value);

    void on_sldV_valueChanged(int value);

private:
    Ui::MainWindow* _ui;
    QImage* _image;
    unsigned char* _imgPixels;
    QRect _imgBounds;

    double _h;
    double _s;
    double _v;
};

#endif // MAINWINDOW_H
