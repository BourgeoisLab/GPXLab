/****************************************************************************
 *   Copyright (c) 2014 Frederic Bourgeois <bourgeoislab@gmail.com>         *
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

#ifndef _DIALOG_ABOUT_H_
#define _DIALOG_ABOUT_H_

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
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 6 Dec 2014
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

#endif // _DIALOG_ABOUT_H_
