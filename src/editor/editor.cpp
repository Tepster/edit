#include "editor/editor.h"

#include <QApplication>
#include <QClipboard>

#include <QChar>


void _t::editor::editor::init(QWidget *parent_widget)
{
    this->setParent(parent_widget);

    // todo: start maximized
    this->resize(1280, 720);

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

    this->cell_size = QSize(9, 18);
    this->selection_background = QColor(100, 100, 110);

    this->painter.init(
        &this->canvas,
        QColor(60, 60, 65),
        QFont("Consolas", 12),
        QColor(250, 250, 250),
        &this->cell_size);

    this->setCursor(Qt::IBeamCursor);

    this->update();

    this->text << "";

    this->cursor.coords.set_text(&this->text);

    this->setFocus();
}


void _t::editor::editor::keyPressEvent(QKeyEvent *event)
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
            this->cursor_move(this->cursor.coords - 1, selection);
            break;

        case Qt::Key_Up:
            this->cursor_move(
                coordinates(
                    this->cursor.row() > 0 ? this->cursor.row() - 1 : 0,
                    this->cursor.col()),
                selection);
            break;

        case Qt::Key_Right:
            this->cursor_move(this->cursor.coords + 1, selection);
            break;

        case Qt::Key_Down:
            this->cursor_move(
                coordinates(this->cursor.row() + 1, this->cursor.col()),
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
            if (i < this->cursor.col() || this->cursor.col() == 0)
            {
                // move it to /i/
                this->cursor_move(
                    coordinates(this->cursor.row(), i),
                    selection);
            }
            else
            {
                // move to the beginning of the line
                this->cursor_move(
                    coordinates(this->cursor.row(), 0),
                    selection);
            }
        }
            break;

        case Qt::Key_End:
            this->cursor_move(
                coordinates(this->cursor.row(), this->active_line().length()),
                selection);
            break;

        case Qt::Key_PageUp:
            //this->cursor.move(some_coords);
            break;

        case Qt::Key_PageDown:
            //this->cursor.move(some_coords);
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

        this->delete_char(--this->cursor.coords);

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Delete)
    {
        this->cursor_deactivate();

        this->delete_char(this->cursor.coords);

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Tab)
    {
        this->write("    ");
    }

    else if (event->key() == Qt::Key_C)
    {
        if (event->modifiers() == Qt::ControlModifier)
        {
            if (this->cursor.selection_mode)
            {
                QString text;

                coordinates begin;
                coordinates end;

                if (this->cursor.selection_from < this->cursor.coords)
                {
                    begin = this->cursor.selection_from;
                    end = this->cursor.coords - 1;
                }
                else
                {
                    begin = this->cursor.coords;
                    end = this->cursor.selection_from;
                }

                this->each_cell(begin, end, [&](const coordinates &coords)
                {
                    if (this->text[coords.row].length() > coords.col)
                    {
                        text += this->text[coords.row][coords.col];
                    }
                    else
                    {
                        text += this->newline_character;
                    }
                });

                QApplication::clipboard()->setText(text);
            }
        }
        else
        {
            this->write(event->text());
        }
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

void _t::editor::editor::mouse_press(QMouseEvent *event)
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

    qint32 row = event->localPos().y() / this->cell_size.height();
    qint32 col = event->localPos().x() / this->cell_size.width();
    if ((qint32)event->localPos().x() % this->cell_size.width()
        > this->cell_size.width() / 2)
    {
        ++col;
    }

    this->cursor_move(
        coordinates(row, col),
        QApplication::keyboardModifiers() == Qt::ShiftModifier);

    this->cursor_activate();
}

void _t::editor::editor::mouse_move(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
    {
        return;
    }

    this->cursor_deactivate();

    qint32 row = event->localPos().y() / this->cell_size.height();
    qint32 col = event->localPos().x() / this->cell_size.width();

    if (row < 0)
    {
        row = 0;
    }

    if (col < 0)
    {
        col = 0;
    }
    else if ((qint32)event->localPos().x() % this->cell_size.width()
        > this->cell_size.width() / 2)
    {
        ++col;
    }

    this->cursor_move(coordinates(row, col), true);

    this->cursor_activate();
}

