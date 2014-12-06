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

/**
 * @mainpage GPXLab Documentation
 *
 * @section WhatIs What is GPXLab?
 *
 * GPXLab is an application to display and manage GPS tracks previously 
 * recorded with a GPS tracker.
 *
 * The idea is to give the user the possibility to combine several tracks into
 * one single file and have statistic information about the summarized
 * tracks or about a single track. Furthermore the user can see all tracks on 
 * a map and select there a track and/or a single track point. An altitude
 * and a speed diagram and also a list of the track points offers visual
 * information about the selected item.
 *
 * @image html demo.png
 *
 * The view is divided into 6 different parts:
 * - Statistic information about all tracks
 * - Track and track segment selection
 * - Statistic information about the selected track or track segment
 * - Map showing all tracks
 * - Altitude and speed diagram of the selected track or track segment
 * - List of the track points of the selected track or track segment
 *
 * Except the map all other parts can be hid to maximize the map view.
 *
 * @section Features Features & Functions
 * - Support opening GPX (v1.0 or v1.1) and NMEA files
 * - Combine several tracks into one single GPX file
 * - Rearrange the tracks (move/add/delete)
 * - Modify the meta data of the GPX file and of any tracks inside the file
 * - Get altitude data from the SRTM database
 * - Show statistic information about the summarized tracks and about a single track
 * - Show a map of all tracks
 * - Show an altitude and a speed diagram
 * - Show a list of the track points
 * 
 * @section FileFormat Supported File Formats
 * Currently only GPX and NMEA files are supported. If you have a file
 * in another format you may use GPSBabel (http://www.gpsbabel.org) to 
 * convert it into a GPX file.
 *
 * @section License License
 *
 * This software is free software and licensed under GPL version 3. 
 *
 * @section Compiling Compiling
 *
 * The source code was developed under Qt 5.3.2. There is no guarantee that
 * the code will compile with previous or even later versions.
 *
 * The project is separated in two sub-projects GPXLab and QMapControl.
 * QMapControl is a widget to display a map. Since I had to made some
 * modifications to the original code I added the modified version here.
 * QMapControl is compiled as a library and linked to the main sub-project
 * GPXLab.
 *
 * Both sub-projects are ready to build, although it was only tested under
 * Windows 7 (64bit).
 * 
 * @section Credits Credits
 * - <b> QCustomPlot Qt Widget:</b> http://www.qcustomplot.com
 * - <b> QMapControl Qt Widget:</b> http://www.medieninf.de/qmapcontrol
 * - <b> Nuvola Icons:</b> http://www.icon-king.com/projects/nuvola
 * - <b> FactCow Icons:</b> http://www.fatcow.com
 * 
 * @section ReleaseNotes Release Notes
 *
 * <b>[v0.2.0 Beta]</b>
 * - [update] Updated QMapControl to version 0.9.7.6.
 * - [update] Changed some icons.
 * - [update] Some minor GUI changes.
 * - [new] Double clicking an item in the track selection opens the track properties dialogue.
 * - [new] Added undo/redo functionality.
 * - [new] Added functionality to split and combine a track.
 * - [new] Window state and position is stored on application closing and restored on restart.
 * - [new] Header state of point list is stored on application closing and restored on restart.
 * - [new] Added control to change the map zoom.
 * - [new] Added option to configure the map caching.
 * - [fix] Docks don't disappear any more when tabbed.
 *
 * <b>[v0.1.0 Beta]</b>
 * - [new] Initial beta release.
 *
 * @section Info Information
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @see Project homepage: http://bourgeoislab.wordpress.com/gpxlab
 * @see Latest release: http://github.com/bourgeoislab/gpxlab/releases
 * @see Latest sources: http://github.com/bourgeoislab/gpxlab
 */

#ifndef _GPXLAB_H_
#define _GPXLAB_H_

#include <QMainWindow>
#include <QMenu>
#include <QUndoStack>
#include <QTreeWidgetItem>
#include "settings.h"
#include "gpx_wrapper.h"

/**
 * @defgroup GPXLab GPXLab
 * @brief GPXLab
 * @{
 */

namespace Ui {
class GPXLab;
}

/**
 * @class GPXLab
 *
 * @brief Main application class
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 6 Dec 2014
 */
class GPXLab : public QMainWindow
{
    Q_OBJECT

public:

    /** Organization name */
    static const QString organisationName;

    /** Application name */
    static const QString appName;

    /** Application version */
    static const QString appVersion;

    /**
     * @brief Constructs the main application
     * @param fileName If not empty the file is parsed and appended to the GPX_model
     * @param parent Parent
     */
    explicit GPXLab(const QString &fileName = "", QWidget *parent = 0);
    ~GPXLab();

    /**
     * @brief Opens a file and creates a new GPX_model
     * @param fileName File to open.
     * If fileName is empty an open file dialogue is shown.
     */
    void openFile(QString fileName = "");

