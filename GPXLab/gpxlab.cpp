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
#include "qutils.h"
#include "appendtrackcommand.h"

//////////////////////////////////////////////////////////////////////////////

const QString GPXLab::organisationName = "BourgeoisLab";

const QString GPXLab::appName = "GPXLab";

const QString GPXLab::appVersion = "0.3.0";

const QColor GPXLab::appColor = QColor(0, 200, 50);

//////////////////////////////////////////////////////////////////////////////

GPXLab::GPXLab(const QString &fileName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GPXLab),
    closing(false)
{
    // setup UI
    ui->setupUi(this);

    // create GPX_model wrapper
    gpxmw = new GPX_wrapper(appName);
    connect(gpxmw, SIGNAL(fileLoaded()), this, SLOT(fileLoaded()));
    connect(gpxmw, SIGNAL(fileSaved()), this, SLOT(fileSaved()));
    connect(gpxmw, SIGNAL(modelCleared()), this, SLOT(modelCleared()));
    connect(gpxmw, SIGNAL(modelPropertiesChanged()), this, SLOT(modelPropertiesChanged()));
    connect(gpxmw, SIGNAL(modelMetadataChanged()), this, SLOT(modelMetadataChanged()));
    connect(gpxmw, SIGNAL(trackMetadataChanged(int)), this, SLOT(trackMetadataChanged(int)));
    connect(gpxmw, SIGNAL(trackInserted(int, const GPX_trkType&)), this, SLOT(trackInserted(int, const GPX_trkType&)));
    connect(gpxmw, SIGNAL(trackDeleted(int)), this, SLOT(trackDeleted(int)));
    connect(gpxmw, SIGNAL(trackMovedUp(int)), this, SLOT(trackMovedUp(int)));
    connect(gpxmw, SIGNAL(trackMovedDown(int)), this, SLOT(trackMovedDown(int)));
    connect(gpxmw, SIGNAL(trackSplited(int)), this, SLOT(trackSplited(int)));
    connect(gpxmw, SIGNAL(trackCombined(int)), this, SLOT(trackCombined(int)));
    connect(gpxmw, SIGNAL(trackTimeShifted(int, long)), this, SLOT(trackTimeShifted(int, long)));
    connect(gpxmw, SIGNAL(trackSelectionChanged(int, int, int, const GPX_wptType*)), this, SLOT(trackSelectionChanged(int, int, int, const GPX_wptType*)));
    connect(gpxmw, SIGNAL(pointEdited(int, int, int, GPX_wrapper::TrackPointProperty)), this, SLOT(pointEdited(int, int, int, GPX_wrapper::TrackPointProperty)));
    connect(gpxmw, SIGNAL(pointInserted(int, int, int, const GPX_wptType&)), this, SLOT(pointInserted(int, int, int, const GPX_wptType&)));
    connect(gpxmw, SIGNAL(pointDeleted(int, int, int)), this, SLOT(pointDeleted(int, int, int)));
    connect(gpxmw, SIGNAL(pointSelectionChanged(int, const GPX_wptType*)), this, SLOT(pointSelectionChanged(int, const GPX_wptType*)));

    // set window title
    setMainWindowTitle();

    // load settings
    settings = new Settings(this);
    settings->load();

    // undo stack
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(settings->undoLimit);
    QAction *undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    QAction *redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon(":/images/redo.png"));
    connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(setMainWindowTitle()));

    // tree widget
    connect(ui->treeTracks, SIGNAL(itemDoubleClicked(QTreeWidgetItem* , int)), this, SLOT(tree_doubleClicked(QTreeWidgetItem*, int)));

    // map widget
    ui->mapWidget->init(gpxmw, undoStack, settings->doPersistentCaching, settings->cachePath);
    ui->mapWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->mapWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(map_showContextMenu(const QPoint&)));
    connect(ui->mapWidget, SIGNAL(viewChanged(const QPointF&, int)), this, SLOT(map_viewChanged(const QPointF&, int)));

    // calendar widget
    ui->calendarWidget->init(gpxmw);

    // table widget
    ui->tableWidgetPoints->init(gpxmw, undoStack);
    connect(settings, SIGNAL(settingsChanged(bool)), ui->tableWidgetPoints, SLOT(settingsChanged(bool)));

    // diagram widget
    ui->diagramWidget->addAction(ui->dockWidgetDiagrams->toggleViewAction());
    ui->diagramWidget->init(gpxmw);

    // zoom slider widget
    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setMinimumWidth(10);
    zoomSlider->setMaximumWidth(100);
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(0);
    ui->mainToolBar->insertWidget(ui->actionMapZoom, zoomSlider);
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoom_valueChanged(int)));

    // build recent files action
    actionOpenRecentFile = new QAction*[settings->maxRecentFiles];
    for (int i = 0; i < settings->maxRecentFiles; ++i)
    {
        actionOpenRecentFile[i] = new QAction(this);
        actionOpenRecentFile[i]->setVisible(false);
        connect(actionOpenRecentFile[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
        ui->menuFile->insertAction(ui->actionExit, actionOpenRecentFile[i]);
    }
    ui->menuFile->insertSeparator(ui->actionExit);
    updateRecentFiles();

    // menu edit
    ui->menuEdit->addAction(undoAction);
    ui->menuEdit->addAction(redoAction);

    // menu view
    ui->menuView->addAction(ui->dockWidgetFile->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetTracks->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetDiagrams->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetPoints->toggleViewAction());
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->actionRestore_Default_View);

    // install event filters for dock widgets to catch resize events
    ui->dockWidgetFile->installEventFilter(this);
    ui->dockWidgetTracks->installEventFilter(this);
    ui->dockWidgetDiagrams->installEventFilter(this);
    ui->dockWidgetPoints->installEventFilter(this);

    // dock file
    ui->dockWidgetFile->addAction(ui->dockWidgetFile->toggleViewAction());
    ui->dockWidgetFile->addAction(ui->actionEdit_File_Properties);

    // dock tracks
    ui->dockWidgetTracks->addAction(ui->dockWidgetTracks->toggleViewAction());
    ui->dockWidgetTracks->addAction(ui->actionEdit_Track_Properties);
    ui->dockWidgetTracks->addAction(ui->actionGetAltitudeFromSRTM);
    ui->dockWidgetTracks->addAction(ui->actionSetStartTime);

    // dock diagram
    ui->dockWidgetDiagrams->addActions(ui->diagramWidget->actions());

    // dock points
    ui->dockWidgetPoints->addAction(ui->dockWidgetPoints->toggleViewAction());
    ui->dockWidgetPoints->addAction(ui->actionInsert_Point);
    ui->dockWidgetPoints->addAction(ui->actionDelete_Point);

    // default context menu
    addActions(ui->menuView->actions());

    // connect signals for track and point selection
    connect(ui->diagramWidget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(diagram_selectionChanged(QMouseEvent*)));
    connect(ui->mapWidget, SIGNAL(selectionChanged(int, int, double, double)), this, SLOT(map_selectionChanged(int, int, double, double)));
    connect(ui->mapWidget, SIGNAL(selectionChanged(int)), this, SLOT(map_selectionChanged(int)));
    connect(ui->tableWidgetPoints, SIGNAL(selectionChanged(int)), this, SLOT(table_selectionChanged(int)));
    connect(ui->treeTracks, SIGNAL(itemSelectionChanged()), this, SLOT(tree_selectionChanged()));
    connect(ui->calendarWidget, SIGNAL(selectionChanged(int)), this, SLOT(cal_selectionChanged(int)));

    // open file if any passed as argument
    if (!fileName.isEmpty())
    {
        openFile(fileName);
    }
}

