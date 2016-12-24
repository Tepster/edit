#include "editor/coordinates.h"

#include <QtGlobal>


_t::editor::coordinates::coordinates(
    qint32 row,
    qint32 col,
    const QStringList *text)
{
    this->set(row, col, text);
}


void _t::editor::coordinates::set(
    qint32 row,
    qint32 col,
    const QStringList *text)
{
    if (text)
    {
        this->set_text(text);
    }

    this->set_row(row);
    this->set_col(col);
}

void _t::editor::coordinates::set_text(const QStringList *text)
{
    this->text = text;

    // check for negative value
    if (this->row < 0)
    {
        this->row = 0;
    }

    // check for negative value
    if (this->col < 0)
    {
        this->col = 0;
    }

    // check for out-of-range value
    if (this->text)
    {
        if (this->text->count() <= this->row)
        {
            this->row = this->text->count() - 1;
        }

        if (this->text->at(this->row).length() < this->col)
        {
            this->col = this->text->at(this->row).length();
        }
    }
}

void _t::editor::coordinates::set_row(qint32 row)
{
    // check for negative value
    if (row < 0)
    {
        this->row = 0;
    }

    // check for when *text is not set-up OR it is and the row is in range
    else if (!this->text || this->text->count() > row)
    {
        this->row = row;
    }

    // otherwise set row to the end of the text
    else
    {
        this->row = this->text->count() - 1;
    }
}

void _t::editor::coordinates::set_col(qint32 col)
{
    // check for negative value
    if (col < 0)
    {
        this->col = 0;
    }

    // check for when *text is not set-up OR it is and the column is in range
    else if (!this->text || this->text->at(this->row).length() >= col)
    {
        this->col = col;
    }

    // otherwise set column to the end of the active line
    else
    {
        this->col = this->text->at(this->row).length();
    }
}


_t::editor::coordinates &_t::editor::coordinates::operator=(
    const coordinates &coords)
{
    this->set(coords.row, coords.col, coords.text);

    return *this;
}


_t::editor::coordinates &_t::editor::coordinates::operator++()
{
    if (this->text)
    {
        // coords not at the end of the active line
        if (this->text->at(this->row).length() > this->col)
        {
            ++this->col;
        }

        // coords are at the end of the active line and not at the last line
        else if (this->text->count() - 1 > this->row)
        {
            ++this->row;
            this->col = 0;
        }
    }
    else
    {
        ++this->col;
    }

    return *this;
}

_t::editor::coordinates _t::editor::coordinates::operator++(int)
{
    coordinates tmp(*this);

    ++(*this);

    return tmp;
}


_t::editor::coordinates &_t::editor::coordinates::operator--()
{
    // column is not at the beginning of the line
    if (this->col > 0)
    {
        --this->col;
    }

    // cursor is not at the first line
    else if (this->row > 0)
    {
        --this->row;

        if (this->text)
        {
            this->col = this->text->at(this->row);
        }
    }

    return *this;
}

_t::editor::coordinates _t::editor::coordinates::operator--(int)
{
    coordinates tmp(*this);

    --(*this);

    return tmp;
}


_t::editor::coordinates _t::editor::coordinates::operator+(qint32 value) const
{
    coordinates tmp(*this);
    tmp += value;

    return tmp;
}

_t::editor::coordinates _t::editor::coordinates::operator-(qint32 value) const
{
    coordinates tmp(*this);
    tmp -= value;

    return tmp;
}


_t::editor::coordinates &_t::editor::coordinates::operator+=(qint32 value)
{
    if (value < 0)
    {
        *this -= qAbs(value);
    }
    else
    {
        // todo
        while (value > 0)
        {
            ++(*this);
            --value;
        }
    }

    return *this;
}

_t::editor::coordinates &_t::editor::coordinates::operator-=(qint32 value)
{
    if (value < 0)
    {
        *this += qAbs(value);
    }
    else
    {
        // todo
        while (value > 0)
        {
            --(*this);
            --value;
        }
    }

    return *this;
}
