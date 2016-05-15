#include "display.h"

Display::Display(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(500, 500);

    this->presscount = 0;

    this->button = new QPushButton();
    this->button->setParent(this);
    this->button->setText("text");
    this->button->setGeometry(50, 200, 400, 100);
    this->button->setCheckable(true);

    QProgressBar *bar = new QProgressBar();
    bar->setParent(this);
    bar->setRange(0, 100);
    bar->setValue(0);

    QSlider *slider = new QSlider();
    slider->setParent(this);
    slider->setRange(0, 100);
    slider->setValue(0);

    connect(slider, SIGNAL(valueChanged(int)), bar, SLOT(setValue(int)));
    connect(this->button, SIGNAL(clicked(bool)), this, SLOT(slotButtonClicked(bool)));
    connect(this, SIGNAL(maxPressCountReached()), QApplication::instance(), SLOT(quit()));
}

void Display::slotButtonClicked(bool checked)
{
    if(checked) this->button->setText("No one expects the spanish inquisition.");
    else this->button->setText("text");

    this->presscount++;

    if(this->presscount == 10) emit maxPressCountReached();
}
