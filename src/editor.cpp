#include "editor.h"

#include <QApplication>
#include <QClipboard>

#include <QChar>


void _t::editor::init(QWidget *parent_widget)
{
    this->setParent(parent_widget);

    // todo: start maximized
    this->resize(1280, 720);

    this->background = QColor(60, 60, 65);
    this->selection_background = QColor(100, 100, 110);

    this->font = QFont("Consolas", 12);
    this->font_color = QColor(250, 250, 250);

    this->cursor_timer.setParent(this);
    connect(
        &this->cursor_timer,
        SIGNAL(timeout()),
        this,
        SLOT(cursor_timer_tick()));
    this->cursor_timer.setInterval(500);

    this->area.setParent(this);
    this->area.resize(this->width(), this->height());
    connect(
        &this->area,
        SIGNAL(mouse_pressed(QMouseEvent *)),
        this,
        SLOT(mouse_press(QMouseEvent *)));

    this->area.setMouseTracking(true);
    connect(
        &this->area,
        SIGNAL(mouse_moved(QMouseEvent *)),
        this,
        SLOT(mouse_move(QMouseEvent *)));

    // todo: same size as 'this'
    this->canvas = QPixmap(1280, 720);
    this->canvas.fill(this->background);

    this->setCursor(Qt::IBeamCursor);

    this->update();

    this->text << "";

    this->_cursor.coords.set_text(&this->text);

    this->setFocus();
}


