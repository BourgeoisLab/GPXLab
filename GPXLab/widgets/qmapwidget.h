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

#ifndef _QMAPWIDGET_H_
#define _QMAPWIDGET_H_

#include "mapcontrol.h"
#include "gpx_wrapper.h"

using namespace qmapcontrol;

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
 * @version 1.0
 * @date 7 Nov 2014
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
     * @brief Generates the map accorind to the GPX_Model
     * @param gpxmw GPX_model wrapper
     */
    void build(const GPX_wrapper *gpxmw);

    /**
     * @brief Clears the map
     */
    void clear();

    /**
     * @brief Selects a track
     * @param trackNumber Track number
     * @param trackSegmentNumber Track number segment, if -1 select all segments
     */
    void selectTrack(int trackNumber, int trackSegmentNumber = -1);

    /**
     * @brief Selects a point given latitude/longitude coordinates
     * @param lat Latitude
     * @param lon Longitude
     */
    void selectPoint(double lat, double lon);

    /**
     * @brief Sets the visibility of the selected point
     * @param visible If true shows the selected point
     */
    void selectedPointSetVisible(bool visible);

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

signals:

    /**
     * @brief Signal when the selection changed
     * @param trackNumber New selected track number
     * @param trackSegmentNumber New selected track segment number
     * @param lat Latitude of the selected point
     * @param lon Longitude of the selected point
     */
    void selectionChanged(int trackNumber, int trackSegmentNumber, double lat, double lon);

private slots:
    void geometryClicked(Geometry* geometry, QPoint point);

private:

    void selectPoint(Point &point);

    QPen* linePen;
    QPen* linePenSelected;
    QPen* linePenSameTrack;
    QPen* pointPenSelected;
    QPen* pointsMiddle;
    //QPen* pointsStart;
    //QPen* pointsEnd;
    QPen* pointsIntersection;
    //QPen* pointsEndSegment;
    Layer* trackLayer;
    Point* selectedPoint;
    Geometry* selectedTrack;
    bool followSelection;
    bool showOnlySelectedTrack;
};

#endif // QMAPWIDGET_H
