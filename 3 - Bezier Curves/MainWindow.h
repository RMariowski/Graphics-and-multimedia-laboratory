#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include "PixelPainter.h"
#include "Marker.h"

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
    void addMarker(QPoint* point, int size);

    double getDistanceBetweenPoints(int x1, int y1, int x2, int y2);

private slots:
    void on_btnClear_clicked();

    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent*);

private:
    Ui::MainWindow* _ui;
    Canvas* _canvas;
    PixelPainter* _pixelPainter;

    QList<QPoint> _clicks;
    QList<Marker> _markers;
    Marker* _selectedMarker;
};

#endif // MAINWINDOW_H
