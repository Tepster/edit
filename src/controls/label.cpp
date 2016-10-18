#include "controls.h"

#include <QLabel>


void _t::controls::label::mousePressEvent(QMouseEvent *event)
{
    emit this->clicked(event);
}