void _t::editor::keyPressEvent(QKeyEvent *event)
{
    // home, end, left, up, right, down, pageup, pagedown
    if (event->key() >= 0x01000010 && event->key() <= 0x01000017)
    {
        this->cursor_deactivate();

        bool selection;
        if (event->modifiers() != Qt::ShiftModifier)
        {
            this->deselect();
            selection = false;
        }
        else
        {
            selection = true;
        }


        switch (event->key())
        {
        case Qt::Key_Left:
            this->cursor_move(this->_cursor.coords - 1, selection);
            break;

        case Qt::Key_Up:
            this->cursor_move(
                coordinates(
                    this->_cursor.row() > 0 ? this->_cursor.row() - 1 : 0,
                    this->_cursor.col()),
                selection);
            break;

        case Qt::Key_Right:
            this->cursor_move(this->_cursor.coords + 1, selection);
            break;

        case Qt::Key_Down:
            this->cursor_move(
                coordinates(this->_cursor.row() + 1, this->_cursor.col()),
                selection);
            break;

        case Qt::Key_Home:
        {
            quint32 i = 0;

            // go with /i/ to beginning of non-white characters
            while (i < this->active_line().length()
                && this->active_line().at(i) == ' ')
            {
                ++i;
            }

            // cursor is in non-white characters area
            // OR at the beginning of the line
            if (i < this->_cursor.col() || this->_cursor.col() == 0)
            {
                // move it to /i/
                this->cursor_move(
                    coordinates(this->_cursor.row(), i),
                    selection);
            }
            else
            {
                // move to the beginning of the line
                this->cursor_move(
                    coordinates(this->_cursor.row(), 0),
                    selection);
            }
        }
            break;

        case Qt::Key_End:
            this->cursor_move(
                coordinates(this->_cursor.row(), this->active_line().length()),
                selection);
            break;

        case Qt::Key_PageUp:
            //this->_cursor.move(some_coords);
            break;

        case Qt::Key_PageDown:
            //this->_cursor.move(some_coords);
            break;
        }

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        this->write(this->newline_character);
    }

    else if (event->key() == Qt::Key_Backspace)
    {
        this->cursor_deactivate();

        // cursor is not at the beginning of the line
        if (this->_cursor.col() > 0)
        {
            --this->_cursor.col();

            QPainter painter(&this->canvas);
            this->setup_painter_clear(painter);

            this->clear_cell(painter);

            // cursor is not at the end of the line
            if (this->_cursor.col() + 1 < this->active_line().length())
            {
                // move the rest of the active line one cell left
                painter.drawPixmap(
                    this->_cursor.col() * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->canvas,
                    (this->_cursor.col() + 1) * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    (this->active_line().length() - this->_cursor.col())
                        * this->cell_width,
                    this->cell_height);

                this->clear_cell(
                    painter,
                    coordinates(
                        this->_cursor.row(),
                        this->active_line().length() - 1));
            }

            this->update();

            this->active_line().remove(this->_cursor.col(), 1);
        }

        // cursor is at the beginning of the line
        else
        {
            if (this->_cursor.row() > 0)
            {
                --this->_cursor.row();
                this->_cursor.col() = this->active_line().length();

                QPainter painter(&this->canvas);
                this->setup_painter_clear(painter);

                // original line isn't empty
                if (this->text[this->_cursor.row() + 1].length() > 0)
                {
                    // move the remaining text to the end of the line
                    painter.drawPixmap(
                        this->_cursor.col() * this->cell_width,
                        this->_cursor.row() * this->cell_height,
                        this->canvas,
                        0,
                        (this->_cursor.row() + 1) * this->cell_height,
                        this->text[this->_cursor.row() + 1].length()
                            * this->cell_width,
                        this->cell_height);

                    painter.drawRect(
                        0,
                        (this->_cursor.row() + 1) * this->cell_height,
                        this->text[this->_cursor.row() + 1].length()
                            * this->cell_width,
                        this->cell_height);

                    this->active_line()
                        .append(this->text[this->_cursor.row() + 1]);
                }

                // move all following lines one row up
                for (qint32 i = this->_cursor.row() + 2;
                     i < this->text.count();
                     ++i)
                {
                    qint32 line_length = this->text[i].length();

                    painter.drawPixmap(
                        0,
                        (i - 1) * this->cell_height,
                        this->canvas,
                        0,
                        i * this->cell_height,
                        line_length * this->cell_width,
                        this->cell_height);

                    painter.drawRect(
                        0,
                        i * this->cell_height,
                        line_length * this->cell_width,
                        this->cell_height);
                }

                this->update();

                this->text.removeAt(this->_cursor.row() + 1);
            }
        }

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Delete)
    {
        this->cursor_deactivate();

        // cursor is not at the end of the line
        if (this->_cursor.col() < this->active_line().length())
        {
            QPainter painter(&this->canvas);
            this->setup_painter_clear(painter);

            this->clear_cell(painter);

            // cursor is not before the last letter in the line
            if (this->_cursor.col() + 1 < this->active_line().length())
            {
                // move the rest of the active line one cell left
                painter.drawPixmap(
                    this->_cursor.col() * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->canvas,
                    (this->_cursor.col() + 1) * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    (this->active_line().length() - this->_cursor.col())
                        * this->cell_width,
                    this->cell_height);

                this->clear_cell(
                    painter,
                    coordinates(
                        this->_cursor.row(),
                        this->active_line().length() - 1));
            }

            this->update();

            this->active_line().remove(this->_cursor.col(), 1);
        }

        // cursor is at the end of the line
        else
        {
            // cursor is not at the last line
            if (this->_cursor.row() + 1 < this->text.length())
            {
                QPainter painter(&this->canvas);
                this->setup_painter_clear(painter);

                // next line isn't empty
                if (this->text[this->_cursor.row() + 1].length() > 0)
                {
                    // move the next line to the end of this line
                    painter.drawPixmap(
                        this->_cursor.col() * this->cell_width,
                        this->_cursor.row() * this->cell_height,
                        this->canvas,
                        0,
                        (this->_cursor.row() + 1) * this->cell_height,
                        this->text[this->_cursor.row() + 1].length()
                            * this->cell_width,
                        this->cell_height);

                    painter.drawRect(
                        0,
                        (this->_cursor.row() + 1) * this->cell_height,
                        this->text[this->_cursor.row() + 1].length()
                            * this->cell_width,
                        this->cell_height);

                    this->active_line()
                        .append(this->text[this->_cursor.row() + 1]);
                }

                // move all following lines one row up
                for (qint32 i = this->_cursor.row() + 2;
                     i < this->text.count();
                     ++i)
                {
                    qint32 line_length = this->text[i].length();

                    painter.drawPixmap(
                        0,
                        (i - 1) * this->cell_height,
                        this->canvas,
                        0,
                        i * this->cell_height,
                        line_length * this->cell_width,
                        this->cell_height);

                    painter.drawRect(
                        0,
                        i * this->cell_height,
                        line_length * this->cell_width,
                        this->cell_height);
                }

                this->update();

                this->text.removeAt(this->_cursor.row() + 1);
            }
        }

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Tab)
    {
        this->write("    ");
    }

    else if (event->key() == Qt::Key_V)
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            this->write(QApplication::clipboard()->text());
        }
        else
        {
            this->write(event->text());
        }
    }

    else if (event->key() == Qt::Key_Escape)
    {
        this->cursor_deactivate();

        this->deselect();

        this->cursor_activate();
    }

    else if (!event->text().isEmpty())
    {
        this->write(event->text());
    }
}