void _t::editor::editor::focusInEvent(QFocusEvent *)
{
    this->cursor_activate();
}

void _t::editor::editor::focusOutEvent(QFocusEvent *)
{
    this->cursor_deactivate();
}


void _t::editor::editor::write(const QString &text)
{
    this->cursor_deactivate();

    for (QChar character : text)
    {
        qint32 line_rest_length = this->active_line().length()
            - this->cursor.col();

        if (character == this->newline_character)
        {
            // cursor is not at the last line
            if (this->cursor.row() + 1 < this->text.count())
            {
                // move all following lines one row down
                for (qint32 i = this->text.count() - 1;
                     i > this->cursor.row();
                     --i)
                {
                    this->painter.shift_down(
                        coordinates(i, 0),
                        this->text.at(i).length());
                }
            }

            // cursor is at the end of the line
            if (this->cursor.col() == this->active_line().length())
            {
                this->text.insert(this->cursor.row() + 1, "");
            }
            else
            {
                // create a new line with the rest of the original line
                this->text.insert(
                    this->cursor.row() + 1,
                    this->active_line().right(line_rest_length));

                this->active_line().chop(line_rest_length);

                // move the rest of the active line
                // to the beginning of the next line
                this->painter.move(
                    this->cursor.coords,
                    line_rest_length,
                    coordinates(this->cursor.row() + 1, 0));
            }

            ++this->cursor.row();
            this->cursor.col() = 0;
        }

        // character is not a newline
        else
        {
            // cursor is not at the end of the line
            if (this->cursor.col() < this->active_line().length())
            {
                this->painter.shift_right(
                    this->cursor.coords,
                    line_rest_length);
            }

            this->painter.draw_char(this->cursor.coords, character);

            this->active_line().insert(this->cursor.col(), character);

            ++this->cursor.col();
        }
    }

    this->update();

    this->cursor_activate();
}


QString &_t::editor::editor::active_line()
{
    return this->text[this->cursor.row()];
}


void _t::editor::editor::deselect()
{
    if (!this->cursor.selection_mode)
    {
        return;
    }

    this->cursor.selection_mode = false;

    coordinates from, to;

    // selection goes in forward direction
    if (this->cursor.coords > this->cursor.selection_from)
    {
        from = this->cursor.selection_from;
        to = this->cursor.coords - 1;
    }
    else
    {
        from = this->cursor.coords;
        to = this->cursor.selection_from;
    }

    this->each_cell(from, to, [&](const coordinates &coords)
    {
        this->draw_deselected_cell(coords);
    });
}


void _t::editor::editor::delete_char(const coordinates &coords)
{
    // coords not at the end of the line
    if (coords.col < this->text.at(coords.row).length())
    {
        this->painter.clear(coords);

        this->painter.shift_left(
            coords,
            this->text.at(coords.row).length() - coords.col);

        this->update();

        this->active_line().remove(coords.col, 1);
    }

    // coords at the end of the line
    else
    {
        // there is a line below this
        if (this->text.count() - 1 > coords.row)
        {
            // next line isn't empty
            if (this->text.at(coords.row + 1).length() > 0)
            {
                this->painter.move(
                    coordinates(coords.row + 1, 0),
                    this->text.at(coords.row + 1).length(),
                    coords);

                this->text[coords.row].append(this->text.at(coords.row + 1));
            }

            // move all following lines one row up
            for (qint32 i = coords.row + 2; i < this->text.count(); ++i)
            {
                this->painter.shift_up(
                    coordinates(i, 0),
                    this->text.at(i).length());
            }

            this->update();

            this->text.removeAt(coords.row + 1);
        }
    }
}


void _t::editor::editor::draw_selected_cell(
    const _t::editor::coordinates &coords)
{
    if (coords.col < this->text.at(coords.row).length())
    {
        this->painter.draw_char(
            coords,
            this->text.at(coords.row).at(coords.col),
            this->selection_background);
    }
    else
    {
        this->painter.clear(coords, this->selection_background);
    }
}

void _t::editor::editor::draw_deselected_cell(
    const _t::editor::coordinates &coords)
{
    this->painter.clear(coords);

    if (coords.col < this->text.at(coords.row).length())
    {
        this->painter.draw_char(
            coords,
            this->text.at(coords.row).at(coords.col));
    }
}


