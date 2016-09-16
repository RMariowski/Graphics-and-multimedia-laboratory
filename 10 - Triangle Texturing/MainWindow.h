#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

#include "Canvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

    ~MainWindow();

    void drawAllForCanvas();

    void drawAllForCanvas2();

private slots:
    void mousePressEvent(QMouseEvent* event);

    void mouseMoveEvent(QMouseEvent* event);

    void mouseReleaseEvent(QMouseEvent* event);

    void paintEvent(QPaintEvent*);

    void on_chkVisibility_toggled(bool checked);

private:
    Ui::MainWindow* _ui;

    Canvas* _canvas;
    QList<Marker> _markers;

    Canvas* _canvas2;
    QList<Marker> _markers2;

    QImage* _image;
    Canvas* _activeCanvas;
    Marker* _selectedMarker;
};

#endif // MAINWINDOW_H