GPXLab::~GPXLab()
{
    delete ui;
    delete gpxmw;
    delete settings;
}

bool GPXLab::openFile(QString fileName)
{
    bool retValue = false;
    GPX_model::fileType_e fileType = GPX_model::GPXM_FILE_AUTOMATIC;

    if (fileName.isEmpty())
    {
        QString selectedFilter;
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "GPX (*.gpx);;NMEA (*.txt *.nmea);; SpoQ (*.act *.xml)", &selectedFilter);
        if (selectedFilter == "GPX (*.gpx)")
            fileType = GPX_model::GPXM_FILE_GPX;
        else if (selectedFilter == "NMEA (*.txt *.nmea)")
            fileType = GPX_model::GPXM_FILE_NMEA;
        else if (selectedFilter == "SpoQ (*.act *.xml)")
            fileType = GPX_model::GPXM_FILE_ACT;
    }

    if (!fileName.isEmpty())
    {
        // clear undo stack
        undoStack->clear();

        // initialize GPX_model wrapper
        gpxmw->init(appName);

        // load file
        ui->statusBar->showMessage(tr("Loading file: ") + fileName);
        if (gpxmw->load(fileName, fileType, true) == GPX_model::GPXM_OK)
        {
            // set window title
            setMainWindowTitle();

            // add file to recent file list
            settings->addToRecentFile(fileName);
            updateRecentFiles();

            // select first track
            gpxmw->select(0);

            // enable actions
            updateActions(true);   

            retValue = true;
        }
        else
        {
            QMessageBox::critical(this, appName, tr("Failed to open \"") + fileName + "\".");
            closeFile();
        }
        ui->statusBar->showMessage("");
    }
    return retValue;
}