void _t::editor::mouse_press(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
    {
        return;
    }

    this->cursor_deactivate();

    if (QApplication::keyboardModifiers() != Qt::ShiftModifier)
    {
        this->deselect();
    }

    qint32 row = event->localPos().y() / this->cell_height;
    qint32 col = event->localPos().x() / this->cell_width;
    if ((qint32)event->localPos().x() % this->cell_width > this->cell_width / 2)
    {
        ++col;
    }

    this->cursor_move(
        coordinates(row, col),
        QApplication::keyboardModifiers() == Qt::ShiftModifier);

    this->cursor_activate();
}

void _t::editor::mouse_move(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
    {
        return;
    }

    this->cursor_deactivate();

    qint32 row = event->localPos().y() / this->cell_height;
    qint32 col = event->localPos().x() / this->cell_width;
    if ((qint32)event->localPos().x() % this->cell_width > this->cell_width / 2)
    {
        ++col;
    }

    this->cursor_move(coordinates(row, col), true);

    this->cursor_activate();
}

void _t::editor::focusInEvent(QFocusEvent *)
{
    this->cursor_activate();
}

void _t::editor::focusOutEvent(QFocusEvent *)
{
    this->cursor_deactivate();
}


void _t::editor::write(const QString &text)
{
    this->cursor_deactivate();

    QPainter painter(&this->canvas);

    for (QChar character : text)
    {
        if (character == this->newline_character)
        {
            qint32 active_line_rest_length = this->active_line().length()
                - this->_cursor.col();

            this->setup_painter_clear(painter);

            // cursor is not at the last line
            if (this->_cursor.row() + 1 < this->text.count())
            {
                // move all following lines one row down
                for (qint32 i = this->text.count() - 1;
                     i > this->_cursor.row();
                     --i)
                {
                    painter.drawPixmap(
                        0,
                        (i + 1) * this->cell_height,
                        this->canvas,
                        0,
                        i * this->cell_height,
                        this->cell_width * this->text[i].length(),
                        this->cell_height);

                    painter.drawRect(
                        0,
                        i * this->cell_height,
                        this->cell_width * this->text[i].length(),
                        this->cell_height);
                }
            }

            // cursor is at the end of the line
            if (this->_cursor.col() == this->active_line().length())
            {
                this->text.insert(this->_cursor.row() + 1, "");
            }
            else
            {
                // create a new line with the rest of the original line
                this->text.insert(
                    this->_cursor.row() + 1,
                    this->active_line().right(active_line_rest_length));

                this->active_line().chop(active_line_rest_length);

                // move the rest of the active line
                // to the beginning of the next line
                painter.drawPixmap(
                    0,
                    (this->_cursor.row() + 1) * this->cell_height,
                    this->canvas,
                    this->_cursor.col() * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->cell_width * active_line_rest_length,
                    this->cell_height);

                painter.drawRect(
                    this->_cursor.col() * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->cell_width * active_line_rest_length,
                    this->cell_height);
            }

            ++this->_cursor.row();
            this->_cursor.col() = 0;
        }

        // character is not a newline
        else
        {
            // cursor is not at the end of line
            if (this->_cursor.col() < this->active_line().length())
            {
                this->setup_painter_clear(painter);

                // move the rest of the active line one cell right
                painter.drawPixmap(
                    (this->_cursor.col() + 1) * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->canvas,
                    this->_cursor.col() * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->cell_width
                        * (this->active_line().length() - this->_cursor.col()),
                    this->cell_height);

                painter.drawRect(
                    this->_cursor.col() * this->cell_width,
                    this->_cursor.row() * this->cell_height,
                    this->cell_width,
                    this->cell_height);
            }

            this->setup_painter_write(painter);

            painter.drawText(
                this->_cursor.col() * this->cell_width,
                this->_cursor.row() * this->cell_height,
                this->cell_width,
                this->cell_height,
                0,
                character);

            this->active_line().insert(this->_cursor.col(), character);

            ++this->_cursor.col();
        }
    }

    painter.end();

    this->update();

    this->cursor_activate();
}


