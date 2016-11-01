#ifndef _T_EDITOR_CURSOR_H
#define _T_EDITOR_CURSOR_H

#include <QPixmap>

#include "editor/coordinates.h"


namespace _t
{
    namespace editor
    {
        class cursor;
    }
}

/**
 * The _t::editor::cursor struct.
 */
struct _t::editor::cursor
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

#endif // _T_EDITOR_CURSOR_H