void GPXLab::appendFiles(QStringList &fileNames)
{
    GPX_model::fileType_e fileType = GPX_model::GPXM_FILE_AUTOMATIC;
    if (fileNames.isEmpty())
    {
        QString selectedFilter;
        fileNames = QFileDialog::getOpenFileNames(this, tr("Append Files"), "", "GPX (*.gpx);;NMEA (*.txt *.nmea);; SpoQ (*.act *.xml)", &selectedFilter);
        if (selectedFilter == "GPX (*.gpx)")
            fileType = GPX_model::GPXM_FILE_GPX;
        else if (selectedFilter == "NMEA (*.txt *.nmea)")
            fileType = GPX_model::GPXM_FILE_NMEA;
        else if (selectedFilter == "SpoQ (*.act *.xml)")
            fileType = GPX_model::GPXM_FILE_ACT;
    }
    if (!fileNames.isEmpty())
    {
        // load files
        for (int i = 0; i < fileNames.size(); ++i)
        {
            ui->statusBar->showMessage(tr("Appending file: ") + fileNames.at(i));
            undoStack->push(new AppendTrackCommand(gpxmw, fileNames.at(i), fileType));
        }
        ui->statusBar->showMessage("");

        // select last track
        gpxmw->select(gpxmw->getNumTracks() - 1);
    }
}

void GPXLab::saveFile(QString fileName)
{
    if (fileName.isEmpty())
        fileName = QFileDialog::getSaveFileName(this, tr("Save File"), gpxmw->getFileName(), "GPX (*.gpx)");
    if (!fileName.isEmpty())
    {
        // save file
        ui->statusBar->showMessage(tr("Saving file: ") + fileName);
        if (gpxmw->save(fileName) == GPX_model::GPXM_OK)
        {
            // clear undo stack
            undoStack->clear();

            // set window title
            setMainWindowTitle();

            // add file to recent file list
            settings->addToRecentFile(fileName);
            updateRecentFiles();
        }
        else
        {
            QMessageBox::critical(this, appName, tr("Failed to save \"") + fileName + "\".");
        }
        ui->statusBar->showMessage("");
    }
}

void GPXLab::closeFile()
{
    // clear GPX_model wrapper
    gpxmw->clear();

    // clear undo stack
    undoStack->clear();

    // set window title
    setMainWindowTitle();

    // disable actions
    updateActions(false);
}

