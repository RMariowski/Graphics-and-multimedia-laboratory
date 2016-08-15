#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <cmath>
#include <vector>
#include <QStack>

#define PI 3.14159265358979323846

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    _imgBounds.setX(_ui->frmCanvas->x());
    _imgBounds.setY(_ui->frmCanvas->y());
    _imgBounds.setWidth(_ui->frmCanvas->width());
    _imgBounds.setHeight(_ui->frmCanvas->height());

    _img = new QImage(_imgBounds.width(), _imgBounds.height(), QImage::Format_RGB32);
    _imgPixels = _img->bits();

    _pressPos = QPoint(-1, -1);

    _drawMode = DRAW_MODE_LINE;

    on_btnClear_clicked();
}

MainWindow::~MainWindow()
{
    delete _img;
    delete _ui;
}

void MainWindow::drawPixel(int x, int y, int r, int g, int b)
{
    int width = _imgBounds.width();
    _imgPixels[width * 4 * y + 4 * x] = b;
    _imgPixels[width * 4 * y + 4 * x + 1] = g;
    _imgPixels[width * 4 * y + 4 * x + 2] = r;
}

void MainWindow::drawLine(int startX, int startY, int endX, int endY)
{
    double a = (startY - static_cast<double>(endY)) / (startX - static_cast<double>(endX));
    double b = startY - a * startX;

    if (startX > endX)
        std::swap(startX, endX);

    if (startY > endY)
        std::swap(startY, endY);

    int x = startX;
    int y = startY;

    if (startX == endX)
    {
        for (; y <= endY; y++)
            drawPixel(startX, y, 0, 0, 0);
    }
    else if (startY == endY)
    {
        for (; x <= endX; x++)
            drawPixel(x, startY);
    }
    else
    {
        for (; x <= endX; x++)
        {
            int y = round(a * x + b);
            drawPixel(x, y, 0, 0, 0);
        }

        for (; y <= endY; y++)
        {
            int x = round((y - b) / a);
            drawPixel(x, y, 0, 0, 0);
        }
    }

    update();
}

void MainWindow::drawCircle(int x, int y, double r)
{
    for (double rad = 0; rad <= 2 * PI; rad += 0.001)
    {
        int xO = static_cast<int>(r * cos(rad));
        int yO = static_cast<int>(r * sin(rad));
        drawPixel(x + xO, y + yO, 0, 0, 0);
    }

    update();
}

void MainWindow::drawEllipse(int x, int y, double a, double b)
{
    for (double rad = 0; rad <= 2 * PI; rad += 0.001)
    {
        int xO = static_cast<int>(a * cos(rad));
        int yO = static_cast<int>(b * sin(rad));
        drawPixel(x + xO, y + yO, 0, 0, 0);
    }

    update();
}

void MainWindow::drawPolygon(int x, int y, double a, double b)
{
    int w = _ui->spnVertices->value();
    double distanceBetweenVertices = 2 * PI / static_cast<double>(w);

    std::vector<QPoint> vertices;

    for (double rad = 0.0; w > 0; w--, rad += distanceBetweenVertices)
    {
        int xO = static_cast<int>(a * cos(rad));
        int yO = static_cast<int>(b * sin(rad));
        vertices.push_back(QPoint(x + xO, y + yO));
    }

    for (size_t i = 0; i < vertices.size(); i++)
    {
        QPoint p1 = vertices[i];
        QPoint p2;

        if (i + 1 >= vertices.size())
        {
            p2 = vertices[0];
        }
        else
        {
            p2 = vertices[i + 1];
        }

        drawLine(p1.x(), p1.y(), p2.x(), p2.y());
    }

    update();
}

