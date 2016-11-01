#ifndef _T_MAIN_WINDOW_H
#define _T_MAIN_WINDOW_H

#include <QWidget>

#include "editor/editor.h"


namespace _t
{
    class main_window;
}

/**
 * The _t::main_window class representing the main window of the application.
 */
class _t::main_window : public QWidget
{
    Q_OBJECT


    /**
     * The editor control.
     *
     * @var _t::editor::editor editor
     */
    _t::editor::editor editor;

public:
    /**
     * @param QWidget * parent
     */
    explicit main_window(QWidget *parent = 0);
};

#endif // _T_MAIN_WINDOW_H
