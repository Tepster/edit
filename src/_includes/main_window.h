#ifndef _T_MAIN_WINDOW_H
#define _T_MAIN_WINDOW_H

#include <QWidget>

#include <QMenuBar>
#include <QMenu>

#include <QString>

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

    /**
     * Path of the opened file.
     *
     * @var QString file_path
     */
    QString file_path = 0;


    /**
     * Saves the file to specific location.
     *
     * @param const QString & location
     */
    void save(const QString &location);

    /**
     * Asks user if file should be saved and saves it, if desired.
     *
     * @return bool  False if user chose to cancel the action.
     */
    bool save_if_desired();

    /**
     * Checks if the document has been modified.
     *
     * @return bool
     */
    bool document_modified();

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
