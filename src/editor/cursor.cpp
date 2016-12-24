#include "editor/cursor.h"


quint32 &_t::editor::cursor::row()
{
    return (quint32)this->coords.row;
}

quint32 &_t::editor::cursor::col()
{
    return (quint32)this->coords.col;
}


_t::editor::cursor &_t::editor::cursor::operator++()
{
    ++this->coords;

    return *this;
}

_t::editor::cursor _t::editor::cursor::operator++(int)
{
    cursor tmp = *this;

    ++(*this);

    return tmp;
}


_t::editor::cursor &_t::editor::cursor::operator--()
{
    --this->coords;

    return *this;
}

_t::editor::cursor _t::editor::cursor::operator--(int)
{
    cursor tmp = *this;

    --(*this);

    return tmp;
}


_t::editor::cursor &_t::editor::cursor::operator+=(qint32 value)
{
    if (value > 0)
    {
        this->coords += value;
    }
    else
    {
        this->coords -= abs(value);
    }

    return *this;
}

_t::editor::cursor &_t::editor::cursor::operator-=(qint32 value)
{
    if (value > 0)
    {
        this->coords -= value;
    }
    else
    {
        this->coords += abs(value);
    }

    return *this;
}
