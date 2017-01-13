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
    editor/editor.cpp \
    editor/vscrollbar.cpp \
    editor/scrollbar.cpp

HEADERS  += \
    _includes/main_window.h \
    _includes/controls/label.h \
    _includes/editor/editor.h \
    _includes/editor/coordinates.h \
    _includes/editor/cursor.h \
    _includes/editor/drawing_manager.h \
    _includes/editor/scrollbar.h \
    _includes/editor/vscrollbar.h \
    _includes/editor/sh_rules.h \
    _includes/utils.h

INCLUDEPATH += _includes
