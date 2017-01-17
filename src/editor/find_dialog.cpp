#include "editor/find_dialog.h"

#include <QGridLayout>


_t::editor::find_dialog::find_dialog(_t::editor::editor *parent)
    : parent_editor(parent)
{
    this->setWindowTitle(this->default_title);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(&this->find_lb, 0, 0);
    layout->addWidget(&this->find_tb, 0, 1);
    layout->addWidget(&this->submit_btn, 0, 2);
    layout->addWidget(&this->regex_chb, 1, 1);

    this->setLayout(layout);

    this->find_lb.setText("Find text:");
    this->find_tb.setMinimumWidth(200);
    this->submit_btn.setText("Find");
    this->regex_chb.setText("Regular expressions");

    this->setFixedSize(this->sizeHint());

    connect(
        &this->submit_btn,
        SIGNAL(clicked()),
        this,
        SLOT(submit_btn_pressed()));
}


void _t::editor::find_dialog::got_to_eof()
{
    this->setWindowTitle(
        this->default_title + " - Search passed the end of file.");
}


void _t::editor::find_dialog::showEvent(QShowEvent *)
{
    this->setWindowTitle(this->default_title);

    this->find_tb.selectAll();
}

void _t::editor::find_dialog::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        this->close();
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        this->submit_btn.click();
    }
}

void _t::editor::find_dialog::submit_btn_pressed()
{
    this->setWindowTitle(this->default_title);

    this->parent_editor->find_next(
        this->find_tb.text(),
        this->regex_chb.isChecked());
}
