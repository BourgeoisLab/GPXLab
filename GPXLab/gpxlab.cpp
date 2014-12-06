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
#include "qutils.h"

#include "selectcommand.h"
#include "appendtrackcommand.h"

//////////////////////////////////////////////////////////////////////////////

const QString GPXLab::organisationName = "BourgeoisLab";

const QString GPXLab::appName = "GPXLab";

const QString GPXLab::appVersion = "0.2.0";

//////////////////////////////////////////////////////////////////////////////

GPXLab::GPXLab(const QString &fileName, QWidget *parent) :
    QMainWindow(parent),
    gpxmw(NULL),
    ui(new Ui::GPXLab),
    updatingLevel(0),
    closing(false)
{
    // setup UI
    ui->setupUi(this);

    // store default settings
    defaultState = saveState();
    defaultGeometry = saveGeometry();

    // load settings
    loadSettings();

    // set window title
    setWindowTitle(appName);

    // initialize widgets
    ui->mapWidget->init(settings.doPersistentCaching, -1, settings.cachePath);


    zoomSlider = new QSlider(Qt::Horizontal, this);
    zoomSlider->setMinimumWidth(10);
    zoomSlider->setMaximumWidth(100);
    zoomSlider->setMinimum(0);
    zoomSlider->setMaximum(0);
    ui->mainToolBar->insertWidget(ui->actionMapZoom, zoomSlider);
    connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(zoom_valueChanged(int)));
    connect(ui->mapWidget, SIGNAL(viewChanged(const QPointF&, int)), this, SLOT(map_viewChanged(const QPointF&, int)));

    // undo stack
    undoStack = new QUndoStack(this);
    undoStack->setUndoLimit(undoLimit);
    undoAction = undoStack->createUndoAction(this, tr("&Undo"));
    undoAction->setShortcuts(QKeySequence::Undo);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    redoAction = undoStack->createRedoAction(this, tr("&Redo"));
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setIcon(QIcon(":/images/redo.png"));
    ui->menuEdit->addAction(undoAction);
    ui->menuEdit->addAction(redoAction);
    ui->menuEdit->addSeparator();
    connect(undoStack, SIGNAL(indexChanged(int)), this, SLOT(undo_indexChanged(int)));

    // build recent files action
    for (int i = 0; i < MaxRecentFiles; ++i)
    {
        actionOpenRecentFile[i] = new QAction(this);
        actionOpenRecentFile[i]->setVisible(false);
        connect(actionOpenRecentFile[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
        ui->menuFile->insertAction(ui->actionExit, actionOpenRecentFile[i]);
    }
    ui->menuFile->insertSeparator(ui->actionExit);
    updateRecentFileActions();

    // install event filters for dock widgets to catch resize events
    ui->dockWidgetFile->installEventFilter(this);
    ui->dockWidgetTracks->installEventFilter(this);
    ui->dockWidgetDiagrams->installEventFilter(this);
    ui->dockWidgetPoints->installEventFilter(this);

    // build context menus
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    addAction(ui->dockWidgetFile->toggleViewAction());
    addAction(ui->dockWidgetTracks->toggleViewAction());
    addAction(ui->dockWidgetDiagrams->toggleViewAction());
    addAction(ui->dockWidgetPoints->toggleViewAction());
    addAction(ui->mainToolBar->toggleViewAction());
    addAction(separator);
    addAction(ui->actionRestore_Default_View);
    ui->menuView->addAction(ui->dockWidgetFile->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetTracks->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetDiagrams->toggleViewAction());
    ui->menuView->addAction(ui->dockWidgetPoints->toggleViewAction());
    ui->menuView->addAction(ui->mainToolBar->toggleViewAction());
    ui->menuView->addSeparator();
    ui->menuView->addAction(ui->actionRestore_Default_View);
    ui->dockWidgetFile->addAction(ui->dockWidgetFile->toggleViewAction());
    ui->dockWidgetFile->addAction(ui->actionEdit_File_Properties);
    ui->dockWidgetTracks->addAction(ui->dockWidgetTracks->toggleViewAction());
    ui->dockWidgetTracks->addAction(ui->actionEdit_Track_Properties);
    ui->dockWidgetTracks->addAction(ui->actionGetAltitudeFromSRTM);
    ui->dockWidgetDiagrams->addAction(ui->dockWidgetDiagrams->toggleViewAction());
    ui->diagramWidget->addAction(ui->dockWidgetDiagrams->toggleViewAction());
    ui->dockWidgetPoints->addAction(ui->dockWidgetPoints->toggleViewAction());
    ui->mapWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->mapWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(map_showContextMenu(const QPoint&)));

    // connect signals for track & track point selection
    connect(ui->diagramWidget, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(diagram_selectionChanged(QMouseEvent*)));
    connect(ui->mapWidget, SIGNAL(selectionChanged(int, int, double, double)), this, SLOT(map_selectionChanged(int, int, double, double)));
    connect(ui->tableWidgetPoints, SIGNAL(selectionChanged(int)), this, SLOT(table_selectionChanged(int)));
    connect(ui->treeTracks, SIGNAL(itemDoubleClicked(QTreeWidgetItem* , int)), this, SLOT(tree_doubleClicked(QTreeWidgetItem*, int)));
    connect(ui->treeTracks, SIGNAL(itemSelectionChanged()), this, SLOT(tree_selectionChanged()));

    // open file if any passed as argument
    if (!fileName.isEmpty())
    {
        openFile(fileName);
    }
}

