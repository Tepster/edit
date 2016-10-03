#include "editor.h"


qint32 _t::editor::coordinates::row() const
{
    return this->_row;
}

qint32 _t::editor::coordinates::col() const
{
    return this->_col;
}


_t::editor::coordinates *_t::editor::coordinates::set_text(QStringList *text)
{
    this->text = text;

    return this;
}

_t::editor::coordinates *_t::editor::coordinates::set_row(qint32 row)
{
    if (this->text->count() > row)
    {
        this->_row = row;
    }
    else
    {
        this->_row = this->text->count() - 1;
    }

    return this;
}

_t::editor::coordinates *_t::editor::coordinates::set_col(qint32 col)
{
    qint32 current_line_length = this->text->at(this->_row).length();

    if (current_line_length >= col)
    {
        this->_col = col;
    }
    else
    {
        this->_col = current_line_length;
    }

    return this;
}

_t::editor::coordinates *_t::editor::coordinates::set(qint32 row, qint32 col)
{
    this->set_row(row)->set_col(col);

    return this;
}


_t::editor::coordinates _t::editor::coordinates::operator++()
{
    if (this->text->at(this->_row).length() > this->_col)
    {
        ++this->_col;
    }
    else
    {
        if (this->text->count() - 1 > this->_row)
        {
            ++this->_row;
            this->_col = 0;
        }
    }

    return *this;
}

_t::editor::coordinates _t::editor::coordinates::operator++(int)
{
    coordinates original = *this;
    this->operator++();

    return original;
}


_t::editor::coordinates _t::editor::coordinates::operator--()
{
    if (this->_col > 0)
    {
        --this->_col;
    }
    else
    {
        if (this->_row > 0)
        {
            --this->_row;
            this->_col = this->text->at(this->_row).length();
        }
    }

    return *this;
}

_t::editor::coordinates _t::editor::coordinates::operator--(int)
{
    coordinates original = *this;
    this->operator--();

    return original;
}


_t::editor::coordinates _t::editor::coordinates::operator+(quint32 value)
{
    // very ugly and inefficient
    coordinates coords = *this;

    for (quint32 i = 0; i < value; ++i)
    {
        this->operator++();
    }

    return coords;
}

_t::editor::coordinates _t::editor::coordinates::operator-(quint32 value)
{
    // very ugly and inefficient
    coordinates coords = *this;

    for (quint32 i = 0; i < value; ++i)
    {
        this->operator--();
    }

    return coords;
}


_t::editor::coordinates _t::editor::coordinates::operator+=(quint32 value)
{
    // very ugly and inefficient
    for (quint32 i = 0; i < value; ++i)
    {
        this->operator++();
    }

    return *this;
}

_t::editor::coordinates _t::editor::coordinates::operator-=(quint32 value)
{
    // very ugly and inefficient
    for (quint32 i = 0; i < value; ++i)
    {
        this->operator--();
    }

    return *this;
}
