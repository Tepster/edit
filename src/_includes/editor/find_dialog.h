#ifndef _T_EDITOR_FIND_DIALOG_H
#define _T_EDITOR_FIND_DIALOG_H

#include "../editor.h"

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>

#include <QKeyEvent>
#include <QShowEvent>

#include <QString>

#include "editor.h"


class _t::editor::find_dialog : public QWidget
{
    Q_OBJECT


    /**
     * @var _t::editor::editor * parent_editor
     */
    _t::editor::editor *parent_editor = 0;


    /**
     * @var const QString default_title
     */
    const QString default_title = "Find";


    /**
     * @var QLabel find_lb
     */
    QLabel find_lb;

    /**
     * @var QLineEdit find_tb
     */
    QLineEdit find_tb;

    /**
     * @var QPushButton submit_btn
     */
    QPushButton submit_btn;

    /**
     * @var QCheckBox regex_chb
     */
    QCheckBox regex_chb;


    /**
     * Handles show event.
     *
     * @param QShowEvent * event
     */
    void showEvent(QShowEvent *event);

    /**
     * Handles key-press event.
     *
     * @param QKeyEvent * event
     */
    void keyPressEvent(QKeyEvent *event);

public:
    /**
     * Main constructor.
     *
     * @param _t::editor::editor * parent
     */
    explicit find_dialog(_t::editor::editor *parent);


    /**
     * Informs user that the finding got to the end of file.
     */
    void got_to_eof();

private slots:
    /**
     * Handles submit button press.
     */
    void submit_btn_pressed();
};

#endif // _T_EDITOR_FIND_DIALOG_H
