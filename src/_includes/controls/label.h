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
    void mouse_pressed(QMouseEvent *event);

    /**
     * @param QMouseEvent * event
     */
    void mouse_released(QMouseEvent *event);

    /**
     * @param QMouseEvent * event
     */
    void mouse_moved(QMouseEvent *event);
};

#endif // _T_CONTROLS_LABEL_H
