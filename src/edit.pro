QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = edit
TEMPLATE = app


SOURCES += main.cpp \
    main_window.cpp \
    controls/label.cpp \
    editor/cursor.cpp \
    editor/coordinates.cpp \
    editor/drawing_manager.cpp \
    editor/editor.cpp

HEADERS  += \
    _includes/main_window.h \
    _includes/controls.h \
    _includes/editor/editor.h \
    _includes/editor/coordinates.h \
    _includes/editor/cursor.h \
    _includes/editor/drawing_manager.h

INCLUDEPATH += _includes
