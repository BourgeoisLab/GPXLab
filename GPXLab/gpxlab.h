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
 * The view is divided into seven different parts:
 * - Statistic information about all tracks
 * - Calendar for track selection
 * - Track tree for track selection
 * - Statistic information about the selected track or track segment
 * - Map showing all tracks
 * - Diagram of the selected track or track segment
 * - List of the track points of the selected track or track segment
 *
 * Except the map all other parts can be hid to maximize the map view.
 *
 * @section Features Features & Functions
 * - Support opening GPX (v1.0 or v1.1), NMEA and SpoQ files
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
 * Currently GPX, NMEA and SpoQ files are supported. If you have a file
 * in another format you may use GPSBabel (http://www.gpsbabel.org) to 
 * convert it into a GPX file.
 *
 * @section License License
 *
 * GPXLab is free software and licensed under GPL version 3. 
 *
 * @section Compiling Compiling
 *
 * The source code was developed under Qt 5.3.2 / 5.3.3. There is no guarantee
 * that the code will compile with previous or even later versions.
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
 * <b>[v0.3.0]</b>
 * - [update] Updated QMapControl to version 0.9.7.8.
 * - [update] Updated QCustomPlot to version 1.3.0.
 * - [update] Updated copyright to "2014 - 2015".
 * - [new] Track on the map is highlighted when the mouse cursor is moved over it.
 * - [new] Show some point properties when the mouse cursor is over the point.
 * - [new] Added functionality to insert/delete track points.
 * - [new] Added functionality to set the start time of a track.
 * - [new] Added functionality to move a single track point using SHIFT + mouse click.
 * - [new] Added a calendar to select a track by date.
 * - [new] Diagram curves can be changed to show other properties.
 * - [new] Support SpoQ files (.act and .xml).
 * - [new] Support Garmin's TrackPointExtension heart rate extension.
 * - [fix] Ignore upper/lower case to detect file type.
 * - [fix] Draw tracks  with 0 or only 1 point correctly.
 * - [fix] Changed way of iteration in GPX_wrapper.
 * - [fix] Fixed bug in NMEA sentence parser.
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
 * @version 1.2
 * @date 5 Feb 2015
 */
class GPXLab : public QMainWindow
{
    Q_OBJECT

public:

    /** @brief Organization name */
    static const QString organisationName;

    /** @brief Application name */
    static const QString appName;

    /** @brief Application version */
    static const QString appVersion;

    /** @brief Application main color */
    static const QColor appColor;

public:

    /**
     * @brief Constructs the main application
     * @param fileName If not empty the file is parsed and appended to the GPX_model
     * @param parent Parent
     */
    explicit GPXLab(const QString &fileName = "", QWidget *parent = 0);
    ~GPXLab();

    /**
     * @brief Opens a file and creates a new GPX_model
     * @param fileName File to open, if empty an open file dialogue is shown
     * @return True on success
     */
    bool openFile(QString fileName = "");

    /**
     * @brief Appends files to the GPX_model
     * @param fileNames Files to append, if empty an open files dialogue is shown
     */
    void appendFiles(QStringList &fileNames);

    /**
     * @brief Saves the current GPX_model to a file
     * @param fileName File name, if empty an save file dialogue is shown
     */
    void saveFile(QString fileName = "");

    /**
     * @brief Closes the current file
     */
    void closeFile();

private slots:

    /**
     * @brief Called when a file is successfully loaded
     */
    void fileLoaded();

    /**
     * @brief Called when a file is successfully saved
     */
    void fileSaved();

    /**
     * @brief Called when the GPX_model is cleared
     */
    void modelCleared();

    /**
     * @brief Called when properties of the GPX_model changed
     */
    void modelPropertiesChanged();

    /**
     * @brief Called when the metadata of the GPX_model changed
     */
    void modelMetadataChanged();

    /**
     * @brief Called when the metadata of a track changed
     * @param trackNumber Track number
     */
    void trackMetadataChanged(int trackNumber);

    /**
     * @brief Called when a track was inserted
     * @param trackNumber Track Number
     * @param track Track
     */
    void trackInserted(int trackNumber, const GPX_trkType &track);

    /**
     * @brief Called when a track was deleted
     * @param trackNumber Track number
     */
    void trackDeleted(int trackNumber);

    /**
     * @brief Called when a track was moved up
     * @param trackNumber Track number
     */
    void trackMovedUp(int trackNumber);

    /**
     * @brief Called when a track was moved down
     * @param trackNumber Track number
     */
    void trackMovedDown(int trackNumber);

    /**
     * @brief Called when a track was splited
     * @param trackNumber Track number
     */
    void trackSplited(int trackNumber);

    /**
     * @brief Called when two track segments were combined
     * @param trackNumber Track number
     */
    void trackCombined(int trackNumber);

    /**
     * @brief Signal when a track was time shifted
     * @param trackNumber Track number
     * @param seconds Amounts of seconds shifted
     */
    void trackTimeShifted(int trackNumber, long seconds);

    /**
     * @brief Called when a new track was selected
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number, -1 for no point selected
     * @param wpt Point
     */
    void trackSelectionChanged(int trackNumber, int trackSegmentNumber, int pointNumber, const GPX_wptType *trkpt);

    /**
     * @brief Called when a point was edited
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param properties Properties changed (bit field)
     */
    void pointEdited(int trackNumber, int trackSegmentNumber, int pointNumber, GPX_wrapper::TrackPointProperty properties);

    /**
     * @brief Called when a point was inserted
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param trkpt Point
     */
    void pointInserted(int trackNumber, int trackSegmentNumber, int pointNumber, const GPX_wptType& trkpt);

    /**
     * @brief Called when a point was deleted
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     */
    void pointDeleted(int trackNumber, int trackSegmentNumber, int pointNumber);

    /**
     * @brief Called when a new point was selected
     * @param pointNumber Point number
     * @param trkpt Point
     */
    void pointSelectionChanged(int pointNumber, const GPX_wptType *trkpt);

    /**
     * @brief Sets the main window title
     */
    void setMainWindowTitle();

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

    void map_selectionChanged(int pointNumber);

    void map_viewChanged(const QPointF &coordinate, int zoom);

    void cal_selectionChanged(int trackNumber);

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

    void mapmenu_selectionChanged();

    void on_actionSplit_Track_triggered();

    void on_actionCombine_Track_triggered();

    void on_actionRestore_Default_View_triggered();

    void on_actionSettings_triggered();

    void on_actionDelete_Point_triggered();

    void on_actionSetStartTime_triggered();

    void on_actionInsert_Point_triggered();

private:

    void updateActions(bool enabled);
    void updateRecentFiles();
    void updateFileProperties(bool clear);
    void updateTrackProperties(bool clear);
    int check_need_save();
    void update_size();
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent* event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void getOffsets(int &left, int &right, int &top, int &bottom);
    bool eventFilter(QObject *obj, QEvent *event);

private:

    /** @brief UI */
    Ui::GPXLab* ui;

    /** @brief GPX_model wrapper */
    GPX_wrapper* gpxmw;

    /** @brief Application settings */
    Settings* settings;

    /** @brief Undo stack */
    QUndoStack *undoStack;

    /** @brief Closing flag to prevent unnecessary stuff */
    bool closing;

    /** @brief Open recent file actions */
    QAction **actionOpenRecentFile;

    /** @brief Slider to change zoom of map */
    QSlider *zoomSlider;
};

/** @} GPXLab */

#endif // _GPXLAB_H_
