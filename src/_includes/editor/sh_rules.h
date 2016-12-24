#ifndef _T_EDITOR_SH_RULES_H
#define _T_EDITOR_SH_RULES_H

#include <QMap>

#include <QColor>
#include <QString>


namespace _t
{
    namespace editor
    {
        class sh_rules;
    }
}

class _t::editor::sh_rules
{
public:
    QMap<unsigned int, QString> cpp
    {
        // string literals
        { QColor(255, 170, 100).rgba(), "(\"[^\\n\"]*\")" },

        // pre-processor directives
        { QColor(100, 200, 255).rgba(), "(?:^|\\n)\\s*(#[^\\S\\n]*(?:include|define|ifdef|ifndef|endif)\\b)" },
        { QColor(100, 200, 100).rgba(), "(?:^|\\n)\\s*#[^\\S\\n]*include[^\\S\\n]+(<[^>]*>|\"[^\"\\n]*\")" },

        // comments
        { QColor(130, 130, 130).rgba(), "((?:/\\*[^(?:\\*/)]*(?:\\*/)?)|(?://[^\\n]*))" },
        { QColor(255, 90, 100).rgba(), "\\b(namespace|class|public|unsigned|int)\\b" },
    };


    QMap<unsigned int, QString> assembly
    {
        // string literals
        { QColor(255, 170, 100).rgba(), "\\s+(R[0-7])\\b" },
        { QColor(255, 170, 100).rgba(), "\\b(\{[a-zA-Z0-9_]+\})\\b" },

        // functions
        { QColor(100, 200, 255).rgba(), "^\\s*(::[a-zA-Z0-9_]+)\\b" },

        // comments
        { QColor(130, 130, 130).rgba(), "(#[^\\n]*)$" },

        // keywords
        { QColor(255, 90, 100).rgba(), "\\s*(mov|clr|eq|gt|lt|jmp|jmpif|shl|shr|add|sub|mul|div|ina|read|write)(?:\\s+|$)" },
    };
};

#endif // _T_EDITOR_SH_RULES_H
