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

#include "gpxlab.h"
#include "ui_gpxlab.h"
#include "dialog_about.h"
#include "dialog_modelproperties.h"
#include "dialog_trackproperties.h"
#include "dialog_srtm.h"
#include <QMessageBox>

//////////////////////////////////////////////////////////////////////////////

void GPXLab::on_actionAbout_triggered()
{
    Dialog_about *dlg = new Dialog_about(this);
    dlg->exec();
    delete dlg;
}

void GPXLab::on_actionEdit_File_Properties_triggered()
{
    if (gpxmw)
    {
        GPX_metadataType metadata = *gpxmw->getModelMetadata(); // copy metadata
        Dialog_modelProperties *dlg = new Dialog_modelProperties(metadata, *gpxmw->getModelStats(), this);
        if(dlg->exec() == QDialog::Accepted)
        {
            // set new metadata
            gpxmw->setModelMetadata(metadata);

            // update file properties
            if (check_modified())
            {
                updating = true;
                updateFile();
                updating = false;
            }
        }
        delete dlg;
    }
}

void GPXLab::on_pushButtonFileEdit_clicked()
{
    on_actionEdit_File_Properties_triggered();
}

void GPXLab::on_actionEdit_Track_Properties_triggered()
{
    if (gpxmw)
    {
        const GPX_statsType *stats = gpxmw->getTrackStats(gpxmw->getSelectedTrackNumber());
        if (stats)
        {
            GPX_trkMetadataType metadata = *gpxmw->getTrackMetadata(gpxmw->getSelectedTrackNumber()); // copy metadata
            Dialog_trackProperties *dlg = new Dialog_trackProperties(metadata, *stats, this);
            if(dlg->exec() == QDialog::Accepted)
            {
                // set new metadata
                gpxmw->setTrackMetadata(gpxmw->getSelectedTrackNumber(), metadata);

                // update track properties
                if (check_modified())
                {
                    ui->treeTracks->setCurrentTrackText(gpxmw->getItemName(gpxmw->getSelectedTrackNumber()));
                    updating = true;
                    updateTrack(ORIGIN_UNSPECIFIC);
                    updating = false;
                }
            }
            delete dlg;
        }
    }
}

void GPXLab::on_pushButtonTrackEdit_clicked()
{
    on_actionEdit_Track_Properties_triggered();
}