GPXLab::~GPXLab()
{
    delete ui;
    if (gpxmw)
        delete gpxmw;
}

void GPXLab::loadSettings()
{
    QSettings qsettings;
    restoreGeometry(qsettings.value("geometry").toByteArray());
    restoreState(qsettings.value("windowState").toByteArray());
    ui->tableWidgetPoints->restoreState(qsettings.value("tableState").toByteArray());
    settings.load();
}

void GPXLab::saveSettings()
{
    QSettings qsettings;
    qsettings.setValue("geometry", saveGeometry());
    qsettings.setValue("windowState", saveState());
    qsettings.setValue("tableState", ui->tableWidgetPoints->saveState());
    settings.save();
}

void GPXLab::openFile(QString fileName)
{
    if (fileName.isEmpty())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "GPX (*.gpx);;NMEA (*.txt *.nmea)");
    if (!fileName.isEmpty())
    {
        SelectCommand selectCommand(this, 0);
        const GPX_metadataType *metadata;

        // delete old GPX_Wrapper if necessary
        if (gpxmw != NULL)
            delete gpxmw;

        // create new GPX_Wrapper
        gpxmw = new GPX_wrapper(appName);

        // load file
        ui->statusBar->showMessage(tr("Loading file: ") + fileName);
        switch (gpxmw->load(fileName, true))
        {
        case GPX_model::GPXM_OK:

            // set window title
            setWindowTitle(gpxmw->getFileName() + " - " + appName);

            // set current file
            setCurrentFile(fileName);

            // update file properties
            updateFile();

            // begin update of track widgets
            beginUpdate();

            // build stuff
            build();

            // center map and zoom in
            metadata = gpxmw->getModelMetadata();
            ui->mapWidget->setViewAndZoomIn(metadata->bounds);

            // select first track
            selectCommand.redo();

            // end update
            endUpdate();

            // enable / disable actions
            ui->actionAppend_File->setEnabled(true);
            ui->actionSave_File->setEnabled(false);
            ui->actionSave_File_As->setEnabled(true);
            ui->actionClose_File->setEnabled(true);
            ui->actionEdit_File_Properties->setEnabled(true);
            ui->actionEdit_Track_Properties->setEnabled(true);
            ui->actionGetAltitudeFromSRTM->setEnabled(true);
            ui->actionFit_View->setEnabled(true);
            ui->actionFit_Track->setEnabled(true);
            ui->actionFit_Point->setEnabled(true);
            ui->actionFollow_Item->setEnabled(true);
            ui->actionShow_Only_Selected_Track->setEnabled(true);
            ui->pushButtonTrackAppend->setEnabled(true);
            ui->pushButtonTrackRemove->setEnabled(true);
            ui->pushButtonTrackMoveUp->setEnabled(true);
            ui->pushButtonTrackMoveDown->setEnabled(true);
            ui->actionSplit_Track->setEnabled(true);
            ui->actionCombine_Track->setEnabled(true);
            break;

        default:
            QMessageBox::critical(this, appName, tr("Failed to open \"") + fileName + "\".");
            break;
        }
        ui->statusBar->showMessage("");
    }
}