void _t::editor::editor::update()
{
    this->area.setPixmap(this->canvas);
}


void _t::editor::editor::cursor_move(
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
        if (target_coords > this->cursor.coords)
        {
            // nothing is selected
            // OR selection started before current cursor coordinates
            if (!this->cursor.selection_mode
                || this->cursor.selection_from < this->cursor.coords)
            {
                // initiate selection
                if (!this->cursor.selection_mode)
                {
                    this->cursor.selection_from = this->cursor.coords;
                    this->cursor.selection_mode = true;
                }

                this->each_cell(this->cursor.coords, target_coords - 1,
                    [&](const coordinates &coords)
                    {
                        this->draw_selected_cell(coords);
                    });
            }

            // selection started behind or at current cursor coordinates
            else
            {
                // target coordinates are before or at original selection start
                if (target_coords <= this->cursor.selection_from)
                {
                    this->each_cell(this->cursor.coords, target_coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });
                }

                // target coordinates are where the selection started
                else if (target_coords == this->cursor.selection_from + 1)
                {
                    this->cursor.selection_mode = false;

                    // deselect whole selection
                    this->each_cell(
                        this->cursor.coords,
                        this->cursor.selection_from,
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
                        this->cursor.coords,
                        this->cursor.selection_from,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });

                    ++this->cursor.selection_from;

                    this->each_cell(
                        this->cursor.selection_from,
                        target_coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_selected_cell(coords);
                        });
                }
            }
        }

        // target coordinates are before current cursor coordinates
        else if (target_coords != this->cursor.coords)
        {
            // nothing is selected
            // OR selection started behind or at current cursor coordinates
            if (!this->cursor.selection_mode
                || this->cursor.selection_from >= this->cursor.coords)
            {
                // initiate selection
                if (!this->cursor.selection_mode)
                {
                    this->cursor.selection_from = this->cursor.coords - 1;
                    this->cursor.selection_mode = true;
                }

                this->each_cell(target_coords, this->cursor.coords - 1,
                    [&](const coordinates &coords)
                    {
                        this->draw_selected_cell(coords);
                    });
            }

            // selection started before current cursor coordinates
            else
            {
                // target coordinates are after original selection start
                if (target_coords > this->cursor.selection_from)
                {
                    this->each_cell(target_coords, this->cursor.coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });
                }

                // target coordinates are where the selection started
                else if (target_coords == this->cursor.selection_from)
                {
                    this->cursor.selection_mode = false;

                    // deselect whole selection
                    this->each_cell(
                        this->cursor.selection_from,
                        this->cursor.coords - 1,
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
                        this->cursor.selection_from,
                        this->cursor.coords - 1,
                        [&](const coordinates &coords)
                        {
                            this->draw_deselected_cell(coords);
                        });

                    --this->cursor.selection_from;

                    this->each_cell(target_coords, this->cursor.selection_from,
                        [&](const coordinates &coords)
                        {
                            this->draw_selected_cell(coords);
                        });
                }
            }
        }
    }

    this->cursor.coords = target_coords;
}


void _t::editor::editor::cursor_show()
{
    if (!this->cursor_visible)
    {
        this->cursor.background = this->canvas.copy(
            this->cursor.col() * this->cell_size.width(),
            this->cursor.row() * this->cell_size.height(),
            this->cell_size.width(),
            this->cell_size.height());

        this->painter.draw_cursor(this->cursor.coords);

        this->update();

        this->cursor_visible = true;
    }
}

void _t::editor::editor::cursor_hide()
{
    if (this->cursor_visible)
    {
        this->painter.draw_pixmap(
            this->cursor.coords,
            this->cursor.background);

        this->update();

        this->cursor_visible = false;
    }
}

void _t::editor::editor::cursor_activate()
{
    this->cursor_show();

    this->cursor_timer.start();
}

void _t::editor::editor::cursor_deactivate()
{
    this->cursor_timer.stop();

    this->cursor_hide();
}


void _t::editor::editor::cursor_timer_tick()
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


void _t::editor::editor::each_cell(
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