void GPXLab::setMainWindowTitle()
{
    if (gpxmw->getFileName().isEmpty())
    {
        setWindowTitle(appName);
    }
    else
    {
        if (undoStack->isClean())
        {
            setWindowTitle(gpxmw->getFileName() + " - " + appName);
            ui->actionSave_File->setEnabled(false);
        }
        else
        {
            setWindowTitle("*" + gpxmw->getFileName() + " - " + appName);
            ui->actionSave_File->setEnabled(true);
        }
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::updateActions(bool enabled)
{
    ui->actionAppend_File->setEnabled(enabled);
    ui->actionSave_File_As->setEnabled(enabled);
    ui->actionClose_File->setEnabled(enabled);
    ui->actionEdit_File_Properties->setEnabled(enabled);
    ui->actionEdit_Track_Properties->setEnabled(enabled);
    ui->actionGetAltitudeFromSRTM->setEnabled(enabled);
    ui->actionFit_View->setEnabled(enabled);
    ui->actionFit_Track->setEnabled(enabled);
    ui->actionFit_Point->setEnabled(enabled);
    ui->actionFollow_Item->setEnabled(enabled);
    ui->actionShow_Only_Selected_Track->setEnabled(enabled);
    ui->pushButtonTrackAppend->setEnabled(enabled);
    ui->pushButtonTrackRemove->setEnabled(enabled);
    ui->pushButtonTrackMoveUp->setEnabled(enabled);
    ui->pushButtonTrackMoveDown->setEnabled(enabled);
    ui->actionSplit_Track->setEnabled(enabled);
    ui->actionCombine_Track->setEnabled(enabled);
    ui->actionInsert_Point->setEnabled(enabled);
    ui->actionDelete_Point->setEnabled(enabled);
    ui->actionSetStartTime->setEnabled(enabled);
}

void GPXLab::updateRecentFiles()
{
    int numRecentFiles = qMin(settings->recentFiles.size(), settings->maxRecentFiles);
    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = QString::number(i + 1) + " " +  settings->recentFiles[i];
        actionOpenRecentFile[i]->setText(text);
        actionOpenRecentFile[i]->setData(settings->recentFiles[i]);
        actionOpenRecentFile[i]->setVisible(true);
    }
    for (int i = numRecentFiles; i < settings->maxRecentFiles; ++i)
    {
        actionOpenRecentFile[i]->setVisible(false);
    }
}

void GPXLab::updateFileProperties(bool clear)
{
    if (!clear)
    {
        const GPX_statsType *stats = gpxmw->getModelStats();
        const GPX_metadataType *metadata = gpxmw->getModelMetadata();
        ui->labelFileName->setText(QString::fromStdString(metadata->name));
        ui->labelFileTracks->setText(QString::number(gpxmw->getNumTracks()));
        ui->labelFileDuration->setText(QUtils::seconds_to_DHMS(stats->duration));
        ui->labelFileDistance->setText(QString::number(stats->distance, 'f', 1) + " km");
        ui->labelFileSpeed->setText(QString::number(stats->speed, 'f', 1) + " km/h");
        ui->labelFileHeightIntUp->setText(QString::number(stats->heightIntUp, 'f', 0) + " m");
        ui->labelFileHeightIntDown->setText(QString::number(stats->heightIntDown, 'f', 0) + " m");
    }
    else
    {
        ui->labelFileName->setText("");
        ui->labelFileTracks->setText("");
        ui->labelFileDuration->setText("");
        ui->labelFileDistance->setText("");
        ui->labelFileSpeed->setText("");
        ui->labelFileHeightIntUp->setText("");
        ui->labelFileHeightIntDown->setText("");
    }
}

void GPXLab::updateTrackProperties(bool clear)
{
    if (!clear)
    {
        ui->labelTrackName->setText(gpxmw->getItemName(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber()));
        const GPX_statsType *stats = gpxmw->getItemStats(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
        if (stats)
        {
            QDateTime timestamp;
            timestamp.setTime_t(stats->startTime);
            ui->labelTrackStartTime->setText(timestamp.toString("dd.MM.yyyy H:mm:ss"));
            timestamp.setTime_t(stats->endTime);
            ui->labelTrackEndTime->setText(timestamp.toString("dd.MM.yyyy H:mm:ss"));
            ui->labelTrackDuration->setText(QUtils::seconds_to_DHMS(stats->duration));
            ui->labelTrackDistance->setText(QString::number(stats->distance, 'f', 1) + " km");
            ui->labelTrackSpeed->setText(QString::number(stats->speed, 'f', 1) + " km/h");
            ui->labelTrackMinAltitude->setText(QString::number(stats->minhei, 'f', 0) + " m");
            ui->labelTrackMaxAltitude->setText(QString::number(stats->maxhei, 'f', 0) + " m");
            ui->labelTrackHeightIntUp->setText(QString::number(stats->heightIntUp, 'f', 0) + " m");
            ui->labelTrackHeightIntDown->setText(QString::number(stats->heightIntDown, 'f', 0) + " m");
            return;
        }
    }

    ui->labelTrackName->setText("");
    ui->labelTrackStartTime->setText("");
    ui->labelTrackEndTime->setText("");
    ui->labelTrackDuration->setText("");
    ui->labelTrackDistance->setText("");
    ui->labelTrackSpeed->setText("");
    ui->labelTrackMinAltitude->setText("");
    ui->labelTrackMaxAltitude->setText("");
    ui->labelTrackHeightIntUp->setText("");
    ui->labelTrackHeightIntDown->setText("");
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::fileLoaded()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    // file properties
    ui->statusBar->showMessage(tr("Updating file properties..."));
    updateFileProperties(false);

    // track tree
    ui->statusBar->showMessage(tr("Generating track tree..."));
    ui->treeTracks->build(gpxmw);

    // map
    ui->statusBar->showMessage(tr("Generating map..."));
    ui->mapWidget->build(true);

    // calendar
    ui->statusBar->showMessage(tr("Generating track calendar..."));
    ui->calendarWidget->build();

    ui->statusBar->showMessage("");
    QApplication::restoreOverrideCursor();
}

void GPXLab::fileSaved()
{
    // file properties
    updateFileProperties(false);
}

void GPXLab::modelCleared()
{
    // file properties
    updateFileProperties(true);

    // track properties
    updateTrackProperties(true);

    // track tree
    ui->treeTracks->clear();

    // map
    ui->mapWidget->clear();

    // calendar
    ui->calendarWidget->clear();

    // diagram
    ui->diagramWidget->clear();

    // point list
    ui->tableWidgetPoints->clear();
}

void GPXLab::modelPropertiesChanged()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);
    QApplication::processEvents();

    // file properties
    ui->statusBar->showMessage(tr("Updating file properties..."));
    updateFileProperties(false);

    // track properties
    ui->statusBar->showMessage(tr("Updating track properties..."));
    updateTrackProperties(false);

    // track tree
    ui->statusBar->showMessage(tr("Generating track tree..."));
    ui->treeTracks->build(gpxmw);
    ui->treeTracks->select(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());

    // calendar
    ui->statusBar->showMessage(tr("Generating track calendar..."));
    ui->calendarWidget->build();
    ui->calendarWidget->select(gpxmw->getSelectedTrackNumber());

    // diagram
    ui->statusBar->showMessage(tr("Generating diagram..."));
    ui->diagramWidget->build();

    // point list
    ui->statusBar->showMessage(tr("Generating point list..."));
    ui->tableWidgetPoints->build(0);

    ui->statusBar->showMessage("");
    QApplication::restoreOverrideCursor();
}