void GPXLab::appendFiles(QStringList &fileNames)
{
    if (gpxmw)
    {
        if (fileNames.isEmpty())
            fileNames = QFileDialog::getOpenFileNames(this, tr("Append Files"), "", "GPX (*.gpx);;NMEA (*.txt *.nmea)");
        if (!fileNames.isEmpty())
        {
            // load files
            for (int i = 0; i < fileNames.size(); ++i)
            {
                ui->statusBar->showMessage(tr("Loading file: ") + fileNames.at(i));
                undoStack->push(new AppendTrackCommand(this, fileNames.at(i)));
            }
            ui->statusBar->showMessage("");

            // center map and zoom in
            const GPX_metadataType *metadata = gpxmw->getModelMetadata();
            ui->mapWidget->setViewAndZoomIn(metadata->bounds);

            // select last track
            SelectCommand selectCommand(this, gpxmw->getNumTracks() - 1);
            selectCommand.redo();
        }
    }
}

void GPXLab::saveFile(QString fileName)
{
    if (gpxmw)
    {
        if (fileName.isEmpty())
            fileName = QFileDialog::getSaveFileName(this, tr("Save File"), gpxmw->getFileName(), "GPX (*.gpx)");
        if (!fileName.isEmpty())
        {
            // save file
            switch(gpxmw->save(fileName))
            {
                case GPX_model::GPXM_OK:

                    // set window title
                    setWindowTitle(gpxmw->getFileName() + " - " + appName);

                    // set current file
                    setCurrentFile(fileName);

                    // update file properties (in case file name changed)
                    updateFile();

                    // begin update of track widgets
                    beginUpdate();

                    // end update
                    endUpdate();

                    // enable / disable actions
                    ui->actionSave_File->setEnabled(false);

                    // clear undo stack
                    undoStack->clear();
                break;

                default:
                    QMessageBox::critical(this, appName, tr("Failed to save \"") + fileName + "\".");
                    break;
            }
        }
    }
}

void GPXLab::closeFile()
{
    if (gpxmw)
    {
        // delete GPX_Wrapper
        delete gpxmw;
        gpxmw = NULL;

        // clear file properties
        updateFile();

        // begin update of track widgets
        beginUpdate();

        // clear widgets
        updateTrack();
        updatePoint();

        // end update
        endUpdate();

        // disable actions
        ui->actionAppend_File->setEnabled(false);
        ui->actionSave_File_As->setEnabled(false);
        ui->actionClose_File->setEnabled(false);
        ui->actionEdit_File_Properties->setEnabled(false);
        ui->actionEdit_Track_Properties->setEnabled(false);
        ui->actionGetAltitudeFromSRTM->setEnabled(false);
        ui->actionFit_View->setEnabled(false);
        ui->actionFit_Track->setEnabled(false);
        ui->actionFit_Point->setEnabled(false);
        ui->actionFollow_Item->setEnabled(false);
        ui->actionShow_Only_Selected_Track->setEnabled(false);
        ui->pushButtonTrackAppend->setEnabled(false);
        ui->pushButtonTrackRemove->setEnabled(false);
        ui->pushButtonTrackMoveUp->setEnabled(false);
        ui->pushButtonTrackMoveDown->setEnabled(false);
        ui->actionSplit_Track->setEnabled(false);
        ui->actionCombine_Track->setEnabled(false);

        // clear undo stack
        undoStack->clear();
    }
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::beginUpdate()
{
    ++updatingLevel;
    ui->mapWidget->suspendUpdate(true);
}

void GPXLab::endUpdate()
{
    --updatingLevel;
    ui->mapWidget->suspendUpdate(false);
    if (updatingLevel == 0)
    {
        ui->mapWidget->updateRequestNew();
    }
}

void GPXLab::updateFile()
{
    if (gpxmw)
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();
        ui->statusBar->showMessage(tr("Updating file properties..."));
        const GPX_statsType *stats = gpxmw->getModelStats();
        const GPX_metadataType *metadata = gpxmw->getModelMetadata();
        ui->labelFileName->setText(QString::fromStdString(metadata->name));
        ui->labelFileDuration->setText(QUtils::seconds_to_DHMS(stats->duration));
        ui->labelFileDistance->setText(QString::number(stats->distance, 'f', 1) + " km");
        ui->labelFileSpeed->setText(QString::number(stats->speed, 'f', 1) + " km/h");
        ui->labelFileHeightIntUp->setText(QString::number(stats->heightIntUp, 'f', 0) + " m");
        ui->labelFileHeightIntDown->setText(QString::number(stats->heightIntDown, 'f', 0) + " m");
        ui->statusBar->showMessage("");
        QApplication::restoreOverrideCursor();
    }
    else
    {
        // clear file properties
        ui->labelFileName->setText("");
        ui->labelFileDuration->setText("");
        ui->labelFileDistance->setText("");
        ui->labelFileSpeed->setText("");
        ui->labelFileHeightIntUp->setText("");
        ui->labelFileHeightIntDown->setText("");
    }
}

