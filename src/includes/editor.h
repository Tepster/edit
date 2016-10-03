#ifndef _T_EDITOR_H
#define _T_EDITOR_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>

#include <QKeyEvent>
#include <QFocusEvent>

#include <QTimer>

#include <QChar>
#include <QString>
#include <QStringList>

#include <QFont>
#include <QColor>
#include <QPoint>

#include "controls.h"


namespace _t
{
    class editor;
}

class _t::editor : public QWidget
{
    Q_OBJECT

    struct coords
    {
    private:
        QStringList *text;

        qint32 _row = 0;
        qint32 _col = 0;

    public:
        coords() {}
        coords(QStringList *text) : text(text) {}

        qint32 row() const;
        qint32 col() const;

        coords *set_text(QStringList *text);

        coords *set_row(qint32 row);
        coords *set_col(qint32 col);
        coords *set(qint32 row, qint32 col);

        coords &operator++();
        coords operator++(int);

        coords &operator--();
        coords operator--(int);

        coords operator+(quint32 value);
        coords operator-(quint32 value);

        coords &operator+=(quint32 value);
        coords &operator-=(quint32 value);
    };

    struct cursor
    {
        qint32 row = 0;
        qint32 col = 0;

        QPixmap background;
    };

    _t::controls::label area;
    QPixmap canvas;

    QColor background;

    QFont font;
    QColor font_color;

    qint32 cell_width = 9;
    qint32 cell_height = 18;

    cursor cursor;
    QTimer cursor_timer;
    bool cursor_visible = false;

    QStringList text;

    QChar newline_character = '\n';


    void keyPressEvent(QKeyEvent *event);
    void focusInEvent(QFocusEvent *event);
    void focusOutEvent(QFocusEvent *event);

    QString &active_line();

    void setup_painter_clear(QPainter &painter);
    void setup_painter_write(QPainter &painter);

    void clear_cell(QPainter &painter, qint32 row, qint32 col);
    void clear_cell(QPainter &painter);

    void update();

    void cursor_show();
    void cursor_hide();
    void cursor_activate();
    void cursor_deactivate();

private slots:
    void area_clicked(QPoint coords);
    void cursor_timer_tick();

public:
    void init(QWidget *parent_widget);

    void write(const QString &text);
};

#endif // _T_EDITOR_H
