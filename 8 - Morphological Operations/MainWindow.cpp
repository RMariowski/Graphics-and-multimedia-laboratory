#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                        _ui->frmCanvas->width(), _ui->frmCanvas->height());

    _markerSize = 3;
}

MainWindow::~MainWindow()
{
    delete _canvas;
    delete _ui;
}

void MainWindow::paintEvent(QPaintEvent*)
{
    _canvas->draw();
}

void MainWindow::on_btnReset_clicked()
{
    _canvas->reset();
    update();
}

void MainWindow::on_btnDilatation_clicked()
{
    _canvas->dilatation(_markerSize);
    update();
}

void MainWindow::on_btnErosion_clicked()
{
    _canvas->erosion(_markerSize);
    update();
}

void MainWindow::on_btnOpening_clicked()
{
    _canvas->opening(_markerSize);
    update();
}

void MainWindow::on_btnClosing_clicked()
{
    _canvas->closing(_markerSize);
    update();
}

void MainWindow::on_sldSize_valueChanged(int value)
{
    _markerSize = value;
    char buffer[12];
    sprintf(buffer, "%dx%d", value, value);
    _ui->lblMarker->setText(buffer);
}