void MainWindow::floodFill(int x, int y, QColor oldColor, QColor newColor)
{
    QStack<QPoint> stack;

    if (_img->pixelColor(QPoint(x, y)) != oldColor)
        return;

    stack.push(QPoint(x, y));

    while (!stack.isEmpty())
    {
        QPoint p = stack.pop();
        x = p.x();
        y = p.y();

        if (_img->pixelColor(QPoint(x, y)) == oldColor)
        {
            int w = p.x();
            int e = p.x();

            while (_img->pixelColor(QPoint(w, y)) == oldColor)
            {
                w--;
                if (w < 0)
                {
                    w = 0;
                    break;
                }
            }

            while (_img->pixelColor(QPoint(e, y)) == oldColor)
            {
                e++;
                if (e >= _imgBounds.width())
                {
                    e = _imgBounds.width() - 1;
                    break;
                }
            }

            for (int i = w + 1; i < e; i++)
            {
                drawPixel(i, y, newColor.red(), newColor.green(),
                          newColor.blue());
            }

            for (int i = w + 1; i < e; i++)
            {
                if (y + 1 < _imgBounds.height() &&
                    _img->pixelColor(QPoint(i, y + 1)) == oldColor)
                {
                    stack.push(QPoint(i, y + 1));
                }

                if (y -1 >= 0 &&
                    _img->pixelColor(QPoint(i, y - 1)) == oldColor)
                {
                    stack.push(QPoint(i, y - 1));
                }
            }
        }
    }

    update();
}

double MainWindow::getDistanceBetweenPoints(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void MainWindow::on_btnClear_clicked()
{
    for (int y = 0; y < _imgBounds.height(); y++)
    {
        for (int x = 0; x < _imgBounds.width(); x++)
            drawPixel(x, y, 255, 255, 255);
    }
    update();
}

void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.drawImage(_imgBounds.x(), _imgBounds.y(), *_img);
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    _pressPos.setX(event->x() - _imgBounds.x());
    _pressPos.setY(event->y() - _imgBounds.y());

    if (_pressPos.x() < 0 || _pressPos.y() < 0 || _pressPos.x() >= _imgBounds.width() || _pressPos.y() >= _imgBounds.height())
        _pressPos = QPoint(-1, -1);
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;

    if (_pressPos.x() < 0 || _pressPos.y() < 0)
        return;

    int x = event->x() - _imgBounds.x();
    int y = event->y() - _imgBounds.y();

    switch (_drawMode)
    {
        case DRAW_MODE_LINE:
        {
            if (x < _imgBounds.width() && y < _imgBounds.height())
                drawLine(_pressPos.x(), _pressPos.y(), x, y);
        }
        break;

        case DRAW_MODE_CIRCLE:
        {
            double r = getDistanceBetweenPoints(_pressPos.x(), _pressPos.y(), x, y);
            if (_pressPos.x() - r >= 0 && _pressPos.y() - r >= 0 &&
                _pressPos.x() + r < _imgBounds.width() && _pressPos.y() + r < _imgBounds.height())
            {
                drawCircle(_pressPos.x(), _pressPos.y(), r);
            }
        }
        break;

        case DRAW_MODE_ELLIPSE:
        case DRAW_MODE_POLYGON:
        {
            double a = getDistanceBetweenPoints(_pressPos.x(), _pressPos.y(), x, _pressPos.y());
            double b = getDistanceBetweenPoints(_pressPos.x(), _pressPos.y(), _pressPos.x(), y);

            if (_pressPos.x() - a >= 0 && _pressPos.y() - b >= 0 &&
                _pressPos.x() + a < _imgBounds.width() && _pressPos.y() + b < _imgBounds.height())
            {
                if (_drawMode == DRAW_MODE_ELLIPSE)
                {
                    drawEllipse(_pressPos.x(), _pressPos.y(), a, b);
                }
                else
                {
                    drawPolygon(_pressPos.x(), _pressPos.y(), a, b);
                }
            }
        }
        break;

        case DRAW_MODE_FLOOD_FILL:
        {
            if (x < _imgBounds.width() && y < _imgBounds.height())
                floodFill(x, y, _img->pixelColor(QPoint(x, y)),
                          QColor(_ui->sldR->value(), _ui->sldG->value(), _ui->sldB->value()));
        }
        break;
    }
}

void MainWindow::on_btnLine_clicked()
{
    _drawMode = DRAW_MODE_LINE;
}

void MainWindow::on_btnCircle_clicked()
{
    _drawMode = DRAW_MODE_CIRCLE;
}

void MainWindow::on_btnEllipse_clicked()
{
    _drawMode = DRAW_MODE_ELLIPSE;
}

void MainWindow::on_btnPolygon_clicked()
{
    _drawMode = DRAW_MODE_POLYGON;
}

void MainWindow::on_btnFloodFill_clicked()
{
    _drawMode = DRAW_MODE_FLOOD_FILL;
}
