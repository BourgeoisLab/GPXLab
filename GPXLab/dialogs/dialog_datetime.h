/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

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
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 10 Jan 2015
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
    explicit Dialog_DateTime(const QString title, const time_t timestamp, QWidget *parent = 0);
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
