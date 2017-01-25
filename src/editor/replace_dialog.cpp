#include "editor/replace_dialog.h"

#include <QGridLayout>


_t::editor::replace_dialog::replace_dialog(_t::editor::editor *parent)
    : parent_editor(parent)
{
    this->setWindowTitle(this->default_title);
    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    QGridLayout *layout = new QGridLayout;

    layout->addWidget(&this->find_lb, 0, 0);
    layout->addWidget(&this->find_tb, 0, 1);
    layout->addWidget(&this->replace_lb, 1, 0);
    layout->addWidget(&this->replace_tb, 1, 1);
    layout->addWidget(&this->case_sensitivity_chb, 2, 1);
    layout->addWidget(&this->regex_chb, 3, 1);
    layout->addWidget(&this->submit_btn, 4, 1);

    this->setLayout(layout);

    this->find_lb.setText("Find:");
    this->find_tb.setMinimumWidth(200);
    this->replace_lb.setText("Replace:");
    this->replace_tb.setMinimumWidth(200);
    this->case_sensitivity_chb.setText("Case sensitive");
    this->regex_chb.setText("Regular expressions");
    this->submit_btn.setText("Find && Replace");

    this->setFixedSize(this->sizeHint());

    connect(
        &this->submit_btn,
        SIGNAL(clicked()),
        this,
        SLOT(submit_btn_pressed()));
}


void _t::editor::replace_dialog::showEvent(QShowEvent *)
{
    this->setWindowTitle(this->default_title);

    this->find_tb.selectAll();
}

void _t::editor::replace_dialog::keyPressEvent(QKeyEvent *event)
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

void _t::editor::replace_dialog::submit_btn_pressed()
{
    this->setWindowTitle(this->default_title);

    this->parent_editor->replace(
        this->find_tb.text(),
        this->replace_tb.text(),
        this->case_sensitivity_chb.isChecked(),
        this->regex_chb.isChecked());
}
