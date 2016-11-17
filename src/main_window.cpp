#include "main_window.h"

#include <QVBoxLayout>
#include <QSizePolicy>

#include <QKeySequence>

#include <QFileDialog>
#include <QFile>
#include <QTextStream>


_t::main_window::main_window(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(280, 160);
    this->resize(720, 360);


    this->menubar.setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);

    this->menubar.addMenu(&this->file_menu);

    this->file_menu.setTitle("File");

    this->file_menu.addAction(
        "New",
        this, SLOT(menu_file_new()),
        QKeySequence::New);

    this->file_menu.addAction(
        "Open",
        this, SLOT(menu_file_open()),
        QKeySequence::Open);

    this->file_menu.addAction(
        "Save",
        this, SLOT(menu_file_save()),
        QKeySequence::Save);

    this->file_menu.addAction(
        "Save As",
        this, SLOT(menu_file_save_as()),
        QKeySequence::SaveAs);

    this->file_menu.addSeparator();

    this->file_menu.addAction(
        "Quit",
        this, SLOT(menu_file_quit()),
        QKeySequence::Quit);


    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(&this->menubar);
    layout->addWidget(&this->editor);

    this->setLayout(layout);
}


void _t::main_window::save(const QString &location)
{
    QFile file(location);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream filestream(&file);
    filestream.setCodec("UTF-8");
    filestream << this->editor.get_text();
    filestream.flush();

    file.close();

    this->file_path = location;
}


void _t::main_window::menu_file_new()
{

}

void _t::main_window::menu_file_open()
{

}

void _t::main_window::menu_file_save()
{
    if (this->file_path != 0)
    {
        this->save(this->file_path);
    }
    else
    {
        this->menu_file_save_as();
    }
}

void _t::main_window::menu_file_save_as()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save");

    this->save(filename);
}

void _t::main_window::menu_file_quit()
{

}