void GPXLab::modelMetadataChanged()
{
    // file properties
    updateFileProperties(false);
}

void GPXLab::trackMetadataChanged(int trackNumber)
{
    // track properties
    updateTrackProperties(false);

    // track tree
    ui->treeTracks->setItemName(gpxmw->getItemName(trackNumber), trackNumber);
}

void GPXLab::trackInserted(int trackNumber, const GPX_trkType &track)
{
    // file properties
    updateFileProperties(false);

    // track tree
    ui->treeTracks->build(gpxmw);

    // map
    ui->mapWidget->insertTrack(trackNumber, track);

    // calendar
    ui->calendarWidget->build();
}

void GPXLab::trackDeleted(int trackNumber)
{
    // file properties
    updateFileProperties(false);

    // track tree
    ui->treeTracks->build(gpxmw);

    // map
    ui->mapWidget->deleteTrack(trackNumber);

    // calendar
    ui->calendarWidget->build();
}

void GPXLab::trackMovedUp(int trackNumber)
{
    // track tree
    ui->treeTracks->build(gpxmw);

    // map
    ui->mapWidget->moveTrackUp(trackNumber);
}

void GPXLab::trackMovedDown(int trackNumber)
{
    // track tree
    ui->treeTracks->build(gpxmw);

    // map
    ui->mapWidget->moveTrackDown(trackNumber);
}

void GPXLab::trackSplited(int trackNumber)
{
    Q_UNUSED(trackNumber);

    // track tree
    ui->treeTracks->build(gpxmw);
}

void GPXLab::trackCombined(int trackNumber)
{
    Q_UNUSED(trackNumber);

    // track tree
    ui->treeTracks->build(gpxmw);
}

