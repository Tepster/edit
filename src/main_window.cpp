#include "main_window.h"

#include <QVBoxLayout>


_t::main_window::main_window(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(280, 160);
    this->resize(720, 360);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);

    layout->addWidget(&this->editor);

    this->setLayout(layout);
}
