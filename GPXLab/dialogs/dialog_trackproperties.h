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

#ifndef _DIALOG_TRACKPROPERTIES_H_
#define _DIALOG_TRACKPROPERTIES_H_

#include <QDialog>
#include "gpx_model.h"

namespace Ui {
class Dialog_trackProperties;
}

/**
 * @class Dialog_trackProperties
 *
 * @brief Dialog subclass to edit track properties (metadata)
 *
 * The dialog can be used to see and modify track properties (metadata).
 * Also it shows statistic information about the track.
 *
 * @see GPX_model
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 25 Oct 2014
 */
class Dialog_trackProperties : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialog
     * @param metadata Track properties (metadata) to modify
     * @param stats Statistic information about the track
     * @param parent Parent
     */
    explicit Dialog_trackProperties(GPX_trkMetadataType &metadata, const GPX_statsType &stats, QWidget *parent = 0);
    ~Dialog_trackProperties();

private slots:
    void on_Dialog_trackProperties_accepted();
    void on_pushButtonTableAdd_clicked();
    void on_pushButtonTableDelete_clicked();
    void on_pushButtonUndoName_clicked();
    void on_pushButtonUndoCmt_clicked();
    void on_pushButtonUndoDesc_clicked();
    void on_pushButtonUndoSrc_clicked();
    void on_pushButtonUndoType_clicked();
    void on_lineEditName_editingFinished();
    void on_lineEditCmt_editingFinished();
    void on_lineEditDesc_editingFinished();
    void on_lineEditSrc_editingFinished();
    void on_lineEditType_editingFinished();

private:
    static const QString dlgName;
    Ui::Dialog_trackProperties *ui;
    bool modified;
    GPX_trkMetadataType &metadata;

private:
    void setModified(bool modified);
};

#endif // _DIALOG_TRACKPROPERTIES_H_
