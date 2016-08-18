#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Layer.h"

#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>

Canvas* MainWindow::canvas;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), _ui(new Ui::MainWindow)
{
    _ui->setupUi(this);

    canvas = new Canvas(this, _ui->frmCanvas->x(), _ui->frmCanvas->y(),
                        _ui->frmCanvas->width(), _ui->frmCanvas->height());

    _ui->lstLayers->setIconSize(QSize(75, 75));
    _ui->lstLayers->setResizeMode(QListWidget::Adjust);

    _ui->cbMode->addItem("None");
    _ui->cbMode->addItem("Alpha Blending");
    _ui->cbMode->addItem("Average Mode");
    _ui->cbMode->addItem("Multiply Mode");
    _ui->cbMode->addItem("Screen Mode");
    _ui->cbMode->addItem("Difference Mode");
    _ui->cbMode->addItem("Negation Mode");

    setAcceptDrops(true);

    _blendMode = BLEND_MODE_NONE;
}

MainWindow::~MainWindow()
{
    for (int i = 0; i < _ui->lstLayers->count(); ++i)
    {
        QListWidgetItem* item = _ui->lstLayers->item(i);
        Layer* layer = static_cast<Layer*>(_ui->lstLayers->itemWidget(item));
        if (layer != 0)
            delete layer->getImage();
    }
    delete _ui;
}

void MainWindow::loadAndAddImage(const QString& fileName)
{
    QImage* image = new QImage(fileName);
    if (image->isNull()) {
        delete image;
        return;
    }

    QListWidgetItem* widgetItem = new QListWidgetItem();
    widgetItem->setIcon(QIcon(fileName));
    _ui->lstLayers->addItem(widgetItem);

    Layer* layer = new Layer(image);
    widgetItem->setSizeHint(layer->sizeHint());
    _ui->lstLayers->setItemWidget(widgetItem, layer);

    canvas->update();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    for (int i = 0; i < urls.size(); i++)
        loadAndAddImage(urls.at(i).toLocalFile());
}

void MainWindow::paintEvent(QPaintEvent*)
{
    canvas->clear();

    for (int i = 0; i < _ui->lstLayers->count(); ++i)
    {
        QListWidgetItem* item = _ui->lstLayers->item(i);
        Layer* layer = static_cast<Layer*>(_ui->lstLayers->itemWidget(item));

        if (layer != 0 && layer->isVisible())
            canvas->drawImage(layer, _blendMode, (i == 0));
    }

   if (_blendMode != BLEND_MODE_NONE)
        canvas->draw();
}

void MainWindow::on_cbMode_currentIndexChanged(const QString& itemID)
{
    if      (itemID == "None")              _blendMode = BLEND_MODE_NONE;
    else if (itemID == "Alpha Blending")    _blendMode = BLEND_MODE_ALPHA_BLENDING;
    else if (itemID == "Average Mode")      _blendMode = BLEND_MODE_AVERAGE;
    else if (itemID == "Multiply Mode")     _blendMode = BLEND_MODE_MULTIPLY;
    else if (itemID == "Screen Mode")       _blendMode = BLEND_MODE_SCREEN;
    else if (itemID == "Difference Mode")   _blendMode = BLEND_MODE_DIFFERENCE;
    else if (itemID == "Negation Mode")     _blendMode = BLEND_MODE_NEGATION;

    canvas->update();
}

void MainWindow::on_btnUpArrow_clicked()
{
    QListWidget* list = _ui->lstLayers;

    int currentIndex =  list->currentRow();
    if (currentIndex < 1)
        return;

    QListWidgetItem* currentItem =  list->item(currentIndex);
    Layer* oldLayer = static_cast<Layer*>(list->itemWidget(currentItem));
    Layer* newLayer = new Layer(oldLayer->getImage(), oldLayer->isVisible(), oldLayer->getAlpha());

    list->removeItemWidget(currentItem);
    list->takeItem(currentIndex);
    list->insertItem(currentIndex - 1, currentItem);
    list->setCurrentRow(currentIndex - 1);
    list->setItemWidget(currentItem, newLayer);

    canvas->update();
}

void MainWindow::on_btnDownArrow_clicked()
{
    QListWidget* list = _ui->lstLayers;

    int currentIndex =  list->currentRow();
    if (currentIndex >= list->count() - 1)
        return;

    QListWidgetItem* currentItem =  list->item(currentIndex);
    Layer* oldLayer = static_cast<Layer*>(list->itemWidget(currentItem));
    Layer* newLayer = new Layer(oldLayer->getImage(), oldLayer->isVisible(), oldLayer->getAlpha());

    list->removeItemWidget(currentItem);
    list->takeItem(currentIndex);
    list->insertItem(currentIndex + 1, currentItem);
    list->setCurrentRow(currentIndex + 1);
    list->setItemWidget(currentItem, newLayer);

    canvas->update();
}

void MainWindow::on_btnRemove_clicked()
{
    QListWidget* list = _ui->lstLayers;
    int currentIndex =  list->currentRow();
    if (currentIndex < 0 || currentIndex >= list->count())
        return;

    list->takeItem(currentIndex);

    canvas->update();
}
