#include "editor.h"


qint32 &_t::editor::cursor::row()
{
    return this->coords.row;
}

qint32 &_t::editor::cursor::col()
{
    return this->coords.col;
}
