#ifndef _T_EDITOR_H
#define _T_EDITOR_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

#include <QKeyEvent>
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
 * The _t::editor class representing one editor control with specific text inside its area.
 */
class _t::editor : public QWidget
{
    Q_OBJECT

    /**
     * The coords struct for handling editor coordinates.
     * All methods ensure the coordinates to be valid according to line lengths.
     */
    struct coords
    {
    private:
        /**
         * Pointer to _t::editor's text for knowledge of lines lengths.
         *
         * @var QStringList *text
         */
        QStringList *text = 0;

    public:
        /**
         * The actual row value as an x-coordinate.
         *
         * @var qint32 row
         */
        qint32 row = 0;

        /**
         * The actual column value as a y-coordinate.
         *
         * @var qint32 col
         */
        qint32 col = 0;


        /**
         * Default constructor.
         */
        coords() {}

        /**
         * Constructor that sets-up the text pointer.
         *
         * @param QStringList *text
         */
        coords(QStringList *text) : text(text) {}

        /**
         * Constructor that sets-up the coordinates values.
         *
         * @param qint32 row
         * @param qint32 col
         */
        coords(qint32 row, qint32 col) : row(row), col(col) {}


        /**
         * Sets-up the text pointer.
         *
         * @param QStringList *text
         * @return _t::editor::coords *  Self pointer.
         */
        coords *set_text(QStringList *text);


        /**
         * Sets-up the row with validity check.
         *
         * @param qint32 row
         * @return _t::editor::coords *  Self pointer.
         */
        coords *set_row(qint32 row);

        /**
         * Sets-up the column with validity check.
         *
         * @param qint32 col
         * @return _t::editor::coords *  Self pointer.
         */
        coords *set_col(qint32 col);

        /**
         * Sets-up the row and column with validity check.
         *
         * @param qint32 row
         * @param qint32 col
         * @return _t::editor::coords *  Self pointer.
         */
        coords *set(qint32 row, qint32 col);


        /**
         * Places coordinates one cell forward, if valid.
         *
         * @return _t::editor::coords &  Self ref.
         */
        coords &operator++();

        /**
         * Places coordinates one cell forward, if valid.
         *
         * @return _t::editor::coords  Original coordinates.
         */
        coords operator++(int);


        /**
         * Places coordinates one cell backward, if valid.
         *
         * @return _t::editor::coords &  Self ref.
         */
        coords &operator--();

        /**
         * Places coordinates one cell backward, if valid.
         *
         * @return _t::editor::coords  Original coordinates.
         */
        coords operator--(int);


        /**
         * Create coordinates with /value/ cells forward, till valid.
         *
         * @param quint32 value
         * @return _t::editor::coords  New coordinates.
         */
        coords operator+(quint32 value);

        /**
         * Create coordinates with /value/ cells backward, till valid.
         *
         * @param qint32 value
         * @return _t::editor::coords  New coordinates.
         */
        coords operator-(quint32 value);


        /**
         * Move /value/ cells forward, till valid.
         *
         * @param qint32 value
         * @return _t::editor::coords &  Self ref.
         */
        coords &operator+=(quint32 value);

        /**
         * Move /value/ cells backward, till valid.
         *
         * @param qint32 value
         * @return _t::editor::coords &  Self ref.
         */
        coords &operator-=(quint32 value);


        /**
         * Performs equality check of all private and public values.
         *
         * @param const coords &obj
         * @return bool
         */
        inline bool operator==(const coords &obj) const
        {
            return this->col == obj.col && this->row == obj.row && this->text == obj.text;
        }

        /**
         * Performs non-equality check of all private and public values.
         *
         * @param const coords &obj
         * @return bool
         */
        inline bool operator!=(const coords &obj) const
        {
            return !(*this == obj);
        }


        /**
         * Checks if self-coordinates are preceding /obj/.
         *
         * @param const coords &obj
         * @return bool
         */
        inline bool operator<(const coords &obj) const
        {
            return this->row < obj.row || (this->row == obj.row && this->col < obj.col);
        }

        /**
         * Checks if /obj/ coordinates are preceding self-coordinates.
         *
         * @param const coords &obj
         * @return bool
         */
        inline bool operator>(const coords &obj) const
        {
            return obj < *this;
        }

        /**
         * Checks if self-coordinates are preceding or equal to /obj/.
         *
         * @param const coords &obj
         * @return bool
         */
        inline bool operator<=(const coords &obj) const
        {
            return !(*this > obj);
        }

        /**
         * Checks if /obj/ coordinates are preceding or equal to self-coordinates.
         *
         * @param const coords &obj
         * @return bool
         */
        inline bool operator>=(const coords &obj) const
        {
            return !(*this < obj);
        }
    };

    /**
     * The cursor struct representing one cursor specified mainly by its coordinates.
     *
     * Coords struct methods implicitly check the coordinates validity.
     * Row/column getters return reference to native values which are not validated.
     */
    struct cursor
    {
        /**
         * The cursor coordinates.
         *
         * @var _t::editor::coords coords
         */
        coords coords;


        /**
         * Pixmap of active cell for ability to blink cursor.
         *
         * @var QPixmap background
         */
        QPixmap background;


        /**
         * Cursor coordinates row getter.
         *
         * @return qint32 &  Row ref.
         */
        qint32 &row();

        /**
         * Cursor coordinates column getter.
         *
         * @return qint32 &  Column ref.
         */
        qint32 &col();
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
     * @var _t::editor::cursor cursor
     */
    cursor cursor;

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
     * The actual text in the editor, saved in list for easy manipulation with line lengths.
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
     * @param QKeyEvent *event
     */
    void keyPressEvent(QKeyEvent *event);

    /**
     * Handles focus-in event.
     *
     * @param QFocusEvent *event
     */
    void focusInEvent(QFocusEvent *event);

    /**
     * Handles focus-out event.
     *
     * @param QFocusEvent *event
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
     * @param QPainter &painter
     */
    void setup_painter_clear(QPainter &painter);

    /**
     * Sets-up the painter to a writing mode.
     *
     * @param QPainter &painter
     */
    void setup_painter_write(QPainter &painter);


    /**
     * Clears the specified cell.
     *
     * @param QPainter &painter
     * @param const _t::editor::coords &coords
     */
    void clear_cell(QPainter &painter, const coords &coords);

    /**
     * Clears the cell where the cursor is currently placed.
     *
     * @param QPainter &painter
     */
    void clear_cell(QPainter &painter);


    /**
     * Displays the editor canvas on the screen.
     */
    void update();


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

private slots:
    /**
     * Handles any user input from mouse.
     *
     * @param QPoint coords
     */
    void area_clicked(QPoint coords);

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
