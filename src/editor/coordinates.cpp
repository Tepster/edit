#include "editor.h"


_t::editor::coordinates::coordinates(
    quint32 row,
    quint32 col,
    const QStringList *text)
{
    this->set_text(text);

    this->set(row, col);
}


void _t::editor::coordinates::set_text(const QStringList *text)
{
    this->text = text;

    // validate the row/col values
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


void _t::editor::coordinates::set_row(quint32 row)
{
    if (!this->text || this->text->count() > row)
    {
        this->row = row;
    }
    else
    {
        this->row = this->text->count() - 1;
    }
}

void _t::editor::coordinates::set_col(quint32 col)
{
    if (!this->text || this->text->at(this->row).length() >= col)
    {
        this->col = col;
    }
    else
    {
        this->col = this->text->at(this->row).length();
    }
}

void _t::editor::coordinates::set(quint32 row, quint32 col)
{
    if (this->text)
    {
        if (this->text->count() > row)
        {
            this->row = row;
        }
        else
        {
            this->row = this->text->count() - 1;
        }

        if (this->text->at(this->row).length() >= col)
        {
            this->col = col;
        }
        else
        {
            this->col = this->text->at(this->row).length();
        }
    }
    else
    {
        this->row = row;
        this->col = col;
    }
}


_t::editor::coordinates &_t::editor::coordinates::operator=(
    const coordinates &coords)
{
    if (coords.text)
    {
        this->set_text(coords.text);
    }

    this->set(coords.row, coords.col);

    return *this;
}


_t::editor::coordinates &_t::editor::coordinates::operator++()
{
    if (this->text)
    {
        if (this->text->at(this->row).length() > this->col)
        {
            ++this->col;
        }
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
    coordinates tmp = *this;

    ++(*this);

    return tmp;
}


_t::editor::coordinates &_t::editor::coordinates::operator--()
{
    if (this->col > 0)
    {
        --this->col;
    }
    else if (this->row > 0)
    {
        --this->row;
        this->col = this->text ? this->text->at(this->row).length() : 0;
    }

    return *this;
}

_t::editor::coordinates _t::editor::coordinates::operator--(int)
{
    coordinates tmp = *this;

    --(*this);

    return tmp;
}


_t::editor::coordinates _t::editor::coordinates::operator+(quint32 value) const
{
    coordinates ret = *this;
    ret += value;

    return ret;
}

_t::editor::coordinates _t::editor::coordinates::operator-(quint32 value) const
{
    coordinates ret = *this;
    ret -= value;

    return ret;
}


_t::editor::coordinates &_t::editor::coordinates::operator+=(quint32 value)
{
    // veeery ugly
    do
    {
        ++(*this);
    }
    while (--value > 0);

    return *this;
}

_t::editor::coordinates &_t::editor::coordinates::operator-=(quint32 value)
{
    // veeery ugly
    do
    {
        --(*this);
    }
    while (--value > 0);

    return *this;
}
