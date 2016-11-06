#include "editor/vscrollbar.h"


_t::editor::vscrollbar::vscrollbar()
    : _t::editor::scrollbar()
{
    this->setFixedWidth(15);

    this->slider.resize(15, this->height() / 4);
}

void _t::editor::vscrollbar::scroll(qreal shift)
{
    if (shift < 0)
    {
        shift = 0;
    }
    else if (shift > 1)
    {
        shift = 1;
    }

    this->slider.move(0, (this->height() - this->slider.height()) * shift);

    emit this->scroll_event(shift);
}


void _t::editor::vscrollbar::resizeEvent(QResizeEvent *)
{
    this->slider.resize(15, this->height() / 4);
}

void _t::editor::vscrollbar::init_coords(QPoint coords)
{
    this->slider_original_coord = this->slider.y();
    this->slider_press_coord = coords.y();
}


void _t::editor::vscrollbar::slider_moved(QMouseEvent *event)
{
    if (this->scrolling)
    {
        qint32 target_y = this->slider_original_coord + event->globalY()
            - this->slider_press_coord;

        if (target_y < 0)
        {
            target_y = 0;
        }
        else if (target_y >= this->height() - this->slider.height())
        {
            target_y = this->height() - this->slider.height();
        }

        if (this->slider.y() != target_y)
        {
            this->slider.move(0, target_y);

            emit this->scroll_event(
                qreal(this->slider.y())
                / (this->height() - this->slider.height()));
        }
    }
}
