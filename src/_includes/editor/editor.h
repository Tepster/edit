#ifndef _T_EDITOR_EDITOR_H
#define _T_EDITOR_EDITOR_H

#include <functional>

#include <QWidget>
#include <QPixmap>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QFocusEvent>

#include <QTimer>

#include <QChar>
#include <QString>
#include <QStringList>

#include <QColor>
#include <QSize>

#include "controls.h"

#include "editor/coordinates.h"
#include "editor/cursor.h"
#include "editor/drawing_manager.h"


namespace _t
{
    namespace editor
    {
        class editor;
    }
}

/**
 * The _t::editor::editor class representing one editor control
 * with specific text inside its area.
 */
class _t::editor::editor : public QWidget
{
    Q_OBJECT


    /**
     * The main QLabel-based element to which the canvas is associated.
     *
     * @var _t::controls::label area
     */
    controls::label area;

    /**
     * Tha canvas for painting the text.
     *
     * @var QPixmap canvas
     */
    QPixmap canvas;

    /**
     * The drawing manager that paints on the canvas.
     *
     * @var _t::editor::drawing_manager painter
     */
    _t::editor::drawing_manager painter;


    /**
     * The background color for selected cells.
     *
     * @var QColor selection_background
     */
    QColor selection_background;

    /**
     * @var QSize cell_size
     */
    QSize cell_size;


    /**
     * @var _t::editor::cursor _cursor
     */
    _t::editor::cursor _cursor;

    /**
     * Timer for cursor blinking.
     *
     * @var QTimer cursor_timer
     */
    QTimer cursor_timer;

    /**
     * States if cursor, while blinking, is currently visible or not.
     *
     * @var bool cursor_visible
     */
    bool cursor_visible = false;


    /**
     * The actual text in the editor, saved in list for easy manipulation
     * with line lengths.
     *
     * @var QStringList text
     */
    QStringList text;


    /**
     * @var QChar newline_character
     */
    QChar newline_character = '\n';



    /**
     * Handles any user input from keyboard.
     *
     * @param QKeyEvent * event
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * Handles focus-in event.
     *
     * @param QFocusEvent * event
     */
    void focusInEvent(QFocusEvent *event);

    /**
     * Handles focus-out event.
     *
     * @param QFocusEvent * event
     */
    void focusOutEvent(QFocusEvent *event);



    /**
     * Easy way to get line where the cursor is currently placed.
     *
     * @return QString &  Current line ref.
     */
    QString &active_line();



    /**
     * Deselect all selected cells.
     */
    void deselect();


    /**
     * Deletes the character at specified coordinates.
     *
     * @param const _t::editor::coordinates & coords
     */
    void delete_char(const coordinates &coords);


    /**
     * Apply selected-cell style to the cell.
     *
     * @param const _t::editor::coordinates & coords
     */
    void draw_selected_cell(const coordinates &coords);

    /**
     * Apply normal cell style to the cell.
     *
     * @param const _t::editor::coordinates & coords
     */
    void draw_deselected_cell(const coordinates &coords);


    /**
     * Displays the editor canvas on the screen.
     */
    void update();


    /**
     * Places the cursor to specified coordinates.
     *
     * @param _t::editor::coordinates & coords
     * @param bool                      selection
     */
    void cursor_move(const coordinates &coords, bool selection = false);


    /**
     * Displays the cursor on the editor canvas.
     */
    void cursor_show();

    /**
     * Hides the cursor from the editor canvas.
     */
    void cursor_hide();

    /**
     * Activates the cursor blinking.
     */
    void cursor_activate();

    /**
     * Deactivates the cursor blinking.
     */
    void cursor_deactivate();


    /**
     * Applies a function to each cell of the coordinates range.
     * Range coordinates must be in ascending order.
     *
     * @param const _t::editor::coordinates                              & start
     * @param const _t::editor::coordinates                              & end
     * @param       std::function<void(const _t::editor::coordinates &)>   func
     */
    void each_cell(
        const coordinates &start,
        const coordinates &end,
        std::function<void(const coordinates &)> func);

private slots:
    /**
     * Handles any user input from mouse buttons.
     *
     * @param QMouseEvent * event
     */
    void mouse_press(QMouseEvent *event);

    /**
     * Handles mouse movement.
     *
     * @param QMouseEvent * event
     */
    void mouse_move(QMouseEvent *event);


    /**
     * Handles showing/hiding cursor when blinking.
     */
    void cursor_timer_tick();

public:
    /**
     * Initializes the editor with default values, attaches self to the parent.
     *
     * @param QWidget * parent_widget
     */
    void init(QWidget *parent_widget);


    /**
     * Writes the text into editor's memory and displays it on the screen.
     *
     * @param const QString & text
     */
    void write(const QString &text);
};

#endif // _T_EDITOR_EDITOR_H
