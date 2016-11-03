#ifndef _T_EDITOR_DRAWING_MANAGER_H
#define _T_EDITOR_DRAWING_MANAGER_H

#include <QPixmap>
#include <QPainter>

#include <QChar>

#include <QFont>
#include <QColor>
#include <QSize>

#include "editor/coordinates.h"


namespace _t
{
    namespace editor
    {
        class drawing_manager;
    }
}

/**
 * The _t::editor::drawing_manager class which provides functions
 * for drawing on a canvas.
 */
class _t::editor::drawing_manager
{
    /**
     * Painter for drawing on canvas.
     *
     * @var QPainter painter
     */
    QPainter painter;

    /**
     * The canvas for drawing.
     *
     * @var QPixmap * canvas
     */
    QPixmap *canvas;


    /**
     * The background color.
     *
     * @var QColor background
     */
    QColor background;

    /**
     * The font family and style.
     *
     * @var QFont font
     */
    QFont font;

    /**
     * The default font color.
     *
     * @var QColor font_color
     */
    QColor font_color;

    /**
     * @var QSize cell_size *
     */
    QSize *cell_size;


    /**
     * Sets-up the painter to clearing mode.
     */
    void setup_clearing();

    /**
     * Sets-up the painter to clearing mode by specified background color.
     *
     * @param const QColor & color
     */
    void setup_clearing(const QColor &color);

    /**
     * Sets-up the painter to writing mode.
     */
    void setup_writing();

public:
    /**
     * Main initializer of default values.
     *
     * @param const QColor  & background
     * @param const QFont   & font
     * @param const QColor  & font_color
     * @param       QSize   * cell_size
     * @param       QPixmap * canvas
     */
    void init(
        const QColor &background,
        const QFont &font,
        const QColor &font_color,
        QSize *cell_size,
        QPixmap *canvas = 0);

    /**
     * Initializes the canvas.
     *
     * @param QPixmap * canvas
     */
    void init_canvas(QPixmap *canvas);

    /**
     * Ends the painter.
     */
    void end();


    /**
     * Paints the character to specified coordinates.
     *
     * @param const _t::editor::coordinates & coords
     * @param const QChar                   & character
     */
    void draw_char(
        const _t::editor::coordinates &coords,
        const QChar &character);

    /**
     * Paints the character to specified coordinates.
     *
     * @param const _t::editor::coordinates & coords
     * @param const QChar                   & character
     * @param const QColor                  & background
     */
    void draw_char(
        const _t::editor::coordinates &coords,
        const QChar &character,
        const QColor &background);

    /**
     * Draws the cursor to the specified cell.
     *
     * @param const _t::editor::coordinates & coords
     */
    void draw_cursor(const _t::editor::coordinates &coords);

    /**
     * Draws the pixmap to the specified cell.
     *
     * @param const _t::editor::coordinates & coords
     * @param const QPixmap                 & pixmap
     */
    void draw_pixmap(
        const _t::editor::coordinates &coords,
        const QPixmap &pixmap);

    /**
     * Draws the pixmap to the top-left corner.
     *
     * @param const QPixmap & pixmap
     */
    void draw_pixmap(const QPixmap &pixmap);


    /**
     * Moves the area to destination.
     *
     * @param const _t::editor::coordinates & src
     * @param       qint32                    num
     * @param const _t::editor::coordinates & dst
     */
    void move(
        const _t::editor::coordinates &src,
        qint32 num,
        const _t::editor::coordinates &dst);

    /**
     * Shifts the area one row up.
     *
     * @param const _t::editor::coordinates & coords
     * @param       qint32                    num
     */
    void shift_up(const _t::editor::coordinates &coords, qint32 num);

    /**
     * Shifts the area one row down.
     *
     * @param const _t::editor::coordinates & coords
     * @param       qint32                    num
     */
    void shift_down(const _t::editor::coordinates &coords, qint32 num);

    /**
     * Shifts the area one cell left.
     *
     * @param const _t::editor::coordinates & coords
     * @param       qint32                    num
     */
    void shift_left(const _t::editor::coordinates &coords, qint32 num);

    /**
     * Shifts the area one cell right.
     *
     * @param const _t::editor::coordinates & coords
     * @param       qint32                    num
     */
    void shift_right(const _t::editor::coordinates &coords, qint32 num);


    /**
     * Clears the specified cell with default background color.
     *
     * @param const _t::editor::coordinates & coords
     */
    void clear(const _t::editor::coordinates &coords);

    /**
     * Clears the specified cell with the specified color.
     *
     * @param const _t::editor::coordinates & coords
     * @param const QColor                  & color
     */
    void clear(const _t::editor::coordinates &coords, const QColor &color);
};

#endif // _T_EDITOR_DRAWING_MANAGER_H
