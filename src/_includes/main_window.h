#ifndef _T_MAIN_WINDOW_H
#define _T_MAIN_WINDOW_H

#include <QWidget>

#include <QMenuBar>
#include <QMenu>

#include <QCloseEvent>

#include <QString>

#include "editor/editor.h"
#include "editor/sh_rules.h"


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
     * @var QMenu sh_menu
     */
    QMenu sh_menu;


    /**
     * The editor control.
     *
     * @var _t::editor::editor * editor
     */
    _t::editor::editor *editor;

    /**
     * @var _t::editor::sh_rules sh_rules
     */
    _t::editor::sh_rules sh_rules;

    /**
     * Path of the opened file.
     *
     * @var QString file_path
     */
    QString file_path;


    /**
     * Performs save check before closing.
     *
     * @param QCloseEvent * event
     */
    void closeEvent(QCloseEvent *event);


    /**
     * Saves the file to specific location.
     *
     * @param const QString & location
     */
    void save(const QString &location);

    /**
     * Saves the document, if modified and user wants to save it.
     *
     * @return bool  False if user canceled the action.
     */
    bool check_save();

    /**
     * Asks user for the location where the document should be saved.
     *
     * @return QString
     */
    QString ask_save_location();

    /**
     * Checks if the document has been modified.
     *
     * @return bool
     */
    bool document_modified() const;

private slots:
    /**
     * Initiates new file.
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


    /**
     * Highlights for C++.
     */
    void menu_sh_cpp();

    /**
     * Highlights for C#.
     */
    void menu_sh_csharp();

    /**
     * Highlights for PHP.
     */
    void menu_sh_php();

    /**
     * Highlights for JavaScript.
     */
    void menu_sh_javascript();

    /**
     * Highlights for HTML.
     */
    void menu_sh_html();

    /**
     * Highlights for CSS.
     */
    void menu_sh_css();

public:
    /**
     * @param QWidget * parent
     */
    explicit main_window(QWidget *parent = 0);
};

#endif // _T_MAIN_WINDOW_H
