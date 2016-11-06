#ifndef _T_EDITOR_VSCROLLBAR_H
#define _T_EDITOR_VSCROLLBAR_H

#include <QMouseEvent>
#include <QResizeEvent>

#include <QPoint>

#include "scrollbar.h"


namespace _t
{
    namespace editor
    {
        class vscrollbar;
    }
}

class _t::editor::vscrollbar : public _t::editor::scrollbar
{
    Q_OBJECT


    /**
     * Resizes the slider accordingly.
     *
     * @param QResizeEvent * event
     */
    void resizeEvent(QResizeEvent *event);

    /**
     * Initializes the coordinates values.
     *
     * @param QPoint coords
     */
    void init_coords(QPoint coords);

public:
    /**
     * The main constructor.
     */
    vscrollbar();


    /**
     * Moves the slider to position specified by shift.
     *
     * @param qreal shift
     */
    void scroll(qreal shift);

private slots:
    /**
     * Mouse moved on the slider.
     *
     * @param QMouseEvent * event
     */
    void slider_moved(QMouseEvent *event);
};

#endif // _T_EDITOR_VSCROLLBAR_H
