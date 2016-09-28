#ifndef _T_CONTROLS_H
#define _T_CONTROLS_H

#include <QLabel>
#include <QPoint>

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

    void mousePressEvent(QMouseEvent *event);

signals:
    void clicked(QPoint coords);
};

#endif // _T_CONTROLS_H
