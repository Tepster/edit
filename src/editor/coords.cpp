#include "editor.h"


_t::editor::coords *_t::editor::coords::set_text(QStringList *text)
{
    this->text = text;

    return this;
}

_t::editor::coords *_t::editor::coords::set_row(qint32 row)
{
    if (row < 0)
    {
        return this;
    }

    if (this->text->count() > row)
    {
        this->row = row;
    }
    else
    {
        this->row = this->text->count() - 1;
    }

    // checks if isn't behind the end of the line
    this->set_col(this->col);

    return this;
}

_t::editor::coords *_t::editor::coords::set_col(qint32 col)
{
    if (col < 0)
    {
        return this;
    }

    qint32 current_line_length = this->text->at(this->row).length();

    if (current_line_length >= col)
    {
        this->col = col;
    }
    else
    {
        this->col = current_line_length;
    }

    return this;
}

_t::editor::coords *_t::editor::coords::set(qint32 row, qint32 col)
{
    this->set_row(row)->set_col(col);

    return this;
}


_t::editor::coords &_t::editor::coords::operator++()
{
    if (this->text->at(this->row).length() > this->col)
    {
        ++this->col;
    }
    else
    {
        if (this->text->count() - 1 > this->row)
        {
            ++this->row;
            this->col = 0;
        }
    }

    return *this;
}

_t::editor::coords _t::editor::coords::operator++(int)
{
    coords original = *this;
    this->operator++();

    return original;
}


_t::editor::coords &_t::editor::coords::operator--()
{
    if (this->col > 0)
    {
        --this->col;
    }
    else
    {
        if (this->row > 0)
        {
            --this->row;
            this->col = this->text->at(this->row).length();
        }
    }

    return *this;
}

_t::editor::coords _t::editor::coords::operator--(int)
{
    coords original = *this;
    this->operator--();

    return original;
}


_t::editor::coords _t::editor::coords::operator+(quint32 value)
{
    // very ugly and inefficient
    coords coords = *this;

    for (quint32 i = 0; i < value; ++i)
    {
        this->operator++();
    }

    return coords;
}

_t::editor::coords _t::editor::coords::operator-(quint32 value)
{
    // very ugly and inefficient
    coords coords = *this;

    for (quint32 i = 0; i < value; ++i)
    {
        this->operator--();
    }

    return coords;
}


_t::editor::coords &_t::editor::coords::operator+=(quint32 value)
{
    // very ugly and inefficient
    for (quint32 i = 0; i < value; ++i)
    {
        this->operator++();
    }

    return *this;
}

_t::editor::coords &_t::editor::coords::operator-=(quint32 value)
{
    // very ugly and inefficient
    for (quint32 i = 0; i < value; ++i)
    {
        this->operator--();
    }

    return *this;
}
