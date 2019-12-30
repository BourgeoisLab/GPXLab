#ifndef DIALOG_DATETIME_H
#define DIALOG_DATETIME_H

#include <QDialog>

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_DateTime;
}

/**
 * @class Dialog_DateTime
 *
 * @brief Dialogue to select a date and time
 */
class Dialog_DateTime : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param title Window title
     * @param timestamp Initial timestamp
     * @param parent Parent
     */
    explicit Dialog_DateTime(const QString title, const time_t timestamp, QWidget *parent = nullptr);
    ~Dialog_DateTime();

    /**
     * @brief Gets the date and time
     * @return Date and time
     */
    QDateTime getDateTime() const;

    /**
     * @brief Gets the date and time as timestamp value
     * @return Timestamp
     */
    time_t getTimestamp() const;

private:

    Ui::Dialog_DateTime *ui;
};

/** @} Dialogs */

#endif // DIALOG_DATETIME_H
