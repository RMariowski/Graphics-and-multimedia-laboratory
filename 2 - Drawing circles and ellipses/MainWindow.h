#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    enum DrawMode
    {
        DRAW_MODE_LINE,
        DRAW_MODE_CIRCLE,
        DRAW_MODE_ELLIPSE,
        DRAW_MODE_POLYGON
    };

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

private:
    void drawPixel(int x, int y, int r, int g, int b);

    void drawLine(int x1, int y1, int endX, int endY);

    void drawCircle(int x, int y, double r);

    void drawEllipse(int x, int y, double a, double b);

    void drawPolygon(int x, int y, double a, double b);

    double getDistanceBetweenPoints(int x1, int y1, int x2, int y2);

private slots:
    void on_btnClear_clicked();

    void mousePressEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent*);

    void on_btnLine_clicked();

    void on_btnCircle_clicked();

    void on_btnEllipse_clicked();

    void on_btnPolygon_clicked();

private:
    Ui::MainWindow* _ui;

    QImage* _img;
    unsigned char* _imgPixels;
    QRect _imgBounds;

    QPoint _pressPos;

    DrawMode _drawMode;
};

#endif // MAINWINDOW_H
