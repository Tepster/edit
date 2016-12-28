#ifndef _T_EDITOR_SH_RULES_H
#define _T_EDITOR_SH_RULES_H

#include <QVector>
#include <QPair>

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
    QVector<QPair<QColor, QString>> cpp
    {
        // pre-processor directives
        { QColor(100, 200, 255), "(?:^|\\n)\\s*(#[^\\S\\n]*(?:include|define|ifdef|ifndef|endif)\\b)" },
        { QColor(100, 200, 100), "(?:^|\\n)\\s*#[^\\S\\n]*include[^\\S\\n]+(<[^>]*>|\"[^\"\\n]*\")" },

        // comments
        { QColor(130, 130, 130), "((?:/\\*[^(?:\\*/)]*(?:\\*/)?)|(?://[^\\n]*))" },
        { QColor(255, 90, 100), "\\b(namespace|class|public|unsigned|int)\\b" },

        // string literals
        { QColor(255, 170, 100), "(\"[^\\n\"]*\")" },
    };

    QVector<QPair<QColor, QString>> csharp
    {

    };

    QVector<QPair<QColor, QString>> php
    {

    };

    QVector<QPair<QColor, QString>> javascript
    {

    };

    QVector<QPair<QColor, QString>> html
    {

    };

    QVector<QPair<QColor, QString>> css
    {

    };
};

#endif // _T_EDITOR_SH_RULES_H