void GPXLab::trackTimeShifted(int trackNumber, long seconds)
{
    Q_UNUSED(trackNumber);
    Q_UNUSED(seconds);

    // track properties
    updateTrackProperties(false);

    // track tree
    ui->treeTracks->build(gpxmw);

    // calendar
    ui->calendarWidget->build();

    // diagram
    ui->diagramWidget->build();

    // point list
    ui->tableWidgetPoints->build(gpxmw->getSelectedPointNumber());
}

void GPXLab::trackSelectionChanged(int trackNumber, int trackSegmentNumber, int pointNumber, const GPX_wptType *trkpt)
{
    // track properties
    updateTrackProperties(false);

    // track tree
    ui->treeTracks->select(trackNumber, trackSegmentNumber);

    // map
    ui->mapWidget->selectTrack(trackNumber);
    if (trkpt)
        ui->mapWidget->selectedPointSetVisible(true, trkpt->latitude, trkpt->longitude);
    else
        ui->mapWidget->selectedPointSetVisible(false);

    // calendar
    ui->calendarWidget->select(trackNumber);

    // diagram
    ui->diagramWidget->build();
    if (trkpt)
        ui->diagramWidget->setMarkerValue(trkpt->timestamp);

    // point list
    ui->tableWidgetPoints->build(pointNumber);
}

void GPXLab::pointEdited(int trackNumber, int trackSegmentNumber, int pointNumber, GPX_wrapper::TrackPointProperty properties)
{
    Q_UNUSED(trackNumber);
    Q_UNUSED(trackSegmentNumber);
    Q_UNUSED(pointNumber);

    const GPX_wptType *trkpt = gpxmw->getSelectedPoint();

    // file properties
    updateFileProperties(false);

    // track properties
    updateTrackProperties(false);

    // map
    if (properties & GPX_wrapper::latitude || properties & GPX_wrapper::longitude)
    {
        ui->mapWidget->editPoint(trackNumber, pointNumber);
        if (trkpt)
            ui->mapWidget->selectedPointSetVisible(true, trkpt->latitude, trkpt->longitude);
    }

    // diagram
    if (properties & ui->diagramWidget->getCurveMain() || properties & ui->diagramWidget->getCurveSecondary())
    {
        ui->diagramWidget->build();
        if (trkpt)
            ui->diagramWidget->setMarkerValue(trkpt->timestamp);
    }

    // point list
    ui->tableWidgetPoints->update();
}

void GPXLab::pointInserted(int trackNumber, int trackSegmentNumber, int pointNumber, const GPX_wptType& trkpt)
{
    Q_UNUSED(trackNumber);
    Q_UNUSED(trackSegmentNumber);
    Q_UNUSED(pointNumber);
    Q_UNUSED(trkpt);

    // diagram
    ui->diagramWidget->build();

    // point list
    ui->tableWidgetPoints->build();
}

void GPXLab::pointDeleted(int trackNumber, int trackSegmentNumber, int pointNumber)
{
    Q_UNUSED(trackNumber);
    Q_UNUSED(trackSegmentNumber);
    Q_UNUSED(pointNumber);

    // diagram
    ui->diagramWidget->build();

    // point list
    ui->tableWidgetPoints->build();
}

void GPXLab::pointSelectionChanged(int pointNumber, const GPX_wptType *trkpt)
{
    if (trkpt)
    {
        ui->mapWidget->selectedPointSetVisible(true, trkpt->latitude, trkpt->longitude);
        ui->diagramWidget->setMarkerValue(trkpt->timestamp);
        ui->tableWidgetPoints->selectRow(pointNumber);
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::map_selectionChanged(int trackNumber, int trackSegmentNumber, double lat, double lon)
{
    if (closing)
        return;

    gpxmw->select(trackNumber, trackSegmentNumber, lat, lon);
}

void GPXLab::map_selectionChanged(int pointNumber)
{
    if (closing)
        return;

    gpxmw->select(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber);
}

void GPXLab::mapmenu_selectionChanged()
{
    if (closing)
        return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        gpxmw->select(action->data().toInt());
    }
}

void GPXLab::tree_selectionChanged()
{
    if (closing)
        return;

    QList<QTreeWidgetItem*> items = ui->treeTracks->selectedItems();
    if (items.count() > 0)
    {
        QList<QVariant> data = items[0]->data(0, Qt::UserRole).toList();
        gpxmw->select(data.at(0).toInt(), data.at(1).toInt());
    }
}

void GPXLab::table_selectionChanged(int pointNumber)
{
    if (closing)
        return;

    gpxmw->select(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber);
}

void GPXLab::diagram_selectionChanged(QMouseEvent* event)
{
    Q_UNUSED(event);

    if (closing)
        return;

    gpxmw->select(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), (time_t)ui->diagramWidget->getMarkerValue());
}

