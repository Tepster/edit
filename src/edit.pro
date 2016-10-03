QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = edit
TEMPLATE = app


SOURCES += main.cpp \
    main_window.cpp \
    editor.cpp \
    editor_coordinates.cpp \
    label.cpp

HEADERS  += \
    includes/main_window.h \
    includes/editor.h \
    includes/controls.h

INCLUDEPATH += includes