    /**
     * @brief Appends files to the GPX_model
     * @param fileNames Files to append.
     * If the list is empty an open files dialogue is shown.
     */
    void appendFiles(QStringList &fileNames);

    /**
     * @brief Saves the current GPX_model to a file
     * @param fileName File name.
     * If fileName is empty an save file dialogue is shown.
     */
    void saveFile(QString fileName = "");

    /**
     * @brief Closes the current file
     */
    void closeFile();

    /**
     * @brief Begins an update of widgets containing track properties
     * Call this function prior to any updateTrack(), updatePoint() or build()
     * to avoid doing update stuff several times
     * @note Call endUpdate() when finished
     */
    void beginUpdate();

    /**
     * @brief Ends an update of widgets containing track properties
     * @see beginUpdate()
     */
    void endUpdate();

    /**
     * @brief Updates all widgets showing file properties
     */
    void updateFile();

    /**
     * @brief Updates all widgets showing track properties
     */
    void updateTrack();

    /**
     * @brief Updates all widgets showing point properties
     */
    void updatePoint();

    /**
     * @brief Generates the content of all widgets showing track properties
     * @param select If true selects the current track
     */
    void build(bool select = false);

    /**
     * @brief Sets a track name
     * @param trackNumber Track number
     * @param name New track name
     */
    void setTrackName(int trackNumber, const QString& name);

    /**
     * @brief GPX_model wrapper
     * @note Used in the command classes
     */
    GPX_wrapper* gpxmw;

private slots:
    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_actionAppend_File_triggered();

    void tree_selectionChanged();

    void tree_doubleClicked(QTreeWidgetItem* item, int column);

    void on_actionEdit_File_Properties_triggered();

    void on_actionEdit_Track_Properties_triggered();

    void on_actionSave_File_As_triggered();

    void on_actionSave_File_triggered();

    void on_dockWidgetFile_visibilityChanged(bool visible);

    void on_dockWidgetTracks_visibilityChanged(bool visible);

    void on_dockWidgetDiagrams_visibilityChanged(bool visible);

    void on_dockWidgetPoints_visibilityChanged(bool visible);

    void on_mainToolBar_visibilityChanged(bool visible);

    void diagram_selectionChanged(QMouseEvent* event);

    void table_selectionChanged(int pointNumber);

    void map_selectionChanged(int trackNumber, int trackSegmentNumber, double lat, double lon);

    void map_viewChanged(const QPointF &coordinate, int zoom);

    void zoom_valueChanged(int value);

    void on_actionFit_View_triggered();

    void on_actionFit_Track_triggered();

    void on_actionFit_Point_triggered();

    void on_actionFollow_Item_toggled(bool follow);

    void on_actionShow_Only_Selected_Track_toggled(bool checked);

    void on_actionGetAltitudeFromSRTM_triggered();

    void on_pushButtonTrackAppend_clicked();

    void on_pushButtonTrackRemove_clicked();

    void on_pushButtonTrackMoveDown_clicked();

    void on_pushButtonTrackMoveUp_clicked();

    void on_actionAbout_triggered();

    void openRecentFile();

    void on_actionClose_File_triggered();

    void map_showContextMenu(const QPoint& pos);

    void map_contextMenuSelectionChanged();

    void undo_indexChanged(int idx);

    void on_actionSplit_Track_triggered();

    void on_actionCombine_Track_triggered();

    void on_actionRestore_Default_View_triggered();

    void on_actionSettings_triggered();

private:

    /**
     * @brief Maximal number of undo commands stored
     * @see undoStack
     */
    static const int undoLimit = 10;

    /**
     * @brief Maximal number of recent files shown in the menu
     */
    static const int MaxRecentFiles = 5;

    /**
     * @brief UI
     */
    Ui::GPXLab* ui;

    /**
     * @brief Application settings
     */
    Settings settings;

    /**
     * @brief Updating flag to prevent update-loops
     */
    int updatingLevel;

    /**
     * @brief Closing flag to prevent unnecessary stuff
     * Set on closeEvent().
     */
    bool closing;

    /**
     * @brief Open recent file actions
     */
    QAction *actionOpenRecentFile[MaxRecentFiles];

    /**
     * @brief Map context menu for track selection
     */
    QMenu mapContextMenu;

    /**
     * @brief Undo stack
     */
    QUndoStack *undoStack;

    /**
     * @brief Menu action to undo
     */
    QAction *undoAction;

    /**
     * @brief Menu action to redo
     */
    QAction *redoAction;

    QSlider *zoomSlider;

    QByteArray defaultState;
    QByteArray defaultGeometry;

    void loadSettings();
    void saveSettings();
    int check_need_save();
    void update_size();
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent* event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void getOffsets(int &left, int &right, int &top, int &bottom);
    bool eventFilter(QObject *obj, QEvent *event);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
};

/** @} GPXLab */

#endif // _GPXLAB_H_
