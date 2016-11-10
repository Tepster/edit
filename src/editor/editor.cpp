#include "editor/editor.h"

#include <QApplication>
#include <QClipboard>

#include <QGridLayout>

#include <QChar>

#include <QColor>
#include <QFont>


_t::editor::editor::editor()
{
    this->vscrollbar = new _t::editor::vscrollbar(this->shift.ry());


    this->cursor_timer.setParent(this);
    connect(
        &this->cursor_timer,
        SIGNAL(timeout()),
        this,
        SLOT(cursor_timer_ticked()));
    this->cursor_timer.setInterval(500);

    connect(
        &this->area,
        SIGNAL(mouse_press_event(QMouseEvent *)),
        this,
        SLOT(mouse_pressed(QMouseEvent *)));

    this->area.setMouseTracking(true);
    connect(
        &this->area,
        SIGNAL(mouse_move_event(QMouseEvent *)),
        this,
        SLOT(mouse_moved(QMouseEvent *)));

    connect(
        this->vscrollbar,
        SIGNAL(scroll_event()),
        this,
        SLOT(vscrolled()));


    this->cell_size = QSize(10, 20);
    this->selection_background = QColor(80, 80, 90);


    this->vscrollbar->area_size_changed(this->cell_size.height());

    this->area.setCursor(Qt::IBeamCursor);

    this->text << "";

    this->cursor.coords.set_text(&this->text);

    this->canvas = QPixmap(this->size());

    this->painter.init(
        QColor(60, 60, 70),
        QFont("Consolas", 12),
        QColor(250, 250, 250),
        &this->cell_size,
        &this->shift,
        &this->canvas);

    QGridLayout *layout = new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(&this->area, 0, 0);
    layout->addWidget(this->vscrollbar, 0, 1);

    this->setLayout(layout);

    this->setFocusPolicy(Qt::StrongFocus);
}


