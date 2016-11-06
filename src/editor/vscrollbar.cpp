#include "editor/vscrollbar.h"


_t::editor::vscrollbar::vscrollbar(qint32 &shift)
    : _t::editor::scrollbar(shift)
{
    this->setFixedWidth(15);

    this->refresh();
}

void _t::editor::vscrollbar::scroll(qreal shift)
{
    if (shift < 0)
    {
        shift = 0;
    }
    else if (shift > this->area_size - this->height())
    {
        shift = this->area_size - this->height();
    }

    if (shift != this->shift)
    {
        this->shift = shift;

        this->refresh();

        emit this->scroll_event();
    }
}


void _t::editor::vscrollbar::resizeEvent(QResizeEvent *)
{
    this->refresh();
}

void _t::editor::vscrollbar::init_coords(QPoint coords)
{
    this->slider_original_coord = this->slider.y();
    this->slider_press_coord = coords.y();
}

void _t::editor::vscrollbar::refresh()
{
    qint32 slider_height;
    qint32 slider_y;

    if (this->area_size - this->height())
    {
        slider_height = std::pow(this->height(), 2) / this->area_size;
        slider_y = ((this->height() - slider_height) * this->shift)
            / (this->area_size - this->height());
    }
    else
    {
        slider_height = this->area_size;
        slider_y = 0;
    }

    this->slider.resize(this->slider.width(), slider_height);
    this->slider.move(0, slider_y);
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
            this->shift = target_y * (this->area_size - this->height())
                / (this->height() - this->slider.height());

            this->refresh();

            emit this->scroll_event();
        }
    }
}
