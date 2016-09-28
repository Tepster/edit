#include "main_window.h"


_t::main_window::main_window(QWidget *parent)
    : QWidget(parent)
{
    this->editor.init(this);
}
