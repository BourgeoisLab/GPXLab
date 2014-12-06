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

#ifndef _DIALOG_SRTM_H_
#define _DIALOG_SRTM_H_

#include <QDialog>
#include "srtm.h"
#include "gpx_wrapper.h"

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_srtm;
}

/**
 * @class Dialog_srtm
 *
 * @brief Dialog subclass to get altitude values from SRTM
 *
 * The dialogue can be used to get altitude values from SRTM.
 *
 * @see GPX_model and SRTM
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 3 Dec 2014
 */
class Dialog_srtm : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param gpxmw GPX_model wrapper
     * @param parent Parent
     */
    explicit Dialog_srtm(const GPX_wrapper *gpxmw, QWidget *parent = 0);
    ~Dialog_srtm();

    /**
     * @brief Get altitude values
     * @return values
     */
    const QVector<double> &getValues() const;

private slots:
    void on_Dialog_srtm_accepted();
    void on_pushButtonFetchData_clicked();
    void on_horizontalSliderKernelSize_valueChanged(int value);
    void on_horizontalSliderNumPasses_valueChanged(int value);

private:
    static const QString dlgName;
    Ui::Dialog_srtm *ui;
    bool modified;
    SRTM *srtm;
    const GPX_wrapper *gpxmw;
    QVector<double> values;

private:
    void setModified(bool modified);
    bool generateSRTMAltitudeValues(int option);
};

/** @} Dialogs */

#endif // _DIALOG_SRTM_H_
