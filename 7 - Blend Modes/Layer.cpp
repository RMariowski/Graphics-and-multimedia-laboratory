#include "Layer.h"
#include "MainWindow.h"

#include <QHBoxLayout>
#include <QCheckBox>
#include <QSlider>

Layer::Layer(QImage* image, bool visible, int alpha)
    : _image(image)
{
    QHBoxLayout* layout = new QHBoxLayout();

    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(visible);
    QObject::connect(checkBox, &QCheckBox::toggled,
                     this, &Layer::toggled);

    QSlider* slider = new QSlider();
    slider->setOrientation(Qt::Horizontal);
    slider->setMinimum(0);
    slider->setMaximum(255);
    slider->setMinimumWidth(240);
    slider->setValue(alpha);
    QObject::connect(slider, &QSlider::valueChanged,
                     this, &Layer::valueChanged);

    layout->addWidget(checkBox);
    layout->addWidget(slider);
    layout->addStretch();

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);

    _visible = checkBox->isChecked();
    _alpha = slider->value();
}

void Layer::toggled(bool checked)
{
    _visible = checked;
    MainWindow::canvas->update();
}

void Layer::valueChanged(int value)
{
    _alpha = value;

    MainWindow::canvas->update();
}
