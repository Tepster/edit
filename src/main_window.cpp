#include "main_window.h"

#include <QApplication>

#include <QVBoxLayout>
#include <QSizePolicy>

#include <QKeySequence>

#include <QFileDialog>
#include <QFile>
#include <QIODevice>
#include <QTextStream>

#include <QMessageBox>


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

bool _t::main_window::save_if_desired()
{
    QMessageBox msgbox;
    msgbox.setText("The document has been modified.");
    msgbox.setInformativeText("Do you want to save your changes?");
    msgbox.setStandardButtons(
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgbox.setDefaultButton(QMessageBox::Save);

    qint32 answer = msgbox.exec();

    if (answer == QMessageBox::Cancel)
    {
        return false;
    }

    if (answer == QMessageBox::Save)
    {
        this->menu_file_save();
    }

    return true;
}

bool _t::main_window::document_modified()
{
    // the document isn't saved as a file yet
    if (this->file_path == 0)
    {
        if (this->editor.get_text().length())
        {
            return true;
        }

        return false;
    }

    // compare the saved file vs the current document
    QFile file(this->file_path);
    file.open(QIODevice::ReadOnly);

    QString file_text = file.readAll();
    QString document_text = this->editor.get_text();

    file.close();

    if (file_text == document_text)
    {
        return false;
    }

    return true;
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
    if (this->document_modified())
    {
        bool should_proceed = this->save_if_desired();

        if (!should_proceed)
        {
            return;
        }
    }

    QApplication::quit();
}
