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

#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <QDialog>
#include "settings.h"

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_settings;
}

/**
 * @class Dialog_settings
 *
 * @brief Settings dialogue
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 4 Jan 2015
 */
class Dialog_settings : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param settings Settings
     * @param parent Parent
     */
    explicit Dialog_settings(Settings *settings, QWidget *parent = 0);
    ~Dialog_settings();

private slots:

    void on_Dialog_settings_accepted();
    void on_checkBoxMapPersistentCache_toggled(bool checked);
    void on_pushButtonMapCacheLocationSelect_clicked();
    void on_pushButtonMapClearCache_clicked();
    void on_pushButtonMapCacheLocationDefault_clicked();
    void on_pushButtonTilesURLDefault_clicked();

private:

    Ui::Dialog_settings *ui;
    Settings *settings;
};

/** @} Dialogs */

#endif // DIALOG_SETTINGS_H
