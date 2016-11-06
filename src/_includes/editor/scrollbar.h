#ifndef _T_EDITOR_SCROLLBAR_H
#define _T_EDITOR_SCROLLBAR_H

#include <QWidget>

#include <QMouseEvent>
#include <QPaintEvent>

#include <QPoint>

#include "controls/label.h"


namespace _t
{
    namespace editor
    {
        class scrollbar;
    }
}

class _t::editor::scrollbar : public QWidget
{
    Q_OBJECT

protected:
    /**
     * @var _t::controls::label slider
     */
    _t::controls::label slider;


    /**
     * @var bool scrolling
     */
    bool scrolling = false;

    /**
     * @var qint32 slider_original_coord
     */
    qint32 slider_original_coord;

    /**
     * @var qint32 slider_press_coord
     */
    qint32 slider_press_coord;


    /**
     * @var qint32 & shift
     */
    qint32 &shift;

    /**
     * @var qint32 area_size
     */
    qint32 area_size;


    /**
     * Initializes the coordinates values.
     *
     * @param QPoint coords
     */
    virtual void init_coords(QPoint coords) = 0;

    /**
     * Paints the scrollbar color style.
     *
     * @param QPaintEvent * event
     */
    void paintEvent(QPaintEvent *event);


    /**
     * Refreshes the size and position of the slider.
     */
    virtual void refresh() = 0;

protected slots:
    /**
     * Mouse pressed on the slider.
     *
     * @param QMouseEvent * event
     */
    void slider_pressed(QMouseEvent *event);

    /**
     * Mouse released on the slider.
     *
     * @param QMouseEvent * event
     */
    void slider_released(QMouseEvent *event);

    /**
     * Mouse moved on the slider.
     *
     * @param QMouseEvent * event
     */
    virtual void slider_moved(QMouseEvent *event) = 0;

public slots:
    /**
     * Scrollarea size changed.
     *
     * @param qint32 size
     */
    void area_size_changed(qint32 size);

public:
    /**
     * Tha main constructor.
     */
    scrollbar(qint32 &shift);


    /**
     * Moves the slider to position specified by shift.
     *
     * @param qreal shift
     */
    virtual void scroll(qreal shift) = 0;

signals:
    /**
     * @param qreal shift
     */
    void scroll_event();
};

#endif // _T_EDITOR_SCROLLBAR_H
