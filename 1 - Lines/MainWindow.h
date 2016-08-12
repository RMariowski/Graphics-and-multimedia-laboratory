#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QImage>
#include <QMouseEvent>
#include <QRect>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

private:
    void drawPixel(int x, int y, int r, int g, int b);

    void drawLine(int startX, int startY, int endX, int endY);

private slots:
    void on_btnClear_clicked();

    void mousePressEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent*);

private:
    Ui::MainWindow* _ui;

    QImage* _img;
    unsigned char* _imgPixels;
    QRect _imgBounds;

    QPoint _pressPos;
};

#endif // MAINWINDOW_H
