#ifndef _T_EDITOR_COORDINATES_H
#define _T_EDITOR_COORDINATES_H

#include <QStringList>


namespace _t
{
    namespace editor
    {
        struct coordinates;
    }
}

/**
 * The _t::editor::coordinates struct which stores the row and column values
 * and provide basic comparing functions.
 *
 * @note All methods implicitly validate the coordinates values.
 */
struct _t::editor::coordinates
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
     * @param       qint32        row
     * @param       qint32        col
     * @param const QStringList * text
     */
    coordinates(qint32 row, qint32 col, const QStringList *text = 0);


    /**
     * Sets the row and column coordinates and text pointer, if provided.
     *
     * @param       qint32        row
     * @param       qint32        col
     * @param const QStringList * text
     */
    void set(qint32 row, qint32 col, const QStringList *text = 0);

    /**
     * Sets-up the text pointer.
     *
     * @param const QStringList * text
     */
    void set_text(const QStringList *text);

    /**
     * Sets the row coordinate.
     *
     * @param qint32 row
     */
    void set_row(qint32 row);

    /**
     * Sets the column coordinate.
     *
     * @param qint32 col
     */
    void set_col(qint32 col);


    /**
     * Assings the new coordinates and performs validity check.
     * Does not change *text if null.
     *
     * @param _t::editor::coordinates & coords
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
     * @param qint32 value
     * @return _t::editor::coordinates
     */
    coordinates operator+(qint32 value) const;

    /**
     * Creates new coordinates /value/ cells backward.
     *
     * @param qint32 value
     * @return _t::editor::coordinates
     */
    coordinates operator-(qint32 value) const;


    /**
     * Moves the coordinates /value/ cells forward.
     *
     * @param qint32 value
     * @return _t::editor::coordinates &
     */
    coordinates &operator+=(qint32 value);

    /**
     * Moves the coordinates /value/ cells backward.
     *
     * @param qint32 value
     * @return _t::editor::coordinates &
     */
    coordinates &operator-=(qint32 value);


    /**
     * Counts the distance between two coordinates.
     *
     * @param const _t::editor::coordinates & value
     * @return qint32
     */
    qint32 operator-(const coordinates &value) const;


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
     * @param const coordinates & obj
     * @return bool
     */
    inline bool operator!=(const coordinates &obj) const
    {
        return !(*this == obj);
    }


    /**
     * Checks if self-coordinates are preceding /obj/.
     *
     * @param const coordinates & obj
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
     * @param const coordinates & obj
     * @return bool
     */
    inline bool operator>(const coordinates &obj) const
    {
        return obj < *this;
    }

    /**
     * Checks if self-coordinates are preceding or equal to /obj/.
     *
     * @param const coordinates & obj
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
     * @param const coordinates & obj
     * @return bool
     */
    inline bool operator>=(const coordinates &obj) const
    {
        return !(*this < obj);
    }


    /**
     * Checks if coordinates are at [0,0] or not.
     *
     * @return bool
     */
    inline operator bool() const
    {
        return this->row || this->col;
    }
};

#endif // _T_EDITOR_COORDINATES_H
