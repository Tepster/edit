#ifndef _T_CONTROLS_H
#define _T_CONTROLS_H

#include <QLabel>

#include <QMouseEvent>


namespace _t
{
    namespace controls
    {
        class label;
    }
}

class _t::controls::label : public QLabel
{
    Q_OBJECT

    /**
     * Event for mouse press.
     *
     * @param QMouseEvent * event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * Event for mouse move.
     *
     * @param QMouseEvent * event
     */
    void mouseMoveEvent(QMouseEvent *event);

signals:
    /**
     * Signal for mouse press.
     *
     * @param QMouseEvent * event
     */
    void mouse_pressed(QMouseEvent *event);

    /**
     * Signal for mouse move.
     *
     * @param QMouseEvent * event
     */
    void mouse_moved(QMouseEvent *event);
};

#endif // _T_CONTROLS_H
