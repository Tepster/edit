#ifndef _T_CONTROLS_LABEL_H
#define _T_CONTROLS_LABEL_H

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
     * @param QMouseEvent * event
     */
    void mousePressEvent(QMouseEvent *event);

    /**
     * @param QMouseEvent * event
     */
    void mouseReleaseEvent(QMouseEvent *event);

    /**
     * @param QMouseEvent * event
     */
    void mouseMoveEvent(QMouseEvent *event);

signals:
    /**
     * @param QMouseEvent * event
     */
    void mouse_press_event(QMouseEvent *event);

    /**
     * @param QMouseEvent * event
     */
    void mouse_release_event(QMouseEvent *event);

    /**
     * @param QMouseEvent * event
     */
    void mouse_move_event(QMouseEvent *event);
};

#endif // _T_CONTROLS_LABEL_H