void _t::editor::editor::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
    {
        switch (event->key())
        {
        case Qt::Key_A:
            this->cursor_deactivate();

            this->cursor.selection_mode = false;
            this->cursor.coords = coordinates(0, 0);

            this->cursor_move(
                coordinates(
                    this->text.count() - 1,
                    this->text.at(this->text.count() - 1).length()),
                true);

            this->cursor_activate();
            break;

        case Qt::Key_C:
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
            break;

        case Qt::Key_V:
            this->write(QApplication::clipboard()->text());
            break;
        }
    }

    // home, end, left, up, right, down, pageup, pagedown
    else if (event->key() >= 0x01000010 && event->key() <= 0x01000017)
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

        if (this->cursor.selection_mode)
        {
            this->cursor.selection_mode = false;

            coordinates from, to;

            // selection goes in forward direction
            if (this->cursor.coords > this->cursor.selection_from)
            {
                from = this->cursor.selection_from;
                to = this->cursor.coords - 1;

                this->cursor.coords = from;
            }
            else
            {
                from = this->cursor.coords;
                to = this->cursor.selection_from;
            }

            this->delete_chars(from, to);
        }
        else if (this->cursor.coords)
        {
            this->delete_char(--this->cursor.coords);
        }

        this->redraw();
        this->vscrollbar->area_size_changed(
            this->text.count() * this->cell_size.height());
        this->scroll_to_cursor();

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Delete)
    {
        this->cursor_deactivate();

        if (this->cursor.selection_mode)
        {
            this->cursor.selection_mode = false;

            coordinates from, to;

            // selection goes in forward direction
            if (this->cursor.coords > this->cursor.selection_from)
            {
                from = this->cursor.selection_from;
                to = this->cursor.coords - 1;

                this->cursor.coords = from;
            }
            else
            {
                from = this->cursor.coords;
                to = this->cursor.selection_from;
            }

            this->delete_chars(from, to);
        }
        else
        {
            this->delete_char(this->cursor.coords);
        }

        this->redraw();
        this->vscrollbar->area_size_changed(
            this->text.count() * this->cell_size.height());
        this->scroll_to_cursor();

        this->cursor_activate();
    }

    else if (event->key() == Qt::Key_Tab)
    {
        this->write("    ");
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

void _t::editor::editor::wheelEvent(QWheelEvent *event)
{
    // 4 rows per usual wheel step
    qreal target_shift_px = qreal(this->shift.y()) - qreal(event->delta()) / 30
        * this->cell_size.height();

    this->vscrollbar->scroll(target_shift_px);
}

void _t::editor::editor::mouse_pressed(QMouseEvent *event)
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

    qint32 row = (event->localPos().y() + this->shift.y())
        / this->cell_size.height();

    qint32 col = (event->localPos().x() + this->shift.x())
        / this->cell_size.width();

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

void _t::editor::editor::mouse_moved(QMouseEvent *event)
{
    if (event->buttons() != Qt::LeftButton)
    {
        return;
    }

    this->cursor_deactivate();

    qint32 row = (event->localPos().y() + this->shift.y())
        / this->cell_size.height();

    qint32 col = (event->localPos().x() + this->shift.x())
        / this->cell_size.width();

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

void _t::editor::editor::resizeEvent(QResizeEvent *)
{
    this->cursor_deactivate();

    this->painter.end();

    this->canvas = QPixmap(
        this->width() - this->vscrollbar->width(),
        this->height());

    this->painter.init_canvas(&this->canvas);
    this->area.resize(this->canvas.size());
    this->redraw();

    this->cursor_activate();
}

void _t::editor::editor::vscrolled()
{
    this->redraw();
}


void _t::editor::editor::write(const QString &text)
{
    this->cursor_deactivate();

    if (this->cursor.selection_mode)
    {
        this->cursor.selection_mode = false;

        coordinates from, to;

        // selection goes in forward direction
        if (this->cursor.coords > this->cursor.selection_from)
        {
            from = this->cursor.selection_from;
            to = this->cursor.coords - 1;

            this->cursor.coords = from;
        }
        else
        {
            from = this->cursor.coords;
            to = this->cursor.selection_from;
        }

        this->delete_chars(from, to);
    }

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

    this->vscrollbar->area_size_changed(
        this->text.count() * this->cell_size.height());

    this->scroll_to_cursor();

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

void _t::editor::editor::redraw()
{
    this->cursor_deactivate();

    this->canvas.fill(QColor(60, 60, 70));

    if (this->text.count() > 1 || this->text.at(0).length() > 0)
    {
        coordinates last_cell;
        if (this->text.at(this->text.count() - 1).length() > 0)
        {
            last_cell.set(
                this->text.count() - 1,
                this->text.at(this->text.count() - 1).length() - 1);
        }
        else
        {
            last_cell.set(
                this->text.count() - 2,
                this->text.at(this->text.count() - 2).length());
        }

        this->each_cell(
            coordinates(0, 0),
            last_cell,
            [&](const coordinates &coords)
            {
                if (this->text.at(coords.row).length() > coords.col)
                {
                    this->painter.draw_char(
                        coords,
                        this->text.at(coords.row).at(coords.col));
                }
            });


        if (this->cursor.selection_mode)
        {
            coordinates selection_from, selection_to;

            if (this->cursor.coords > this->cursor.selection_from)
            {
                selection_from = this->cursor.selection_from;
                selection_to = this->cursor.coords - 1;
            }
            else
            {
                selection_from = this->cursor.coords;
                selection_to = this->cursor.selection_from;
            }

            this->each_cell(
                selection_from,
                selection_to,
                [&](const coordinates &coords)
                {
                    this->draw_selected_cell(coords);
                });
        }

        this->update();
    }

    this->cursor_activate();
}


void _t::editor::editor::delete_char(const coordinates &coords)
{
    // coords not at the end of the line
    if (coords.col < this->text.at(coords.row).length())
    {
        this->painter.clear(coords);

        this->painter.shift_left(
            coordinates(coords.row, coords.col + 1),
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

void _t::editor::editor::delete_chars(
    const coordinates &from,
    const coordinates &to)
{
    // clear all cells in the range
    this->each_cell(from, to,
        [&](const coordinates &coords)
        {
            this->painter.clear(coords);
        });

    coordinates after = to + 1;

    // move the rest of the last line to the end of the first one
    this->painter.move(
        after,
        this->text.at(after.row).length() - after.col,
        from);

    if (from.row != after.row)
    {
        // shift up all following lines
        for (qint32 i = after.row + 1; i < this->text.count(); ++i)
        {
            this->painter.move(
                coordinates(i, 0),
                this->text.at(i).length(),
                coordinates(i - (after.row - from.row), 0));
        }

        // move the rest of the last line to the end of the first one
        this->text[from.row].chop(this->text.at(from.row).length() - from.col);
        this->text[from.row] += this->text.at(after.row)
            .right(this->text.at(after.row).length() - after.col);

        // remove all lines of the range
        for (qint32 i = from.row; i < after.row; ++i)
        {
            this->text.removeAt(from.row + 1);
        }
    }
    else
    {
        // remove the range from the line
        this->text[from.row] = this->text.at(from.row).left(from.col)
            + this->text.at(from.row)
                .right(this->text.at(from.row).length() - after.col);
    }

    this->update();
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

    this->scroll_to_cursor();
}

void _t::editor::editor::scroll_to_cursor()
{
    // cursor above the viewport
    if (this->cursor.row() * this->cell_size.height() < this->shift.y())
    {
        this->vscrollbar->scroll(this->cursor.row() * this->cell_size.height());
    }

    // cursor below the viewport
    else if ((this->cursor.row() + 1) * this->cell_size.height()
        > this->area.height() + this->shift.y())
    {
        this->vscrollbar->scroll((this->cursor.row() + 1)
                * this->cell_size.height() - this->area.height());
    }
}


void _t::editor::editor::cursor_show()
{
    if (!this->cursor_visible)
    {
        this->cursor.background = this->canvas.copy(
            this->cursor.col() * this->cell_size.width() - this->shift.x(),
            this->cursor.row() * this->cell_size.height() - this->shift.y(),
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


void _t::editor::editor::cursor_timer_ticked()
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