QString &_t::editor::active_line()
{
    return this->text[this->_cursor.row()];
}


void _t::editor::setup_painter_clear(QPainter &painter)
{
    painter.setPen(this->background);
    painter.setBrush(QBrush(this->background));
}

void _t::editor::setup_painter_write(QPainter &painter)
{
    painter.setFont(this->font);
    painter.setPen(this->font_color);
}


void _t::editor::clear_cell(QPainter &painter, const coordinates &coords)
{
    painter.drawRect(
        coords.col * this->cell_width,
        coords.row * this->cell_height,
        this->cell_width,
        this->cell_height);
}

void _t::editor::clear_cell(QPainter &painter)
{
    this->clear_cell(painter, this->_cursor.coords);
}


void _t::editor::deselect()
{
    if (!this->_cursor.selection_mode)
    {
        return;
    }

    this->_cursor.selection_mode = false;

    coordinates from, to;

    // selection goes in forward direction
    if (this->_cursor.coords > this->_cursor.selection_from)
    {
        from = this->_cursor.selection_from;
        to = this->_cursor.coords - 1;
    }
    else
    {
        from = this->_cursor.coords;
        to = this->_cursor.selection_from;
    }

    this->each_cell(from, to, [&](const coordinates &coords)
    {
        this->draw_deselected_cell(coords);
    });
}


void _t::editor::draw_selected_cell()
{
    this->draw_selected_cell(this->_cursor.coords);
}

void _t::editor::draw_selected_cell(const _t::editor::coordinates &coords)
{
    this->redraw_cell_bg(coords, this->selection_background);
}

void _t::editor::draw_deselected_cell()
{
    this->draw_deselected_cell(this->_cursor.coords);
}

void _t::editor::draw_deselected_cell(const _t::editor::coordinates &coords)
{
    this->redraw_cell_bg(coords, this->background);
}


void _t::editor::redraw_cell_bg(
    const _t::editor::coordinates &coords,
    const QColor &color)
{
    QPainter painter(&this->canvas);
    painter.setPen(color);
    painter.setBrush(QBrush(color));

    painter.drawRect(
        coords.col * this->cell_width,
        coords.row * this->cell_height,
        this->cell_width,
        this->cell_height);

    this->setup_painter_write(painter);

    painter.drawText(
        coords.col * this->cell_width,
        coords.row * this->cell_height,
        this->cell_width,
        this->cell_height,
        0,
        QChar(this->text[coords.row][coords.col]));
}


void _t::editor::update()
{
    this->area.setPixmap(this->canvas);
}


