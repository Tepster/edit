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
        // number literals
        { QColor(180, 120, 230), "\\b(\\d+|0x[0-9a-fA-F]+)\\b" },

        // functions
        { QColor(120, 220, 255), "\\b([a-zA-Z_][a-zA-Z0-9_]*)[^\\S\\n]*\\(" },

        // general keywords
        { QColor(255, 90, 100), "\\b(asm|auto|break|case|catch|class|const|"
                                "const_cast|constexpr|continue|decltype|"
                                "default|delete|do|dynamic_cast|enum|else|"
                                "explicit|extern|for|friend|goto|if|import|"
                                "inline|module|mutable|namespace|new|noexcept|"
                                "nullptr|operator|private|protected|public|"
                                "register|reinterpret_cast|restrict|return|"
                                "sizeof|static|struct|template|this|throw|try|"
                                "typedef|typeid|typename|self|static_cast|"
                                "switch|union|using|virtual|void|volatile|while"
                                ")\\b" },

        // types
        { QColor(100, 200, 255), "\\b(bool|char|char16_t|char32_t|double|float|"
                                 "int|int16_t|int32_t|int64_t|int8_t|long|"
                                 "short|signed|uint16_t|uint32_t|uint64_t|"
                                 "uint8_t|unsigned|wchar_t)\\b" },

        // string literals
        { QColor(100, 200, 100), "(\"(?:[^\\n\"]|\\\")*\")" },

        // character literals
        { QColor(100, 200, 100), "('(?:[^\\n']|\\')*')" },

        // pre-processor directives
        { QColor(100, 200, 255), "(?:^|\\n)\\s*(#[^\\S\\n]*"
                                 "(?:include|define|undef|if|ifdef|ifndef|else|"
                                 "elif|endif|error|pragma)\\b)" },
        { QColor(100, 200, 100), "(?:^|\\n)\\s*#[^\\S\\n]*"
                                 "include[^\\S\\n]+(<[^\\n]*>|\"[^\\n]*\")" },

        // comments
        { QColor(130, 130, 130), "((?:/\\*[^(?:\\*/)]*(?:\\*/)?)|"
                                 "(?://[^\\n]*))" },
    };

    QVector<QPair<QColor, QString>> csharp
    {
        // number literals
        { QColor(180, 120, 230), "\\b(\\d+|0x[0-9a-fA-F]+)\\b" },

        // other literals
        { QColor(180, 120, 230), "\\b(null|true|false)\\b" },

        // functions
        { QColor(120, 220, 255), "\\b([a-zA-Z_][a-zA-Z0-9_]*)[^\\S\\n]*\\(" },

        // general keywords
        { QColor(255, 90, 100), "\\b(abstract|as|async|await|base|break|case|"
                                "catch|checked|class|const|continue|default|"
                                "delegate|do|enum|else|event|explicit|extern|"
                                "finally|fixed|for|foreach|get|goto|if|in|"
                                "interface|internal|is|lock|namespace|new|"
                                "operator|out|override|params|private|"
                                "protected|public|readonly|ref|return|sealed|"
                                "set|static|struct|this|throw|try|typeof|"
                                "switch|unchecked|unsafe|using|var|virtual|"
                                "void|while|yield)\\b" },

        // types
        { QColor(100, 200, 255), "\\b(bool|byte|char|decimal|double|float|int|"
                                 "long|object|sbyte|short|string|uint|ulong|"
                                 "unsigned|ushort|var)\\b" },

        // string literals
        { QColor(100, 200, 100), "(\"(?:[^\\n\"]|\\\")*\")" },

        // character literals
        { QColor(100, 200, 100), "('(?:[^\\n']|\\')*')" },

        // pre-processor directives
        { QColor(100, 200, 255), "(?:^|\\n)\\s*(#[^\\S\\n]*"
                                 "(?:define|undef|if|else|elif|endif|line|"
                                 "error|warning|region|endregion)\\b)" },

        // comments
        { QColor(130, 130, 130), "((?:/\\*[^(?:\\*/)]*(?:\\*/)?)|"
                                 "(?://[^\\n]*))" },
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
