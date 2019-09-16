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

#include "gpxlab.h"
#include "ui_gpxlab.h"
#include "dialog_about.h"
#include "dialog_settings.h"
#include "dialog_modelproperties.h"
#include "dialog_trackproperties.h"
#include "dialog_srtm.h"
#include "dialog_datetime.h"
#include <QMessageBox>

#include "selectcommand.h"
#include "editfilepropertiescommand.h"
#include "edittrackpropertiescommand.h"
#include "setaltitudecommand.h"
#include "removetrackcommand.h"
#include "movetrackdowncommand.h"
#include "movetrackupcommand.h"
#include "splittrackcommand.h"
#include "combinetrackcommand.h"
#include "pointinsertcommand.h"
#include "pointdeletecommand.h"
#include "tracktimeshiftcommand.h"

//////////////////////////////////////////////////////////////////////////////

void GPXLab::on_actionAbout_triggered()
{
    Dialog_about *dlg = new Dialog_about(this);
    dlg->exec();
    delete dlg;
}

void GPXLab::on_actionSettings_triggered()
{
    Dialog_settings *dlg = new Dialog_settings(settings, this);
    dlg->exec();
    delete dlg;
}


void GPXLab::on_actionEdit_File_Properties_triggered()
{
    GPX_metadataType metadata = *gpxmw->getModelMetadata();
    Dialog_modelProperties *dlg = new Dialog_modelProperties(metadata, *gpxmw->getModelStats(), this);
    if(dlg->exec() == QDialog::Accepted)
        undoStack->push(new EditFilePropertiesCommand(gpxmw, metadata));
    delete dlg;
}

void GPXLab::on_pushButtonEditFileProperties_clicked()
{
    on_actionEdit_File_Properties_triggered();
}

void GPXLab::on_actionEdit_Track_Properties_triggered()
{
    const GPX_statsType *stats = gpxmw->getTrackStats(gpxmw->getSelectedTrackNumber());
    if (stats)
    {
        GPX_trkMetadataType metadata = *gpxmw->getTrackMetadata(gpxmw->getSelectedTrackNumber());
        Dialog_trackProperties *dlg = new Dialog_trackProperties(metadata, *stats, this);
        if(dlg->exec() == QDialog::Accepted)
            undoStack->push(new EditTrackPropertiesCommand(gpxmw, gpxmw->getSelectedTrackNumber(), metadata));
        delete dlg;
    }
}

void GPXLab::on_pushButtonEditTrackProperties_clicked()
{
    on_actionEdit_Track_Properties_triggered();
}

void GPXLab::tree_doubleClicked(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(item);
    Q_UNUSED(column);
    on_actionEdit_Track_Properties_triggered();
}

void GPXLab::on_actionGetAltitudeFromSRTM_triggered()
{
    SelectCommand selectCommand(gpxmw, gpxmw->getSelectedTrackNumber());
    int selectedTrackSegmentNumber = gpxmw->getSelectedTrackSegmentNumber();
    if (selectedTrackSegmentNumber != -1)
    {
        // select whole track
        selectCommand.redo();
    }

    // open SRTM dialog
    Dialog_srtm *dlg = new Dialog_srtm(gpxmw, this);
    if(dlg->exec() == QDialog::Accepted)
        undoStack->push(new SetAltitudeCommand(gpxmw, gpxmw->getSelectedTrackNumber(), dlg->getValues()));

    if (selectedTrackSegmentNumber != -1)
    {
        // re-select track segment
        selectCommand.undo();
    }

    delete dlg;
}

