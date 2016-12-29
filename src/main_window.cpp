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
    this->menubar.addMenu(&this->sh_menu);

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


    this->sh_menu.setTitle("Syntax Highlighting");

    this->sh_menu.addAction("C++", this, SLOT(menu_sh_cpp()));
    this->sh_menu.addAction("C#", this, SLOT(menu_sh_csharp()));
    this->sh_menu.addAction("PHP", this, SLOT(menu_sh_php()));
    this->sh_menu.addAction("JavaScript", this, SLOT(menu_sh_javascript()));
    this->sh_menu.addAction("HTML", this, SLOT(menu_sh_html()));
    this->sh_menu.addAction("CSS", this, SLOT(menu_sh_css()));
    this->sh_menu.addSeparator();
    this->sh_menu.addAction("None", this, SLOT(menu_sh_none()));


    this->editor = new _t::editor::editor;

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(&this->menubar);
    layout->addWidget(this->editor);

    this->setLayout(layout);
}


void _t::main_window::closeEvent(QCloseEvent *event)
{
    if (!this->check_save())
    {
        event->ignore();
    }
}


void _t::main_window::save(const QString &location)
{
    QFile file(location);

    bool file_good = file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file_good)
    {
        QMessageBox msgbox;
        msgbox.setText("Oops. There has been a problem saving the document.");
        msgbox.exec();

        this->file_path.clear();

        return;
    }

    QTextStream filestream(&file);
    filestream.setCodec("UTF-8");
    filestream << this->editor->get_text();
    filestream.flush();

    file.close();

    this->file_path = location;
}

bool _t::main_window::check_save()
{
    if (this->document_modified())
    {
        QMessageBox msgbox;
        msgbox.setText("The document has been modified.");
        msgbox.setInformativeText("Do you want to save your changes?");
        msgbox.setStandardButtons(
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgbox.setDefaultButton(QMessageBox::Save);

        qint32 answer = msgbox.exec();

        // user wants to save the document
        if (answer == QMessageBox::Save)
        {
            if (this->file_path.length())
            {
                this->save(this->file_path);

                return true;
            }

            QString location = this->ask_save_location();

            if (location.length())
            {
                this->save(location);

                return true;
            }

            return false;
        }

        if (answer == QMessageBox::Discard)
        {
            return true;
        }

        return false;
    }

    return true;
}

QString _t::main_window::ask_save_location()
{
    return QFileDialog::getSaveFileName(this, "Save");
}

bool _t::main_window::document_modified() const
{
    // document isn't saved as a file yet
    if (this->file_path.length() == 0)
    {
        // document isn't empty
        if (this->editor->get_text().length())
        {
            return true;
        }

        return false;
    }

    // compare the saved file vs the current document
    QFile file(this->file_path);

    bool file_good = file.open(QIODevice::ReadOnly);
    if (!file_good)
    {
        return true;
    }

    QString file_text = file.readAll();
    QString document_text = this->editor->get_text();

    file.close();

    if (file_text == document_text)
    {
        return false;
    }

    return true;
}


void _t::main_window::menu_file_new()
{
    if (!this->check_save())
    {
        return;
    }

    this->file_path.clear();


    delete this->editor;

    this->editor = new _t::editor::editor;

    // attach the new editor to the layout
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(this->layout());
    layout->addWidget(this->editor);
    this->setLayout(layout);
}

void _t::main_window::menu_file_open()
{
    if (!this->check_save())
    {
        return;
    }

    // ask user for the file path
    QString path = QFileDialog::getOpenFileName(this, "Open File");

    // user canceled
    if (path.length() == 0)
    {
        return;
    }

    QFile file(path);

    bool file_good = file.open(QIODevice::ReadOnly);
    if (!file_good)
    {
        QMessageBox msgbox;
        msgbox.setText(
            "Oops. There has been a problem opening the file.");
        msgbox.exec();

        return;
    }

    this->file_path = path;


    delete this->editor;

    // create a new editor and load the file into it
    this->editor = new _t::editor::editor;
    this->editor->write(file.readAll());
    this->editor->move_cursor_to_beginning();

    // attach the new editor to the layout
    QVBoxLayout *layout = static_cast<QVBoxLayout *>(this->layout());
    layout->addWidget(this->editor);
    this->setLayout(layout);
}

void _t::main_window::menu_file_save()
{
    if (this->file_path.length())
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
    QString filename = this->ask_save_location();

    if (filename.length())
    {
        this->save(filename);
    }
}

void _t::main_window::menu_file_quit()
{
    if (this->check_save())
    {
        QApplication::quit();
    }
}


void _t::main_window::menu_sh_cpp()
{
    this->editor->set_sh_rules(&this->sh_rules.cpp);
}

void _t::main_window::menu_sh_csharp()
{
    this->editor->set_sh_rules(&this->sh_rules.csharp);
}

void _t::main_window::menu_sh_php()
{
    this->editor->set_sh_rules(&this->sh_rules.php);
}

void _t::main_window::menu_sh_javascript()
{
    this->editor->set_sh_rules(&this->sh_rules.javascript);
}

void _t::main_window::menu_sh_html()
{
    this->editor->set_sh_rules(&this->sh_rules.html);
}

void _t::main_window::menu_sh_css()
{
    this->editor->set_sh_rules(&this->sh_rules.css);
}

void _t::main_window::menu_sh_none()
{
    this->editor->set_sh_rules(0);
}
