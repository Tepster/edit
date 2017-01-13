#ifndef _T_UTILS_H
#define _T_UTILS_H

#include <QRegExp>
#include <QChar>


namespace _t
{
    class utils;
}

class _t::utils
{
public:
    static bool is_character(const QChar& ch)
    {
        return (new QRegExp("^[a-zA-Z0-9]$"))->exactMatch(ch);
    }

    static bool is_whitespace(const QChar& ch)
    {
        return (new QRegExp("^\\s$"))->exactMatch(ch);
    }

    static bool is_special(const QChar& ch)
    {
        return !is_character(ch) && !is_whitespace(ch);
    }
};

#endif // _T_UTILS_H
