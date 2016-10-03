#ifndef _T_MAIN_WINDOW_H
#define _T_MAIN_WINDOW_H

#include <QWidget>

#include "editor.h"


namespace _t
{
    class main_window;
}

class _t::main_window : public QWidget
{
    Q_OBJECT

    _t::editor editor;

public:
    explicit main_window(QWidget *parent = 0);
};

#endif // _T_MAIN_WINDOW_H