void GPXLab::on_actionSetStartTime_triggered()
{
    const GPX_statsType *stats = gpxmw->getTrackStats(gpxmw->getSelectedTrackNumber());
    if (stats)
    {
        Dialog_DateTime *dlg = new Dialog_DateTime("Set Start Time", stats->startTime, this);
        if(dlg->exec() == QDialog::Accepted)
            undoStack->push(new TrackTimeShiftCommand(gpxmw, gpxmw->getSelectedTrackNumber(), dlg->getTimestamp() - stats->startTime));
        delete dlg;
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
        {
            if (!openFile(action->data().toString()))
            {
                // remove file from recent file list
                settings->removeFromRecentFile(action->data().toString());
                updateRecentFiles();
            }
        }
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
    if (!gpxmw->getFileName().isEmpty())
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
    if (gpxmw->getSelectedTrackNumber() >= 0)
    {
       undoStack->push(new RemoveTrackCommand(gpxmw, gpxmw->getSelectedTrackNumber()));
    }
}

void GPXLab::on_pushButtonTrackMoveDown_clicked()
{
    if (gpxmw->getSelectedTrackNumber() < gpxmw->getNumTracks() - 1)
    {
        undoStack->push(new MoveTrackDownCommand(gpxmw, gpxmw->getSelectedTrackNumber()));
    }
}

void GPXLab::on_pushButtonTrackMoveUp_clicked()
{
    if (gpxmw->getSelectedTrackNumber() > 0)
    {
        undoStack->push(new MoveTrackUpCommand(gpxmw, gpxmw->getSelectedTrackNumber()));
    }
}

void GPXLab::on_actionSplit_Track_triggered()
{
    if (gpxmw->getSelectedTrackNumber() >= 0)
    {
        undoStack->push(new SplitTrackCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber()));
    }
}

void GPXLab::on_actionCombine_Track_triggered()
{
    if (gpxmw->getSelectedTrackNumber() >= 0)
    {
        undoStack->push(new CombineTrackCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber()));
    }
}

void GPXLab::on_actionInsert_Point_triggered()
{
    if (gpxmw->getSelectedTrackNumber() >= 0)
    {
        int pointNumber = gpxmw->getSelectedPointNumber();
        if (pointNumber < gpxmw->getNumPoints(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber()) - 1)
        {
            const GPX_wptType *wptA = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber);
            const GPX_wptType *wptB = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber + 1);
            if (wptA && wptB)
            {
                GPX_wptType wpt;
                wpt.altitude = wptA->altitude + (wptB->altitude - wptA->altitude)/2;
                wpt.latitude = wptA->latitude + (wptB->latitude - wptA->latitude)/2;
                wpt.longitude = wptA->longitude + (wptB->longitude - wptA->longitude)/2;
                wpt.timestamp = wptA->timestamp + (wptB->timestamp - wptA->timestamp)/2;
                undoStack->push(new PointInsertCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber + 1, wpt));
            }
        }
    }
}

void GPXLab::on_actionDelete_Point_triggered()
{
    if (gpxmw->getSelectedTrackNumber() >= 0)
    {
        undoStack->push(new PointDeleteCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber()));
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::on_actionFit_View_triggered()
{
    const GPX_metadataType *metadata = gpxmw->getModelMetadata();
    ui->mapWidget->setViewAndZoomIn(metadata->bounds);
}

void GPXLab::on_actionFit_Track_triggered()
{
    const GPX_statsType *stats = gpxmw->getTrackStats(gpxmw->getSelectedTrackNumber());
    if (stats)
        ui->mapWidget->setViewAndZoomIn(stats->bounds);
}

void GPXLab::on_actionFit_Point_triggered()
{
    ui->mapWidget->setViewAndZoomInSelectedPoint();
}

void GPXLab::on_actionFollow_Item_toggled(bool follow)
{
    settings->setValue("FollowItem", QVariant(follow));
    ui->mapWidget->setFollowSelection(follow);
}

void GPXLab::on_actionShow_Only_Selected_Track_toggled(bool checked)
{
    settings->setValue("ShowOnlySelectedItem", QVariant(checked));
    ui->mapWidget->setShowOnlySelectedTrack(checked);
    ui->mapWidget->selectTrack(gpxmw->getSelectedTrackNumber());
}

void GPXLab::on_actionRestore_Default_View_triggered()
{
    settings->restoreLayout();
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

void GPXLab::on_dockWidgetFile_visibilityChanged(bool visible)
{
    Q_UNUSED(visible);
    update_size();
}

void GPXLab::on_dockWidgetTracks_visibilityChanged(bool visible)
{
    Q_UNUSED(visible);
    update_size();
}

void GPXLab::on_dockWidgetDiagrams_visibilityChanged(bool visible)
{
    Q_UNUSED(visible);
    update_size();
}

void GPXLab::on_dockWidgetPoints_visibilityChanged(bool visible)
{
    Q_UNUSED(visible);
    update_size();
}

void GPXLab::on_mainToolBar_visibilityChanged(bool visible)
{
    Q_UNUSED(visible);
    update_size();
}
