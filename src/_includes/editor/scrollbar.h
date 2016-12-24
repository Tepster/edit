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

/**
 * The _t::editor::scrollbar abstract class as a base for vertical
 * and horizontal scrollbars.
 *
 * - it should behave like a standalone control
 * - it stores the shift into the given reference, because it is also needed
 *   elsewhere
 * - slider size and position is being deterined by the shift,
 *   scrollbar size and the area_size, which is updated via the method calling
 *   from the parent editor
 * - when scrolls, it fires the event so parent editor can redraw
 *   according to the new shift
 */
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
     * Paints the scrollbar color style.
     *
     * @param QPaintEvent * event
     */
    void paintEvent(QPaintEvent *event);


    /**
     * Initializes the coordinates values before slider-scrolling.
     *
     * @param QPoint coords
     */
    virtual void scroll_init_coords(QPoint coords) = 0;

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
     * Changes the local area_size field and refreshes.
     *
     * @param qint32
     */
    virtual void area_size_changed(qint32) = 0;

public:
    /**
     * The main constructor.
     *
     * @param qint32 & shift
     */
    scrollbar(qint32 &shift);


    /**
     * Validates and moves the slider to position specified by shift
     * and fires the scroll_event.
     *
     * @param qint32 shift
     * @return bool  Whether the scrolling actually occured.
     */
    virtual bool scroll(qint32 shift) = 0;

signals:
    /**
     * Event fired after slider-scroll or manual-scroll.
     */
    void scroll_event();
};

#endif // _T_EDITOR_SCROLLBAR_H
