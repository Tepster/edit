#include "controls/label.h"


void _t::controls::label::mousePressEvent(QMouseEvent *event)
{
    emit this->mouse_pressed(event);
}

void _t::controls::label::mouseReleaseEvent(QMouseEvent *event)
{
    emit this->mouse_released(event);
}

void _t::controls::label::mouseMoveEvent(QMouseEvent *event)
{
    emit this->mouse_moved(event);
}
