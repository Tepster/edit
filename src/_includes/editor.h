#ifndef _T_EDITOR_H
#define _T_EDITOR_H

#include <functional>

#include <QWidget>
#include <QPixmap>
#include <QPainter>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QFocusEvent>

#include <QTimer>

#include <QChar>
#include <QString>
#include <QStringList>

#include <QFont>
#include <QColor>
#include <QPoint>

#include "controls.h"


namespace _t
{
    class editor;
}

/**
 * The _t::editor class representing one editor control
 * with specific text inside its area.
 */
class _t::editor : public QWidget
{
    Q_OBJECT

    /**
     * The _t::editor::coordinates struct which stores the row and column values
     * and provide basic comparing functions.
     *
     * @note All methods implicitly validate the coordinates values.
     */
    struct coordinates
    {
        /**
         * Editor's text pointer for knowledge of line lengths.
         *
         * @var const QStringList * text
         */
        const QStringList *text = 0;


        /**
         * The actual row value as an x-coordinate.
         *
         * @var quint32 row
         */
        quint32 row = 0;

        /**
         * The actual column value as a y-coordinate.
         *
         * @var quint32 col
         */
        quint32 col = 0;


        /**
         * Default constructor.
         */
        coordinates() {}

        /**
         * Constructor that sets-up the text pointer.
         *
         * @param const QStringList * text
         */
        coordinates(const QStringList *text) : text(text) {}

        /**
         * Constructor that sets-up the coordinates values and text pointer.
         *
         * @param quint32             row
         * @param quint32             col
         * @param const QStringList * text
         */
        coordinates(quint32 row, quint32 col, const QStringList *text = 0);


        /**
         * Sets-up the text pointer.
         *
         * @param const QStringList * text
         */
        void set_text(const QStringList *text);


        /**
         * Sets the row coordinate.
         *
         * @param quint32 row
         */
        void set_row(quint32 row);

        /**
         * Sets the column coordinate.
         *
         * @param quint32 col
         */
        void set_col(quint32 col);

        /**
         * Sets the row and column coordinates.
         *
         * @param quint32 row
         * @param quint32 col
         */
        void set(quint32 row, quint32 col);


        /**
         * Assings the new coordinates and performs validity check.
         * Does not change *text if null.
         *
         * @param _t::editor::coordinates coords
         * @return _t::editor::coordinates &
         */
        coordinates &operator=(const coordinates &coords);


        /**
         * Places the coordinates one cell forward.
         *
         * @return _t::editor::coordinates &
         */
        coordinates &operator++();

        /**
         * Places the coordinates one cell forward.
         *
         * @return _t::editor::coordinates
         */
        coordinates operator++(int);


        /**
         * Places the coordinates one cell backward.
         *
         * @return _t::editor::coordinates &
         */
        coordinates &operator--();

        /**
         * Places the coordinates one cell backward.
         *
         * @return _t::editor::coordinates
         */
        coordinates operator--(int);


        /**
         * Creates new coordinates /value/ cells forward.
         *
         * @param quint32 value
         * @return _t::editor::coordinates
         */
        coordinates operator+(quint32 value) const;

        /**
         * Creates new coordinates /value/ cells backward.
         *
         * @param quint32 value
         * @return _t::editor::coordinates
         */
        coordinates operator-(quint32 value) const;


        /**
         * Moves the coordinates /value/ cells forward.
         *
         * @param quint32 value
         * @return _t::editor::coordinates &
         */
        coordinates &operator+=(quint32 value);

        /**
         * Moves the coordinates /value/ cells backward.
         *
         * @param quint32 value
         * @return _t::editor::coordinates &
         */
        coordinates &operator-=(quint32 value);


        /**
         * Performs equality check of row and column values.
         *
         * @param const coordinates & obj
         * @return bool
         */
        inline bool operator==(const coordinates &obj) const
        {
            return this->col == obj.col && this->row == obj.row;
        }

        /**
         * Performs non-equality check of row and column values.
         *
         * @param const coordinates &obj
         * @return bool
         */
        inline bool operator!=(const coordinates &obj) const
        {
            return !(*this == obj);
        }


        /**
         * Checks if self-coordinates are preceding /obj/.
         *
         * @param const coordinates &obj
         * @return bool
         */
        inline bool operator<(const coordinates &obj) const
        {
            return this->row < obj.row
                || (this->row == obj.row && this->col < obj.col);
        }

        /**
         * Checks if /obj/ coordinates are preceding self-coordinates.
         *
         * @param const coordinates &obj
         * @return bool
         */
        inline bool operator>(const coordinates &obj) const
        {
            return obj < *this;
        }

        /**
         * Checks if self-coordinates are preceding or equal to /obj/.
         *
         * @param const coordinates &obj
         * @return bool
         */
        inline bool operator<=(const coordinates &obj) const
        {
            return !(*this > obj);
        }

