#include "editor/scrollbar.h"

#include <QPainter>

#include <QColor>


_t::editor::scrollbar::scrollbar(qint32 &shift)
    : shift(shift)
{
    this->slider.setParent(this);
    this->slider.setStyleSheet("background: rgb(35, 35, 45)");

    connect(
        &this->slider,
        SIGNAL(mouse_press_event(QMouseEvent *)),
        this,
        SLOT(slider_pressed(QMouseEvent *)));

    connect(
        &this->slider,
        SIGNAL(mouse_release_event(QMouseEvent *)),
        this,
        SLOT(slider_released(QMouseEvent *)));

    connect(
        &this->slider,
        SIGNAL(mouse_move_event(QMouseEvent *)),
        this,
        SLOT(slider_moved(QMouseEvent *)));
}


void _t::editor::scrollbar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.fillRect(this->rect(), QColor(50, 50, 60));
}


void _t::editor::scrollbar::slider_pressed(QMouseEvent *event)
{
    this->scrolling = true;

    this->scroll_init_coords(event->globalPos());
}

void _t::editor::scrollbar::slider_released(QMouseEvent *)
{
    this->scrolling = false;
}
