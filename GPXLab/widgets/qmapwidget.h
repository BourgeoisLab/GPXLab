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

#ifndef _QMAPWIDGET_H_
#define _QMAPWIDGET_H_

#include "mapcontrol.h"
#include "gpx_wrapper.h"
#include <QLabel>
#include <QUndoStack>

using namespace qmapcontrol;

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QMapWidget
 *
 * @brief MapControl subclass
 *
 * Extends the MapControl class.
 *
 * @see MapControl
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.2
 * @date 4 Jan 2015
 */
class QMapWidget : public MapControl
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     * @param windowFlags Window flags
     */
    explicit QMapWidget(QWidget *parent = 0, Qt::WindowFlags windowFlags = 0);

    /**
     * @brief Initializes the widget
     * @note Call this function prior to any other function call
     * @param gpxmw GPX_model wrapper
     * @param undoStack Undo stack
     * @param doPersistentCaching Enable / disable persistent caching
     * @param cachePath Path to the cache directory
     * @param tilesURL Tiles URL
     */
    void init(GPX_wrapper *gpxmw, QUndoStack *undoStack, bool doPersistentCaching, QString &cachePath, QString &tilesURL);

    /**
     * @brief Generates the map according to the GPX_Model
     * @param zoomIn If true zooms in
     */
    void build(bool zoomIn);

    /**
     * @brief Clears the map
     */
    void clear();

    /**
     * @brief Selects a track
     * @param trackNumber Track number
     */
    void selectTrack(int trackNumber);

    /**
     * @brief Sets the visibility of the selected point
     * @param visible If true shows the selected point
     * @param lat Latitude
     * @param lon Longitude
     */
    void selectedPointSetVisible(bool visible, double lat = 0.0, double lon = 0.0);

    /**
     * @brief Sets the view and zoom in
     * @param bounds Bound rectangle
     */
    void setViewAndZoomIn(const GPX_boundsType &bounds);

    /**
     * @brief Sets the view to the selected point
     */
    void setViewAndZoomInSelectedPoint();

    /**
     * @brief Sets option to follow selection
     * @param follow Option
     */
    void setFollowSelection(bool follow);

    /**
     * @brief Sets option to show only selected track
     * @param only Option
     */
    void setShowOnlySelectedTrack(bool only);

    /**
     * @brief Inserts a track
     * @param trackNumber Track Number
     * @param track Track
     */
    void insertTrack(int trackNumber, const GPX_trkType &track);

    /**
     * @brief Deletes a track
     * @param trackNumber Track number
     */
    void deleteTrack(int trackNumber);

    /**
     * @brief Moves a track up
     * @param trackNumber Track number
     */
    void moveTrackUp(int trackNumber);

    /**
     * @brief Moves a track down
     * @param trackNumber Track number
     */
    void moveTrackDown(int trackNumber);

    /**
     * @brief Edits a point
     * @param trackNumber Track number
     * @param pointNumber Point number
     */
    void editPoint(int trackNumber, int pointNumber);

signals:

    /**
     * @brief Signal when the selection changed
     * @param trackNumber New selected track number
     * @param trackSegmentNumber New selected track segment number
     * @param lat Latitude of the selected point
     * @param lon Longitude of the selected point
     */
    void selectionChanged(int trackNumber, int trackSegmentNumber, double lat, double lon);

    /**
     * @brief Signal when the point selection changed
     * @param pointNumber New point number
     */
    void selectionChanged(int pointNumber);

private slots:

    /**
     * @brief Called when a geometry was clicked
     * @param geometry Geometry clicked
     * @param point Mouse coordinates
     */
    void geometryClicked(Geometry* geometry, QPoint point);

    /**
     * @brief Called on mouse move events
     * @param geometry Geometry on the mouse coordinates, NULL if none
     * @param point Mouse coordinates
     */
    void geometryOver(Geometry* geometry, QPoint point);

    /**
     * @brief Called on a mouse button event
     * @param evnt Mouse event
     * @param coordinate World coordinates
     */
    void mouseEventCoordinate(const QMouseEvent* evnt, const QPointF coordinate);

    /**
     * @brief Slot when settings are loaded or saved
     */
    void settingsChanged(bool loaded);

private:

    /**
     * @brief Sets the visibility of the over point
     * @param visible If true shows the over point
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     */
    void overPointSetVisible(bool visible, const GPX_wptType *trkpt = NULL);

    void keyPressEvent(QKeyEvent *event);

    void leaveEvent(QEvent *event);

    void setTilesURL(const QString &tilesURL);

private:
    GPX_wrapper *gpxmw;
    QUndoStack *undoStack;
    QPen* pointPenSelected;
    Layer* trackLayer;
    Point* selectedPoint;
    Point* overPoint;
    QLabel* overPointWidget;
    int selectedTrack;
    bool followSelection;
    bool showOnlySelectedTrack;
};

/** @} Widgets */

#endif // QMAPWIDGET_H