void GPXLab::cal_selectionChanged(int trackNumber)
{
    if (closing)
        return;

    gpxmw->select(trackNumber);
}

void GPXLab::map_viewChanged(const QPointF &coordinate, int zoom)
{
    Q_UNUSED(coordinate);

    if (closing)
        return;

    zoomSlider->setMinimum(ui->mapWidget->minZoom());
    zoomSlider->setMaximum(ui->mapWidget->maxZoom());
    zoomSlider->setValue(zoom);
}

void GPXLab::zoom_valueChanged(int value)
{
    if (closing)
        return;

    ui->mapWidget->setZoom(value);
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::map_showContextMenu(const QPoint& pos)
{
    QMenu menu;
    QMenu trackSubMenu;
    QAction *action;
    QAction *separator;

    trackSubMenu.setTitle(tr("Tracks"));
    for (int i = 0; i < gpxmw->getNumTracks(); ++i)
    {
        action = new QAction(this);
        action->setText(gpxmw->getItemName(i));
        action->setData(i);
        action->setCheckable(true);
        if (i == gpxmw->getSelectedTrackNumber())
        {
            action->setChecked(true);
        }
        else
        {
            action->setChecked(false);
            connect(action, SIGNAL(triggered()), this, SLOT(mapmenu_selectionChanged()));
        }
        trackSubMenu.addAction(action);
    }

    menu.addMenu(&trackSubMenu);
    separator = new QAction(this);
    separator->setSeparator(true);
    menu.addAction(separator);
    menu.addAction(ui->actionFit_View);
    menu.addAction(ui->actionFit_Track);
    menu.addAction(ui->actionFit_Point);
    separator = new QAction(this);
    separator->setSeparator(true);
    menu.addAction(separator);
    menu.addAction(ui->actionFollow_Item);
    menu.addAction(ui->actionShow_Only_Selected_Track);
    separator = new QAction(this);
    separator->setSeparator(true);
    menu.addAction(separator);
    menu.addAction(ui->actionSplit_Track);
    menu.addAction(ui->actionCombine_Track);
    separator = new QAction(this);
    separator->setSeparator(true);
    menu.addAction(separator);
    menu.addAction(ui->actionInsert_Point);
    menu.addAction(ui->actionDelete_Point);
    menu.exec(ui->mapWidget->mapToGlobal(pos));
}

void GPXLab::dropEvent(QDropEvent *event)
{
    if (check_need_save() != QMessageBox::Cancel)
    {
        const QMimeData* mimeData = event->mimeData();
        if (mimeData->hasUrls())
        {
            QList<QUrl> urlList = mimeData->urls();
            openFile(urlList.at(0).toLocalFile());
        }
    }
}

void GPXLab::dragEnterEvent(QDragEnterEvent *event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        if (GPX_model::getFileType(urlList.at(0).toLocalFile().toStdString()) != GPX_model::GPXM_FILE_NOT_SUPPORTED)
            event->accept();
    }
}

int GPXLab::check_need_save()
{
    int ret = QMessageBox::NoButton;
    if (!undoStack->isClean())
    {
        QMessageBox msgBox;
        msgBox.setText(tr("The file has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        ret = msgBox.exec();
        if (ret == QMessageBox::Save)
            saveFile(gpxmw->getFileName());
    }
    return ret;
}

void GPXLab::closeEvent(QCloseEvent *event)
{
    if (check_need_save() == QMessageBox::Cancel)
    {
        event->ignore();
    }
    else
    {
        event->accept();
        closing = true;
        settings->save();
    }
}
