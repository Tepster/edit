#include <QApplication>

#include "main_window.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    _t::main_window window;
    window.show();

    return app.exec();
}
