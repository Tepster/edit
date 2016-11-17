#ifndef _T_MAIN_WINDOW_H
#define _T_MAIN_WINDOW_H

#include <QWidget>

#include <QMenuBar>
#include <QMenu>

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
     * The main navigation.
     *
     * @var QMenuBar menubar
     */
    QMenuBar menubar;

    /**
     * @var QMenu file_menu
     */
    QMenu file_menu;


    /**
     * The editor control.
     *
     * @var _t::editor::editor editor
     */
    _t::editor::editor editor;

private slots:
    /**
     * Initiates new file
     */
    void menu_file_new();

    /**
     * Opens the file.
     */
    void menu_file_open();

    /**
     * Saves the file.
     */
    void menu_file_save();

    /**
     * Saves the file to location specified by user.
     */
    void menu_file_save_as();

    /**
     * Quits the application.
     */
    void menu_file_quit();

public:
    /**
     * @param QWidget * parent
     */
    explicit main_window(QWidget *parent = 0);
};

#endif // _T_MAIN_WINDOW_H
