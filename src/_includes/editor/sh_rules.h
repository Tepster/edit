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

        // other literals
        { QColor(180, 120, 230), "\\b(true|false)\\b" },

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
        { QColor(130, 130, 130), "(/\\*(?:[^*]|\\*(?!/))*\\*/)" },
        { QColor(130, 130, 130), "(//[^\\n]*)" },
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
        { QColor(130, 130, 130), "(/\\*(?:[^*]|\\*(?!/))*\\*/)" },
        { QColor(130, 130, 130), "(//[^\\n]*)" },
    };

    QVector<QPair<QColor, QString>> php
    {
        // number literals
        { QColor(180, 120, 230), "\\b(\\d+|0x[0-9a-fA-F]+)\\b" },

        // other literals
        { QColor(180, 120, 230), "\\b(null|true|false)\\b" },

        // magic constants
        { QColor(180, 120, 230), "\\b(__(?:LINE|FILE|DIR|FUNCTION|CLASS|TRAIT|"
                                 "METHOD|NAMESPACE)__)\\b" },

        // functions
        { QColor(120, 220, 255), "\\b([a-zA-Z_][a-zA-Z0-9_]*)[^\\S\\n]*\\(" },

        // general keywords
        { QColor(255, 90, 100), "\\b(abstract|and|as|break|callable|case|catch|"
                                "class|clone|const|continue|declare|default|do|"
                                "echo|else|elseif|enddeclare|endforeach|endif|"
                                "endswitch|endwhile|extends|final|finally|for|"
                                "foreach|function|global|goto|if|implements|"
                                "include|include_once|instanceof|insteadof|"
                                "interface|namespace|new|or|print|private|"
                                "protected|public|require|require_once|return|"
                                "static|switch|throw|trait|try|use|void|while|"
                                "xor|yield)\\b" },

        // types
        { QColor(100, 200, 255), "\\b(array|binary|bool|boolean|char|double|"
                                 "float|int|integer|object|real|string|unset|"
                                 "var)\\b" },

        // string literals
        { QColor(100, 200, 100), "(\"(?:[^\\n\"]|\\\")*\"|"
                                 "'(?:[^\\n']|\\')*')" },

        // comments
        { QColor(130, 130, 130), "(/\\*(?:[^*]|\\*(?!/))*\\*/)" },
        { QColor(130, 130, 130), "(//[^\\n]*)" },
    };

    QVector<QPair<QColor, QString>> javascript
    {
        // number literals
        { QColor(180, 120, 230), "\\b(\\d+|0x[0-9a-fA-F]+)\\b" },

        // other literals
        { QColor(180, 120, 230), "\\b(null|true|false|undefined|Infinity|"
                                 "NaN)\\b" },

        // functions
        { QColor(120, 220, 255), "\\b([$a-zA-Z_][0-9a-zA-Z_$]*)\\(" },

        // general keywords
        { QColor(255, 90, 100), "\\b(abstract|boolean|break|byte|case|catch|"
                                "char|class|const|continue|debugger|default|"
                                "delete|do|double|else|enum|export|extends|"
                                "final|finally|float|for|function|goto|if|"
                                "implements|import|in|instanceof|int|interface|"
                                "long|native|new|package|private|protected|"
                                "public|return|short|static|super|switch|"
                                "synchronized|this|throw|throws|transient|try|"
                                "typeof|var|void|volatile|while|with)\\b" },

        // types
        { QColor(100, 200, 255), "\\b(array|object|var)\\b" },

        // string literals
        { QColor(100, 200, 100), "(\"(?:[^\\n\"]|\\\")*\"|"
                                 "'(?:[^\\n']|\\')*')" },

        // comments
        { QColor(130, 130, 130), "(/\\*(?:[^*]|\\*(?!/))*\\*/)" },
        { QColor(130, 130, 130), "(//[^\\n]*)" },
    };

    QVector<QPair<QColor, QString>> html
    {
        // tags
        { QColor(255, 90, 100), "<(\\s*[a-zA-Z0-9_]+)" },

        // end tags
        { QColor(255, 90, 100), "<\\s*/\\s*([a-zA-Z0-9_]+)\\s*>" },

        // tags' attributes
        { QColor(120, 220, 255), "<\\s*[a-zA-Z0-9_]+\\s+([^>]+)" },

        // attributes' values
        { QColor(100, 200, 100), "\\b[a-zA-Z0-9_-]+\\s*=\\s*((?:[a-zA-Z0-9_-]+)"
                                 "|(?:\"[a-zA-Z0-9_-]+\")|"
                                 "(?:'[a-zA-Z0-9_-]+'))" },

        // equal signs
        { QColor(255, 255, 255), "\\b[a-zA-Z0-9_-]+\\s*(=)" },
    };

    QVector<QPair<QColor, QString>> css
    {
        // units
        { QColor(255, 90, 100), "(%|cm|em|ex|in|mm|pc|pt|px|rem|vh|vw|vmin)" },

        // selectors - html elements
        { QColor(255, 90, 100), "(?:^|[\\s{,>~+])(\\*|a|abbr|acronym|address|applet|area|article|aside|audio|b|base|basefont|bdi|bdo|bgsound|big|blink|blockquote|body|br|button|canvas|caption|center|cite|code|col|colgroup|command|content|data|datalist|dd|del|details|dfn|dialog|dir|div|dl|dt|element|em|embed|fieldset|figcaption|figure|font|footer|form|frame|frameset|h[1-6]|head|header|hgroup|hr|html|i|iframe|image|img|input|ins|isindex|kbd|keygen|label|legend|li|link|listing|main|map|mark|marquee|menu|menuitem|meta|meter|multicol|nav|nobr|noembed|noframes|noscript|object|ol|optgroup|option|output|p|param|picture|plaintext|pre|progress|q|rp|rt|rtc|ruby|s|samp|script|section|select|shadow|slot|small|source|spacer|span|strike|strong|style|sub|summary|sup|table|tbody|td|template|textarea|tfoot|th|thead|time|title|tr|track|tt|u|ul|var|video|wbr|xmp)" },

        // selectors - ids and classes
        { QColor(100, 200, 100), "([.#][a-zA-Z0-9_-]+)" },

        // numbers
        { QColor(180, 120, 230), "[\\s:(,](-?(?:\\d+|\\.\\d+|\\d+\\.\\d+)|#[a-fA-F0-9]{3}|#[a-fA-F0-9]{6})" },

        // pseudo-selectorslinear
        { QColor(120, 220, 255), "(:(?:active|after|before|focus|hover|last-child|nth-child))" },

        // properties values
        { QColor(100, 200, 100), "(-webkit-focus-ring-color|absolute|alpha|always|antialiased|armenian|auto|avoid|baseline|bidi-override|blink|block|bold|bolder|border-box|both|bottom|capitalize|caption|center|circle|close-quote|collapse|crosshair|dashed|decimal|decimal-leading-zero|default|disc|dotted|e-resize|embed|fixed|georgian|grayscale|help|hidden|hide|icon|inherit|inline|inline-block|inline-table|inset|inside|invert|italic|justify|left|lighter|line-through|list-item|lower-roman|lower-greek|lower-latin|lower-alpha|lowercase|ltr|manipulation|menu|message-box|middle|move|n-resize|ne-resize|no-close-quote|no-open-quote|no-repeat|none|normal|nowrap|nw-resize|oblique|open-quote|outside|overline|pointer|pre|pre-line|pre-wrap|progress|relative|repeat|repeat-x|repeat-y|right|rtl|s-resize|scroll|se-resize|separate|show|small-caps|small-caption|solid|square|static|status-bar|sub|super|sw-resize|table|table-caption|table-cell|table-column|table-column-group|table-footer-group|table-header-group|table-row|table-row-group|text|text-bottom|text-top|thin|to|top|underline|upper-roman|upper-latin|upper-alpha|uppercase|visible|w-resize|wait)\\b" },

        // properties
        { QColor(120, 220, 255), "(?:[{;]\\s*)(-moz-box-sizing|-moz-osx-font-smoothing|-moz-user-select|-ms-filter|-ms-overflow-style|-ms-text-size-adjust|-ms-touch-action|-ms-transform|-ms-user-select|-o-transition|-webkit-animation|-webkit-box-shadow|-webkit-box-sizing|-webkit-font-smoothing|-webkit-text-size-adjust|-webkit-transform|-webkit-transition|-webkit-user-select|align-content|align-items|align-self|animation|background|background-attachment|background-blend-mode|background-clip|background-color|background-image|background-origin|background-position|background-repeat|background-size|border|border-bottom|border-bottom-color|border-bottom-left-radius|border-bottom-right-radius|border-bottom-style|border-bottom-width|border-collapse|border-color|border-image|border-image-outset|border-image-repeat|border-image-slice|border-image-source|border-image-width|border-left|border-left-color|border-left-style|border-left-width|border-radius|border-right|border-right-color|border-right-style|border-right-width|border-shadow|border-spacing|border-style|border-top|border-top-color|border-top-left-radius|border-top-right-radius|border-top-style|border-top-width|border-width|bottom|box-shadow|box-sizing|caption-side|clear|clip|color|content|counter-increment|counter-reset|cursor|direction|display|empty-cells|filter|flex|flex-basis|flex-direction|flex-flow|flex-grow|flex-shrink|flex-wrap|float|font|font-family|font-size|font-style|font-variant|font-weight|height|justify-content|left|letter-spacing|line-height|list-style|list-style-image|list-style-position|list-style-type|margin|margin-bottom|margin-left|margin-right|margin-left|max-height|max-width|min-height|min-width|opacity|order|orphans|outline|outline-color|outline-offset|outline-style|outline-width|overflow|overflow-x|overflow-y|padding|padding-bottom|padding-left|padding-right|padding-top|page-break-after|page-break-before|page-break-inside|pointer-events|position|quotes|resize|right|src|table-layout|text-align|text-decoration|text-decoration-color|text-decoration-line|text-decoration-style|text-indent|text-overflow|text-rendering|text-shadow|text-transform|transform|transition|transition-delay|transition-duration|transition-property|transition-timing-function|top|touch-action|unicode-bidi|user-select|vertical-align|visibility|white-space|widows|width|word-break|word-spacing|word-wrap|z-index)" },

        // at-rules
        { QColor(255, 90, 100), "(@(?:charset|font-face|import|media|page))" },

        // functions
        { QColor(120, 220, 255), "[\\s:(,](-moz-linear-gradient|-o-linear-gradient|-webkit-linear-gradient|linear-gradient|rgba?|url)\\(" },

        // string literals
        { QColor(100, 200, 100), "(\"(?:[^\\n\"]|\\\")*\"|"
                                 "'(?:[^\\n']|\\')*')" },

        // others
        { QColor(255, 90, 100), "(!important)" },
        { QColor(100, 200, 100), "(even|odd)" },

        // comments
        { QColor(130, 130, 130), "(/\\*(?:[^*]|\\*(?!/))*\\*/)" },
    };
};

#endif // _T_EDITOR_SH_RULES_H
