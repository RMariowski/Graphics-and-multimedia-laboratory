#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "Canvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    void loadAndAddImage(const QString& fileName);

    void dragEnterEvent(QDragEnterEvent* e);

    void dropEvent(QDropEvent* e);

    void paintEvent(QPaintEvent*);

public:
    static Canvas* canvas;

private slots:
    void on_cbMode_currentIndexChanged(const QString &arg1);

    void on_btnUpArrow_clicked();

    void on_btnDownArrow_clicked();

    void on_btnRemove_clicked();

private:
    Ui::MainWindow* _ui;
    BlendMode _blendMode;
};

#endif // MAINWINDOW_H