void GPXLab::on_actionGetAltitudeFromSRTM_triggered()
{
    if (gpxmw)
    {
        QList<QTreeWidgetItem*> items = ui->treeTracks->selectedItems();
        if (items.count() > 0)
        {
            // set selection to parent track if a track segment is currently selected
            if(items[0]->parent())
                ui->treeTracks->setCurrentItem(items[0]->parent());

            // open SRTM dialog
            Dialog_srtm *dlg = new Dialog_srtm(gpxmw, this);
            if(dlg->exec() == QDialog::Accepted)
            {
                // set new altitude values
                if (gpxmw->setAltitudeValues(dlg->getValues(), gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber()) == GPX_model::GPXM_OK)
                {
                    // update track properties
                    if (check_modified())
                    {
                        updating = true;
                        updateFile();
                        updateTrack(ORIGIN_UNSPECIFIC);
                        updating = false;
                    }
                }
            }
            delete dlg;
        }
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::on_actionOpen_triggered()
{
    if (check_need_save() != QMessageBox::Cancel)
        openFile();
}

void GPXLab::openRecentFile()
{
    if (check_need_save() != QMessageBox::Cancel)
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            openFile(action->data().toString());
    }
}

void GPXLab::on_actionAppend_File_triggered()
{
    QStringList fileNames;
    appendFiles(fileNames);
}

void GPXLab::on_pushButtonTrackAppend_clicked()
{
    on_actionAppend_File_triggered();
}

void GPXLab::on_actionSave_File_triggered()
{
    if (gpxmw)
        saveFile(gpxmw->getFileName());
    else
        saveFile();
}

void GPXLab::on_actionSave_File_As_triggered()
{
    saveFile();
}

void GPXLab::on_actionClose_File_triggered()
{
    if (check_need_save() != QMessageBox::Cancel)
        closeFile();
}

void GPXLab::on_actionExit_triggered()
{
    close();
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::on_pushButtonTrackRemove_clicked()
{
    if (gpxmw)
    {
        if (gpxmw->removeTrack(gpxmw->getSelectedTrackNumber()) == GPX_model::GPXM_OK)
        {
            if (check_modified())
            {
                gpxmw->setSelectedTrack(gpxmw->getSelectedTrackNumber());
                gpxmw->setSelectedPointByNumber(-1);
                updating = true;
                updateFile();
                build();
                updateTrack(ORIGIN_UNSPECIFIC);
                updating = false;
            }
        }
    }
}

void GPXLab::on_pushButtonTrackMoveDown_clicked()
{
    if (gpxmw)
    {
        if (gpxmw->moveTrackDown(gpxmw->getSelectedTrackNumber()) == GPX_model::GPXM_OK)
        {
            if (check_modified())
            {
                gpxmw->setSelectedTrack(gpxmw->getSelectedTrackNumber() + 1);
                updating = true;
                build(true);
                updating = false;
            }
        }
    }
}

void GPXLab::on_pushButtonTrackMoveUp_clicked()
{
    if (gpxmw)
    {
        if (gpxmw->moveTrackUp(gpxmw->getSelectedTrackNumber()) == GPX_model::GPXM_OK)
        {
            if (check_modified())
            {
                gpxmw->setSelectedTrack(gpxmw->getSelectedTrackNumber() - 1);
                updating = true;
                build(true);
                updating = false;
            }
        }
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::on_actionFit_View_triggered()
{
    if (gpxmw)
    {
        const GPX_metadataType *metadata = gpxmw->getModelMetadata();
        ui->mapWidget->setViewAndZoomIn(metadata->bounds);
    }
}

void GPXLab::on_actionFit_Track_triggered()
{
    if (gpxmw)
    {
        const GPX_statsType *stats = gpxmw->getTrackStats(gpxmw->getSelectedTrackNumber());
        if (stats)
            ui->mapWidget->setViewAndZoomIn(stats->bounds);
    }
}

void GPXLab::on_actionFit_Point_triggered()
{
    if (gpxmw)
    {
        ui->mapWidget->setViewAndZoomInSelectedPoint();
    }
}

void GPXLab::on_actionFollow_Item_toggled(bool follow)
{
    if (gpxmw)
    {
        ui->mapWidget->setFollowSelection(follow);
    }
}

void GPXLab::on_actionShow_Only_Selected_Track_toggled(bool checked)
{
    if (gpxmw)
    {
        ui->mapWidget->setShowOnlySelectedTrack(checked);
        ui->mapWidget->selectTrack(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::getOffsets(int &left, int &right, int &top, int &bottom)
{
    QDockWidget *docks[4] = {ui->dockWidgetFile,
                                   ui->dockWidgetTracks,
                                   ui->dockWidgetDiagrams,
                                   ui->dockWidgetPoints};

    left = 0;
    right = 0;
    top = 0;
    bottom = 0;

    // dock widgets
    for (int i = 0; i < 4; ++i)
    {
        if (!docks[i]->isFloating() && docks[i]->isVisible())
        {
            switch (dockWidgetArea(docks[i]))
            {
                case Qt::LeftDockWidgetArea:
                    if (docks[i]->width() + 4 > left)
                        left = docks[i]->width() + 4;
                    break;
                case Qt::RightDockWidgetArea:
                    if (docks[i]->width() + 4 > right)
                        right = docks[i]->width() + 4;
                    break;
                case Qt::TopDockWidgetArea:
                    if (docks[i]->height() + 4 > top)
                        top = docks[i]->height() + 4;
                    break;
                case Qt::BottomDockWidgetArea:
                    if (docks[i]->height() + 4 > bottom)
                        bottom = docks[i]->height() + 4;
                    break;
                default:
                    break;
            }
        }
    }

    // menubar
    if (ui->menuBar->isVisible())
    {
        top += ui->menuBar->height();
    }

    // toolbar
    if (!ui->mainToolBar->isFloating() && ui->mainToolBar->isVisible())
    {
        switch (toolBarArea(ui->mainToolBar))
        {
            case Qt::LeftToolBarArea:
                left += ui->mainToolBar->width();
                break;
            case Qt::RightToolBarArea:
                right += ui->mainToolBar->width();
                break;
            case Qt::TopToolBarArea:
                top += ui->mainToolBar->height();
                break;
            case Qt::BottomToolBarArea:
                bottom += ui->mainToolBar->height();
                break;
            default:
                break;
        }
    }

    // statusbar
    if (ui->statusBar->isVisible())
    {
        bottom += ui->statusBar->height();
    }
}

void GPXLab::update_size()
{
    if (closing)
        return;

    QSize mapSize;
    int left, right, top, bottom;

    // get offsets due to docks, menubar, toolbar and statusbar
    getOffsets(left, right, top, bottom);

    // resize map widget
    mapSize.setWidth(width() - left - right - 1);
    mapSize.setHeight(height() - top - bottom - 1);
    ui->mapWidget->resize(mapSize);
}

void GPXLab::resizeEvent(QResizeEvent* event)
{
    Q_UNUSED(event);
    update_size();
}

bool GPXLab::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Resize)
    {
        if (obj == ui->dockWidgetFile ||
            obj == ui->dockWidgetTracks ||
            obj == ui->dockWidgetDiagrams ||
            obj == ui->dockWidgetPoints)
            update_size();
    }
    return QWidget::eventFilter(obj, event);
}

void GPXLab::on_actionShowFile_toggled(bool checked)
{
    QAction *action = ui->dockWidgetFile->toggleViewAction();
    if (action->isChecked() != checked)
        action->trigger();
}

void GPXLab::on_actionShowTracks_toggled(bool checked)
{
    QAction *action = ui->dockWidgetTracks->toggleViewAction();
    if (action->isChecked() != checked)
        action->trigger();
}

void GPXLab::on_actionShowDiagrams_toggled(bool checked)
{
    QAction *action = ui->dockWidgetDiagrams->toggleViewAction();
    if (action->isChecked() != checked)
        action->trigger();
}

void GPXLab::on_actionShowTrackPoints_toggled(bool checked)
{
    QAction *action = ui->dockWidgetPoints->toggleViewAction();
    if (action->isChecked() != checked)
        action->trigger();
}

void GPXLab::on_actionShowToolbar_toggled(bool checked)
{
    QAction *action = ui->mainToolBar->toggleViewAction();
    if (action->isChecked() != checked)
        action->trigger();
}

void GPXLab::on_dockWidgetFile_visibilityChanged(bool visible)
{
    ui->actionShowFile->setChecked(visible);
    update_size();
}

void GPXLab::on_dockWidgetTracks_visibilityChanged(bool visible)
{
    ui->actionShowTracks->setChecked(visible);
    update_size();
}

void GPXLab::on_dockWidgetDiagrams_visibilityChanged(bool visible)
{
    ui->actionShowDiagrams->setChecked(visible);
    update_size();
}

void GPXLab::on_dockWidgetPoints_visibilityChanged(bool visible)
{
    ui->actionShowTrackPoints->setChecked(visible);
    update_size();
}

void GPXLab::on_mainToolBar_visibilityChanged(bool visible)
{
    ui->actionShowToolbar->setChecked(visible);
    update_size();
}