void GPXLab::updateTrack()
{
    if (gpxmw)
    {
        const GPX_statsType *stats = gpxmw->getItemStats(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
        if (stats)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::processEvents();

            // Track properties
            ui->statusBar->showMessage(tr("Updating track properties..."));
            QDateTime timestamp;
            ui->labelTrackName->setText(gpxmw->getItemName(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber()));
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

            // Map
            ui->statusBar->showMessage(tr("Updating map..."));
            ui->mapWidget->selectTrack(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());

            // Map menu
            ui->statusBar->showMessage(tr("Updating map menu..."));
            foreach (QAction *action, mapContextMenu.actions())
            {
                if (action->data().toInt() == gpxmw->getSelectedTrackNumber())
                {
                    action->setChecked(true);
                    action->setEnabled(false);
                }
                else
                {
                    action->setChecked(false);
                    action->setEnabled(true);
                }
            }

            // Track tree
            ui->statusBar->showMessage(tr("Updating track tree..."));
            ui->treeTracks->select(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());

            // Diagram
            ui->statusBar->showMessage(tr("Updating diagram..."));
            gpxmw->generateDiagramValues(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
            ui->diagramWidget->setData(gpxmw->getDiagramTimeValues(), gpxmw->getDiagramAltitudeValues(), gpxmw->getDiagramSpeedValues());

            // Point list
            ui->statusBar->showMessage(tr("Updating point list..."));
            ui->tableWidgetPoints->build(gpxmw);

            ui->statusBar->showMessage("");
            QApplication::restoreOverrideCursor();
            return;
        }
    }

    // clear track properties
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

    // clear track tree
    ui->treeTracks->clear();

    // clear map
    ui->mapWidget->clear();

    // clear map context menu
    mapContextMenu.clear();

    // clear diagram
    ui->diagramWidget->clear();

    // clear point list
    ui->tableWidgetPoints->clear();
}

void GPXLab::updatePoint()
{
    if (gpxmw)
    {
        const GPX_wptType *wpt = gpxmw->getSelectedPoint();
        if (wpt)
        {
            QApplication::setOverrideCursor(Qt::WaitCursor);
            QApplication::processEvents();

            ui->statusBar->showMessage(tr("Updating point list..."));
            ui->mapWidget->selectPoint(wpt->latitude, wpt->longitude);
            ui->diagramWidget->setMarkerValue(wpt->timestamp);
            ui->tableWidgetPoints->selectRow(gpxmw->getSelectedPointNumber());

            ui->statusBar->showMessage("");
            QApplication::restoreOverrideCursor();
            return;
        }
    }
    ui->mapWidget->selectedPointSetVisible(false);
    ui->diagramWidget->setMarkerValue(0.0);
    ui->tableWidgetPoints->selectRow(0);
}

void GPXLab::build(bool select)
{
    if (gpxmw)
    {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents();

        // Map
        ui->statusBar->showMessage(tr("Generating map..."));
        ui->mapWidget->build(gpxmw);
        if (select)
            ui->mapWidget->selectTrack(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());

        // Map menu
        ui->statusBar->showMessage(tr("Generating map menu..."));
        int numTracks = gpxmw->getNumTracks();
        mapContextMenu.clear();
        mapContextMenu.setTitle(tr("Tracks"));
        for (int i = 0; i < numTracks; ++i)
        {
            QAction *action = new QAction(this);
            action->setText(gpxmw->getItemName(i));
            action->setData(i);
            action->setCheckable(true);
            action->setEnabled(true);
            if (select)
            {
                if (i == gpxmw->getSelectedTrackNumber())
                {
                    action->setChecked(true);
                    action->setEnabled(false);
                }
                else
                {
                    action->setChecked(false);
                    action->setEnabled(true);
                }
            }
            connect(action, SIGNAL(triggered()), this, SLOT(map_contextMenuSelectionChanged()));
            mapContextMenu.addAction(action);
        }

        // Track tree
        ui->statusBar->showMessage(tr("Generating track tree..."));
        ui->treeTracks->build(gpxmw);
        if (select)
            ui->treeTracks->select(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());

        ui->statusBar->showMessage("");
        QApplication::restoreOverrideCursor();
    }
}

void GPXLab::setTrackName(int trackNumber, const QString& name)
{
    ui->treeTracks->setTrackName(trackNumber, name);
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::map_selectionChanged(int trackNumber, int trackSegmentNumber, double lat, double lon)
{
    if (updatingLevel || closing)
        return;

    if (gpxmw)
    {
        SelectCommand selectCommand(this, trackNumber, trackSegmentNumber, lat, lon);
        selectCommand.redo();
    }
}

void GPXLab::map_contextMenuSelectionChanged()
{
    if (updatingLevel || closing)
        return;

    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        SelectCommand selectCommand(this, action->data().toInt());
        selectCommand.redo();
    }
}

void GPXLab::tree_selectionChanged()
{
    if (updatingLevel || closing)
        return;

    if (gpxmw)
    {
        QList<QTreeWidgetItem*> items = ui->treeTracks->selectedItems();
        if (items.count() > 0)
        {
            QList<QVariant> data = items[0]->data(0, Qt::UserRole).toList();
            SelectCommand selectCommand(this, data.at(0).toInt(), data.at(1).toInt());
            selectCommand.redo();
        }
    }
}

void GPXLab::table_selectionChanged(int pointNumber)
{
    if (updatingLevel || closing)
        return;

    if (gpxmw)
    {
        SelectCommand selectCommand(this, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber);
        selectCommand.redo();
    }
}

void GPXLab::diagram_selectionChanged(QMouseEvent* event)
{
    Q_UNUSED(event);

    if (updatingLevel || closing)
        return;

    if (gpxmw)
    {
        SelectCommand selectCommand(this, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), (time_t)ui->diagramWidget->getMarkerValue());
        selectCommand.redo();
    }
}

