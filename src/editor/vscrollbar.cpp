#include "editor/vscrollbar.h"


_t::editor::vscrollbar::vscrollbar(qint32 &shift)
    : _t::editor::scrollbar(shift)
{
    this->setFixedWidth(15);

    this->refresh();
}


bool _t::editor::vscrollbar::scroll(qint32 shift)
{
    // should scroll to negative OR text area height smaller than the editor
    if (shift < 0 || this->area_size <= this->height())
    {
        shift = 0;
    }

    // end of the text area ends above the end of the editor
    else if (shift + this->height() > this->area_size)
    {
        // set shift so the text area ends same as the editor
        shift = this->area_size - this->height();
    }

    if (shift != this->shift)
    {
        this->shift = shift;

        this->refresh();

        emit this->scroll_event();

        return true;
    }

    return false;
}


void _t::editor::vscrollbar::area_size_changed(qint32 height)
{
    // only proceed if there is actually a change
    if (this->area_size != height)
    {
        this->area_size = height;
        this->correct_scroll();

        this->refresh();
    }
}


void _t::editor::vscrollbar::resizeEvent(QResizeEvent *event)
{
    // the size changed vertically
    if (event->size().height() != event->oldSize().height())
    {
        this->correct_scroll();
        this->refresh();
    }
}


void _t::editor::vscrollbar::scroll_init_coords(QPoint coords)
{
    this->slider_original_coord = this->slider.y();
    this->slider_press_coord = coords.y();
}

void _t::editor::vscrollbar::correct_scroll()
{
    this->scroll(this->shift);
}

void _t::editor::vscrollbar::refresh()
{
    qint32 slider_height;
    qint32 slider_y;

    // text area is smaller than the editor - slider is full-size
    if (this->area_size <= this->height())
    {
        slider_height = this->height();
        slider_y = 0;
    }
    else
    {
        // no need for checking division by zero - is checked in condition above
        slider_height = std::pow(this->height(), 2) / this->area_size;

        // set minimal height of slider
        if (slider_height < this->height() / 10)
        {
            slider_height = this->height() / 10;
        }

        // no need for checking division by zero - is checked in condition above
        slider_y = ((this->height() - slider_height) * this->shift)
            / (this->area_size - this->height());
    }

    this->slider.resize(this->slider.width(), slider_height);
    this->slider.move(0, slider_y);
}


void _t::editor::vscrollbar::slider_moved(QMouseEvent *event)
{
    // in scrolling mode AND slider is not full-height
    if (this->scrolling && this->height() != this->slider.height())
    {
        qint32 target_y = this->slider_original_coord + event->globalY()
            - this->slider_press_coord;

        this->scroll(target_y * (this->area_size - this->height())
                / (this->height() - this->slider.height()));
    }
}
