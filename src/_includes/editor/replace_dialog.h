#ifndef _T_EDITOR_REPLACE_DIALOG_H
#define _T_EDITOR_REPLACE_DIALOG_H

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


class _t::editor::replace_dialog : public QWidget
{
    Q_OBJECT


    /**
     * @var _t::editor::editor * parent_editor
     */
    _t::editor::editor *parent_editor = 0;


    /**
     * @var const QString default_title
     */
    const QString default_title = "Replace";


    /**
     * @var QLabel find_lb
     */
    QLabel find_lb;

    /**
     * @var QLineEdit find_tb
     */
    QLineEdit find_tb;

    /**
     * @var QLabel replace_lb
     */
    QLabel replace_lb;

    /**
     * @var QLineEdit replace_tb
     */
    QLineEdit replace_tb;

    /**
     * @var QCheckBox case_sensitivity_chb
     */
    QCheckBox case_sensitivity_chb;

    /**
     * @var QCheckBox regex_chb
     */
    QCheckBox regex_chb;

    /**
     * @var QPushButton submit_btn
     */
    QPushButton submit_btn;


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
    explicit replace_dialog(_t::editor::editor *parent);

private slots:
    /**
     * Handles submit button press.
     */
    void submit_btn_pressed();
};

#endif // _T_EDITOR_REPLACE_DIALOG_H
