#include "editor/drawing_manager.h"


void _t::editor::drawing_manager::init(
    const QColor *background,
    const QFont *font,
    const QColor *font_color,
    QSize *cell_size,
    const QPoint *shift,
    QPixmap *canvas)
{
    this->background = background;
    this->font = font;
    this->font_color = font_color;
    this->cell_size = cell_size;
    this->shift = shift;

    if (canvas)
    {
        this->init_canvas(canvas);
    }
}

void _t::editor::drawing_manager::init_canvas(QPixmap *canvas)
{
    this->canvas = canvas;

    this->canvas->fill(*this->background);

    painter.begin(this->canvas);
}

void _t::editor::drawing_manager::end()
{
    if (this->painter.isActive())
    {
        this->painter.end();
    }
}


void _t::editor::drawing_manager::setup_clearing()
{
    this->setup_clearing(*this->background);
}

void _t::editor::drawing_manager::setup_clearing(const QColor &color)
{
    this->painter.setPen(color);
    this->painter.setBrush(QBrush(color));
}

void _t::editor::drawing_manager::setup_writing()
{
    this->setup_writing(*this->font_color);
}

void _t::editor::drawing_manager::setup_writing(const QColor &color)
{
    this->painter.setFont(*this->font);
    this->painter.setPen(color);
}


void _t::editor::drawing_manager::draw_char(
    const coordinates &coords,
    const QChar &character)
{
    this->draw_char(coords, character, *this->background);
}


void _t::editor::drawing_manager::draw_char(
    const coordinates &coords,
    const QChar &character,
    const QColor &background)
{
    this->draw_char(coords, character, background, *this->font_color);
}

void _t::editor::drawing_manager::draw_char(
    const coordinates &coords,
    const QChar &character,
    const QColor &background,
    const QColor &foreground)
{
    this->clear(coords, background);

    this->setup_writing(foreground);

    this->painter.drawText(
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        this->cell_size->width(),
        this->cell_size->height(),
        Qt::AlignCenter,
        character);
}

void _t::editor::drawing_manager::draw_cursor(const coordinates &coords)
{
    this->painter.fillRect(
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row *  this->cell_size->height() - this->shift->y(),
        2,
        this->cell_size->height(),
        QColor(50, 170, 110));
}

void _t::editor::drawing_manager::draw_pixmap(
    const coordinates &coords,
    const QPixmap &pixmap)
{
    this->painter.drawPixmap(
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        this->cell_size->width(),
        this->cell_size->height(),
        pixmap);
}

void _t::editor::drawing_manager::draw_pixmap(const QPixmap &pixmap)
{
    this->painter.drawPixmap(0, 0, pixmap);
}


void _t::editor::drawing_manager::move(
    const coordinates &src,
    qint32 num,
    const coordinates &dst)
{
    if (num <= 0 || src == dst)
    {
        return;
    }

    this->painter.drawPixmap(
        dst.col * this->cell_size->width() - this->shift->x(),
        dst.row * this->cell_size->height() - this->shift->y(),
        *this->canvas,
        src.col * this->cell_size->width() - this->shift->x(),
        src.row * this->cell_size->height() - this->shift->y(),
        num * this->cell_size->width(),
        this->cell_size->height());

    this->setup_clearing();

    // shifting left
    if (src.row == dst.row && src.col > dst.col)
    {
        painter.drawRect(
            (dst.col + num) * this->cell_size->width() - this->shift->x(),
            src.row * this->cell_size->height() - this->shift->y(),
            (src.col - dst.col) * this->cell_size->width(),
            this->cell_size->height());
    }
    else
    {
        painter.drawRect(
            src.col * this->cell_size->width() - this->shift->x(),
            src.row * this->cell_size->height() - this->shift->y(),
            num * this->cell_size->width(),
            this->cell_size->height());
    }
}

void _t::editor::drawing_manager::shift_up(
    const coordinates &coords,
    qint32 num)
{
    if (coords.row == 0 || num <= 0)
    {
        return;
    }

    this->move(coords, num, coordinates(coords.row - 1, coords.col));
}

void _t::editor::drawing_manager::shift_down(
    const coordinates &coords,
    qint32 num)
{
    if (num <= 0)
    {
        return;
    }

    this->move(coords, num, coordinates(coords.row + 1, coords.col));
}

void _t::editor::drawing_manager::shift_left(
    const coordinates &coords,
    qint32 num)
{
    if (coords.col == 0 || num <= 0)
    {
        return;
    }

    this->painter.drawPixmap(
        (coords.col - 1) * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        *this->canvas,
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        num * this->cell_size->width(),
        this->cell_size->height());

    this->setup_clearing();

    this->painter.drawRect(
        (coords.col + num - 1) * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        1 * this->cell_size->width(),
        this->cell_size->height());
}

void _t::editor::drawing_manager::shift_right(
    const coordinates &coords,
    qint32 num)
{
    if (num <= 0)
    {
        return;
    }

    this->painter.drawPixmap(
        (coords.col + 1) * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        *this->canvas,
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        num * this->cell_size->width(),
        this->cell_size->height());

    this->setup_clearing();

    this->painter.drawRect(
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        this->cell_size->width(),
        this->cell_size->height());
}


void _t::editor::drawing_manager::clear(const coordinates &coords)
{
    this->clear(coords, *this->background);
}

void _t::editor::drawing_manager::clear(
    const coordinates &coords,
    const QColor &color)
{
    this->setup_clearing(color);

    this->painter.drawRect(
        coords.col * this->cell_size->width() - this->shift->x(),
        coords.row * this->cell_size->height() - this->shift->y(),
        this->cell_size->width(),
        this->cell_size->height());
}