void _t::editor::cursor_move(
    const _t::editor::coordinates &coords,
    bool selection)
{
    coordinates target_coords = coords;
    if (!target_coords.text)
    {
        target_coords.set_text(&this->text);
    }

    // should proceed with selection
    if (selection)
    {
        // target coordinates are behind current cursor coordinates
        if (target_coords > this->_cursor.coords)
        {
            // nothing is selected
            // OR selection started before current cursor coordinates
            if (!this->_cursor.selection_mode
                || this->_cursor.selection_from < this->_cursor.coords)
            {
                // initiate selection
                if (!this->_cursor.selection_mode)
                {
                    this->_cursor.selection_from = this->_cursor.coords;
                    this->_cursor.selection_mode = true;
                }

                this->each_cell(this->_cursor.coords, target_coords - 1,
                    [&](const coordinates &coords)
                    {
                        this->draw_selected_cell(coords);
                    });
            }

            // selection started behind or at current cursor coordinates
            else
            {
                // target coordinates are before or at original selection start
                if (target_coords <= this->_cursor.selection_from)
                {
                    this->each_cell(this->_cursor.coords, target_coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });
                }

                // target coordinates are where the selection started
                else if (target_coords == this->_cursor.selection_from + 1)
                {
                    this->_cursor.selection_mode = false;

                    // deselect whole selection
                    this->each_cell(
                        this->_cursor.coords,
                        this->_cursor.selection_from,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });
                }

                // target coordinates are behind selection start
                else
                {
                    // deselect the original selection
                    this->each_cell(
                        this->_cursor.coords,
                        this->_cursor.selection_from,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });

                    ++this->_cursor.selection_from;

                    this->each_cell(
                        this->_cursor.selection_from,
                        target_coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_selected_cell(coords);
                        });
                }
            }
        }

        // target coordinates are before current cursor coordinates
        else if (target_coords != this->_cursor.coords)
        {
            // nothing is selected
            // OR selection started behind or at current cursor coordinates
            if (!this->_cursor.selection_mode
                || this->_cursor.selection_from >= this->_cursor.coords)
            {
                // initiate selection
                if (!this->_cursor.selection_mode)
                {
                    this->_cursor.selection_from = this->_cursor.coords - 1;
                    this->_cursor.selection_mode = true;
                }

                this->each_cell(target_coords, this->_cursor.coords - 1,
                    [&](const coordinates &coords)
                    {
                        this->draw_selected_cell(coords);
                    });
            }

            // selection started before current cursor coordinates
            else
            {
                // target coordinates are after original selection start
                if (target_coords > this->_cursor.selection_from)
                {
                    this->each_cell(target_coords, this->_cursor.coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });
                }

                // target coordinates are where the selection started
                else if (target_coords == this->_cursor.selection_from)
                {
                    this->_cursor.selection_mode = false;

                    // deselect whole selection
                    this->each_cell(
                        this->_cursor.selection_from,
                        this->_cursor.coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });
                }

                // target coordinates are before selection start
                else
                {
                    // deselect the original selection
                    this->each_cell(
                        this->_cursor.selection_from,
                        this->_cursor.coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });

                    --this->_cursor.selection_from;

                    this->each_cell(target_coords, this->_cursor.selection_from,
                        [&](const coordinates &coords)
                        {
                            this->draw_selected_cell(coords);
                        });
                }
            }
        }
    }

    this->_cursor.coords = target_coords;
}


void _t::editor::cursor_show()
{
    if (!this->cursor_visible)
    {
        this->_cursor.background = this->canvas.copy(
            this->_cursor.col() * this->cell_width,
            this->_cursor.row() * this->cell_height,
            this->cell_width,
            this->cell_height);

        QPainter painter(&this->canvas);

        painter.fillRect(
            this->_cursor.col() * this->cell_width,
            this->_cursor.row() * this->cell_height,
            2,
            this->cell_height,
            QColor(50, 170, 110));

        this->update();

        this->cursor_visible = true;
    }
}

void _t::editor::cursor_hide()
{
    if (this->cursor_visible)
    {
        QPainter painter(&this->canvas);

        painter.drawPixmap(
            this->_cursor.col() * this->cell_width,
            this->_cursor.row() * this->cell_height,
            this->cell_width,
            this->cell_height,
            this->_cursor.background);

        this->update();

        this->cursor_visible = false;
    }
}

void _t::editor::cursor_activate()
{
    this->cursor_show();

    this->cursor_timer.start();
}

void _t::editor::cursor_deactivate()
{
    this->cursor_timer.stop();

    this->cursor_hide();
}


void _t::editor::cursor_timer_tick()
{
    if (this->cursor_visible)
    {
        this->cursor_hide();
    }
    else
    {
        this->cursor_show();
    }
}


void _t::editor::each_cell(
    const coordinates &start,
    const coordinates &end,
    std::function<void(const coordinates &)> func)
{
    coordinates coords = start;

    if (!coords.text)
    {
        coords.set_text(&this->text);
    }

    while (coords != end)
    {
        func(coords);
        ++coords;
    }

    func(coords);
}