        /**
         * Checks if /obj/ coordinates are preceding
         * or equal to self-coordinates.
         *
         * @param const coordinates &obj
         * @return bool
         */
        inline bool operator>=(const coordinates &obj) const
        {
            return !(*this < obj);
        }
    };

    /**
     * The _t::editor::cursor struct.
     */
    struct cursor
    {
        /**
         * The cursor coordinates.
         *
         * @var _t::editor::coordinates coords
         */
        coordinates coords;


        /**
         * States if the cursor is in selection mode (have some cells selected).
         *
         * @var bool selection_mode
         */
        bool selection_mode = false;

        /**
         * Coordinates where the selection began.
         *
         * @var _t::editor::coordinates selection_from
         */
        coordinates selection_from;


        /**
         * Pixmap of active cell for ability to blink cursor.
         *
         * @var QPixmap background
         */
        QPixmap background;


        /**
         * @return quint32 &
         */
        quint32 &row();

        /**
         * @return quint32 &
         */
        quint32 &col();


        /**
         * Places the cursor one cell forward.
         *
         * @return _t::editor::cursor &
         */
        cursor &operator++();

        /**
         * Places the cursor one cell forward.
         *
         * @return _t::editor::cursor
         */
        cursor operator++(int);


        /**
         * Places the cursor one cell backward.
         *
         * @return _t::editor::cursor &
         */
        cursor &operator--();

        /**
         * Places the cursor one cell backward.
         *
         * @return _t::editor::cursor
         */
        cursor operator--(int);


        /**
         * Places the cursor /value/ cells forward.
         *
         * @return _t::editor::cursor &
         */
        cursor &operator+=(qint32 value);

        /**
         * Places the cursor /value/ cells backward.
         *
         * @return _t::editor::cursor &
         */
        cursor &operator-=(qint32 value);
    };


    /**
     * The main QLabel-based element to which the canvas is associated.
     *
     * @var _t::controls::label area
     */
    _t::controls::label area;

    /**
     * Tha canvas for painting the text.
     *
     * @var QPixmap canvas
     */
    QPixmap canvas;


    /**
     * The editor background color.
     *
     * @var QColor background
     */
    QColor background;

    /**
     * The background color for selected cells.
     *
     * @var QColor selection_background
     */
    QColor selection_background;


    /**
     * The editor font family and style.
     *
     * @var QFont font
     */
    QFont font;

    /**
     * The default font color used in editor.
     *
     * @var QColor font_color
     */
    QColor font_color;


    /**
     * @var qint32 cell_width
     */
    qint32 cell_width = 9;

    /**
     * @var qint32 cell_height
     */
    qint32 cell_height = 18;


    /**
     * @var _t::editor::cursor _cursor
     */
    cursor _cursor;

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
     * Sets-up the painter to a clearing mode.
     *
     * @param QPainter & painter
     */
    void setup_painter_clear(QPainter &painter);

    /**
     * Sets-up the painter to a writing mode.
     *
     * @param QPainter & painter
     */
    void setup_painter_write(QPainter &painter);


    /**
     * Clears the specified cell.
     *
     * @param QPainter &                      painter
     * @param const _t::editor::coordinates & coords
     */
    void clear_cell(QPainter &painter, const coordinates &coords);

    /**
     * Clears the cell where the cursor is currently placed.
     *
     * @param QPainter & painter
     */
    void clear_cell(QPainter &painter);


    /**
     * Deselect all selected cells.
     */
    void deselect();


    /**
     * Apply selected-cell style to the active cell.
     */
    void draw_selected_cell();

    /**
     * Apply selected-cell style to the cell.
     *
     * @param const _t::editor::coordinates & coords
     */
    void draw_selected_cell(const coordinates &coords);

    /**
     * Apply normal cell style to the active cell.
     */
    void draw_deselected_cell();

    /**
     * Apply normal cell style to the cell.
     *
     * @param const _t::editor::coordinates & coords
     */
    void draw_deselected_cell(const coordinates &coords);


    /**
     * Redraw the background color of the cell.
     *
     * @param const _t::editor::coordinates & coords
     * @param const QColor &                  color
     */
    void redraw_cell_bg(const coordinates &coords, const QColor &color);


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
     * @param const _t::editor::coordinates &                      start
     * @param const _t::editor::coordinates &                      end
     * @param std::function<void(const _t::editor::coordinates &)> func
     */
    void each_cell(
        const coordinates &start,
        const coordinates &end,
        std::function<void(const coordinates &)> func);

private slots:
    /**
     * Handles any user input from mouse.
     *
     * @param QMouseEvent * event
     */
    void area_clicked(QMouseEvent *event);

    /**
     * Handles showing/hiding cursor when blinking.
     */
    void cursor_timer_tick();

public:
    /**
     * Initializes the editor with default values, attaches self to the parent.
     *
     * @param QWidget *parent_widget
     */
    void init(QWidget *parent_widget);


    /**
     * Writes the text into editor's memory and displays it on the screen.
     *
     * @param const QString &text
     */
    void write(const QString &text);
};

#endif // _T_EDITOR_H