void GPXLab::map_viewChanged(const QPointF &coordinate, int zoom)
{
    Q_UNUSED(coordinate);
    zoomSlider->setMinimum(ui->mapWidget->minZoom());
    zoomSlider->setMaximum(ui->mapWidget->maxZoom());
    zoomSlider->setValue(zoom);
}

void GPXLab::zoom_valueChanged(int value)
{
    ui->mapWidget->setZoom(value);
}

//////////////////////////////////////////////////////////////////////////////

void GPXLab::setCurrentFile(const QString &fileName)
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    files.removeAll(fileName);
    files.prepend(fileName);
    while (files.size() > MaxRecentFiles)
        files.removeLast();
    settings.setValue("recentFileList", files);
    updateRecentFileActions();
}

void GPXLab::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value("recentFileList").toStringList();
    int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);
    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = QString::number(i + 1) + " " +  files[i];
        actionOpenRecentFile[i]->setText(text);
        actionOpenRecentFile[i]->setData(files[i]);
        actionOpenRecentFile[i]->setVisible(true);
    }
    for (int i = numRecentFiles; i < MaxRecentFiles; ++i)
        actionOpenRecentFile[i]->setVisible(false);
}

void GPXLab::map_showContextMenu(const QPoint& pos)
{
    if (gpxmw)
    {
        QMenu menu;
        QAction *separator = new QAction(this);
        separator->setSeparator(true);
        menu.addMenu(&mapContextMenu);
        menu.addAction(separator);
        menu.addAction(ui->actionFit_View);
        menu.addAction(ui->actionFit_Track);
        menu.addAction(ui->actionFit_Point);
        menu.addAction(separator);
        menu.addAction(ui->actionFollow_Item);
        menu.addAction(ui->actionShow_Only_Selected_Track);
        menu.addAction(separator);
        menu.addAction(ui->actionSplit_Track);
        menu.addAction(ui->actionCombine_Track);
        menu.exec(ui->mapWidget->mapToGlobal(pos));
    }
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
    if (gpxmw)
    {
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
    }
    return ret;
}

void GPXLab::undo_indexChanged(int idx)
{
    Q_UNUSED(idx);

    if (gpxmw)
    {
        // check if modified
        if (!undoStack->isClean())
        {
            // enable save action
            ui->actionSave_File->setEnabled(true);

            // set window title
            setWindowTitle("*" + gpxmw->getFileName() + " - " + appName);
        }
        else
        {
            // set window title
            setWindowTitle(gpxmw->getFileName() + " - " + appName);
        }
    }
    else
    {
        // set window title
        setWindowTitle(appName);
    }
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
        saveSettings();
    }
}
