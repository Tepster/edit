#ifndef _T_EDITOR_VSCROLLBAR_H
#define _T_EDITOR_VSCROLLBAR_H

#include "../editor.h"

#include <QMouseEvent>
#include <QResizeEvent>

#include <QPoint>

#include "scrollbar.h"


/**
 * The _t::editor::vscrollbar representing the vertical scrollbar of the editor.
 */
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
     * Initializes the y-coordinates before slider-scrolling.
     *
     * @param QPoint coords
     */
    void scroll_init_coords(QPoint coords) override;

    /**
     * Correct the shift within boundaries, scrolls eventually.
     */
    void correct_scroll();

    /**
     * Refreshes the size and position of the slider.
     */
    void refresh() override;

private slots:
    /**
     * Mouse moved on the slider.
     *
     * @param QMouseEvent * event
     */
    void slider_moved(QMouseEvent *event) override;

public slots:
    /**
     * Changes the local area_size field and refreshes.
     *
     * @param qint32 height
     */
    void area_size_changed(qint32 height) override;

public:
    /**
     * The main constructor.
     *
     * @param qint32 & shift
     */
    vscrollbar(qint32 &shift);


    /**
     * Moves the slider to position specified by shift.
     *
     * @param qint32 shift
     */
    void scroll(qint32 shift) override;
};

#endif // _T_EDITOR_VSCROLLBAR_H
