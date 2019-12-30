#ifndef DIALOG_ABOUT_H
#define DIALOG_ABOUT_H

#include <QDialog>

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_about;
}

/**
 * @class Dialog_about
 *
 * @brief About dialogue
 */
class Dialog_about : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param parent Parent
     */
    explicit Dialog_about(QWidget *parent = 0);
    ~Dialog_about();

private:

    Ui::Dialog_about *ui;
};

/** @} Dialogs */

#endif // DIALOG_ABOUT_H
