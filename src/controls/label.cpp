#include "controls/label.h"


void _t::controls::label::mousePressEvent(QMouseEvent *event)
{
    emit this->mouse_press_event(event);
}

void _t::controls::label::mouseReleaseEvent(QMouseEvent *event)
{
    emit this->mouse_release_event(event);
}

void _t::controls::label::mouseMoveEvent(QMouseEvent *event)
{
    emit this->mouse_move_event(event);
}
