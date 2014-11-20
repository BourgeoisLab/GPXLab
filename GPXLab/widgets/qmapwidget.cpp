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
 
#include "osmmapadapter.h"
#include "maplayer.h"
#include "geometrylayer.h"
#include "point.h"
#include "linestringext.h"
#include "arrowpoint.h"
#include "invisiblepoint.h"
#include "circlepoint.h"
#include "imagepoint.h"
#include "qmapwidget.h"

QMapWidget::QMapWidget(QWidget *parent, Qt::WindowFlags windowFlags) :
    MapControl(QSize(200,200), MapControl::PanningNoZoom, true, false, parent, windowFlags),
    selectedPoint(NULL),
    selectedTrack(NULL),
    followSelection(false),
    showOnlySelectedTrack(false)
{
    // create OpenStreetMap adapter
    MapAdapter* OSMAdapter = new OSMMapAdapter();

    // create a layer for the map
    trackLayer = new MapLayer("Map", OSMAdapter);
    addLayer(trackLayer);
    trackLayer->setVisible(false);

    // enable persitent cache of tiles
    enablePersistentCache();

    // set pen properties
    linePen = new QPen(QColor(0, 0, 255, 100));
    linePen->setWidth(5);
    linePenSelected = new QPen(QColor(255, 0, 0, 255));
    linePenSelected->setWidth(5);
    linePenSameTrack = new QPen(QColor(255, 0, 0, 150));
    linePenSameTrack->setWidth(5);
    pointsMiddle = new QPen(QColor(127, 0, 0, 255));
    pointsIntersection = new QPen(QColor(0, 0, 0, 255));
    pointPenSelected = new QPen(QColor(3, 201, 48, 255));
    pointPenSelected->setWidth(3);

    // connect click events of the layer
    connect(trackLayer, SIGNAL(geometryClicked(Geometry*, QPoint)), this, SLOT(geometryClicked(Geometry*, QPoint)));

    // disable map at start
    setEnabled(false);
}

void QMapWidget::build(const GPX_wrapper *gpxmw)
{
    const GPX_metadataType *metadata = gpxmw->getModelMetadata();

    suspendUpdate(true);

    // enable and set layers visible
    setEnabled(true);
    trackLayer->setVisible(true);

    // delete old tracks
    trackLayer->clearGeometries(true);

    // add new tracks
    if (gpxmw->initTrkIteration() > 0)
    {
        int trackNumber = 0;
        int trackSegmentNumber = 0;
        int pointNumber = 0;
        int numTrackSegments;
        int numPoints;
        const GPX_trkType* trk;
        const GPX_trksegType* trkseg;
        const GPX_wptType* wpt;
        Point *point;
        QList<Point*> points;
        while ((trk = gpxmw->getNextTrk()))
        {
            trackSegmentNumber = 0;
            numTrackSegments = gpxmw->initTrksegIteration(trackNumber);
            if (numTrackSegments  > 0)
            {
                while ((trkseg = gpxmw->getNextTrkseg(trackNumber)))
                {
                    pointNumber = 0;
                    numPoints = gpxmw->initWptIteration(trackNumber, trackSegmentNumber);
                    if (numPoints > 0)
                    {
                        points.clear();
                        while ((wpt = gpxmw->getNextWpt(trackNumber, trackSegmentNumber)))
                        {
                            if (pointNumber == 0)
                            {
                                if (trackSegmentNumber == 0)
                                {
                                    // first point of first segment
                                    point = new ImagePoint(wpt->longitude, wpt->latitude, ":/images/ledgreen.png", QString::fromStdString(wpt->name), Point::Middle);
                                }
                                else
                                {
                                    // first point of other segments
                                    point = new CirclePoint(wpt->longitude, wpt->latitude, 8, QString::fromStdString(wpt->name), Point::Middle, pointsIntersection);
                                }
                            }
                            else if (pointNumber == (numPoints - 1))
                            {
                                if (trackSegmentNumber == (numTrackSegments - 1))
                                {
                                    // last point of last segment
                                    point = new ImagePoint(wpt->longitude, wpt->latitude, ":/images/ledred.png", QString::fromStdString(wpt->name), Point::Middle);
                                }
                                else
                                {
                                    // last point of other segments
                                    point = new CirclePoint(wpt->longitude, wpt->latitude, 8, QString::fromStdString(wpt->name), Point::Middle, pointsIntersection);
                                }
                            }
                            else
                            {
                                // middle points
                                point = new ArrowPoint(wpt->longitude, wpt->latitude, 10, -wpt->heading, QString::fromStdString(wpt->name), Point::Middle, pointsMiddle);
                            }
                            points.append(point);
                            ++pointNumber;
                        }
                        trackLayer->addGeometry(new LineStringExt(trackNumber, numTrackSegments == 1 ? -1 : trackSegmentNumber, points, gpxmw->getItemName(trackNumber, trackSegmentNumber), linePen));
                    }
                    ++trackSegmentNumber;
                }
            }
            ++trackNumber;
        }
    }

    // add selected point
    selectedPoint = new CirclePoint(0.0, 0.0, 10, "", Point::Middle, pointPenSelected);
    selectedPoint->setCoordinate(QPointF(0.0,0.0));
    selectedPoint->setVisible(false);
    trackLayer->addGeometry(selectedPoint);

    // sets view area
    setViewAndZoomIn(metadata->bounds);

    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::clear()
{
    setEnabled(false);
    trackLayer->setVisible(false);
    trackLayer->clearGeometries(true);
}

void QMapWidget::setViewAndZoomIn(const GPX_boundsType &bounds)
{
    suspendUpdate(true);
    QList<QPointF> view;
    view.append(QPointF(bounds.minlon, bounds.minlat));
    view.append(QPointF(bounds.maxlon, bounds.maxlat));
    MapControl::setViewAndZoomIn(view);
    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::setViewAndZoomInSelectedPoint()
{
    if (selectedPoint->isVisible())
    {
        suspendUpdate(true);
        setView(selectedPoint);
        setZoom(14);
        suspendUpdate(false);
        updateRequestNew();
    }
}

void QMapWidget::setFollowSelection(bool follow)
{
    followSelection = follow;
    if (followSelection)
    {
        if (selectedPoint->isVisible())
            setView(selectedPoint);
        followGeometry(selectedPoint);
    }
    else
    {
        stopFollowing(selectedPoint);
    }
}

void QMapWidget::setShowOnlySelectedTrack(bool only)
{
    showOnlySelectedTrack = only;
}

void QMapWidget::selectTrack(int trackNumber, int trackSegmentNumber)
{
    QList<QPointF> view;
    LineStringExt* track;
    QList<Geometry*> tracks = trackLayer->getGeometries();

    // suspend redraw
    suspendUpdate(true);

    for(QList<Geometry*>::iterator iter = tracks.begin(); iter != tracks.end(); ++iter)
    {
        track = dynamic_cast<LineStringExt*>(*iter);
        if (track)
        {
            if (track->getTrackNumber() == trackNumber)
            {
                if (trackSegmentNumber == track->getTrackSegmentNumber() || trackSegmentNumber == -1)
                {
                    // selected track
                    track->setSelected(true);
                    track->setPen(linePenSelected);

                    // send new selected track to front (= back of the list)
                    trackLayer->sendGeometryToBack(*iter);
                    track->setVisible(true);

                    // bounding box for followSelection
                    if (followSelection)
                    {
                        view.append(track->boundingBox().topLeft());
                        view.append(track->boundingBox().bottomRight());
                    }
                }
                else
                {
                    // friend ( same track number)
                    track->setSelected(false);
                    track->setPen(linePenSameTrack);

                    // send friend of new selected track to front (= back of the list)
                    trackLayer->sendGeometryToBack(*iter);
                    track->setVisible(true);
                }
            }
            else
            {
                // other track number
                track->setSelected(false);
                track->setPen(linePen);
                if (showOnlySelectedTrack)
                    track->setVisible(false);
                else
                    track->setVisible(true);
            }
        }
    }

    // hide selected point and send to front (= back of the list)
    selectedPoint->setVisible(false);
    trackLayer->sendGeometryToBack(selectedPoint);

    // set view and zoom if followSelection
    if (followSelection)
        MapControl::setViewAndZoomIn(view);

    // redraw
    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::selectPoint(double lat, double lon)
{
    suspendUpdate(true);
    selectedPoint->setVisible(true);
    selectedPoint->setCoordinate(QPointF(lon, lat));
    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::selectPoint(Point &point)
{
    suspendUpdate(true);
    selectedPoint->setVisible(true);
    selectedPoint->setCoordinate(point.coordinate());
    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::selectedPointSetVisible(bool visible)
{
    selectedPoint->setVisible(visible);
}

void QMapWidget::geometryClicked(Geometry* geometry, QPoint point)
{
    static QPoint lastPoint(0,0);

    if (geometry == selectedPoint)
        return;

    if (geometry == selectedTrack && lastPoint == point)
        return;

    LineStringExt* track = dynamic_cast<LineStringExt*>(geometry);
    if (track)
    {
        if (geometry->hasClickedPoints())
        {
            QList<Geometry*> points = geometry->clickedPoints();
            if (points.size() > 0)
            {
                Point *p = dynamic_cast<Point*>(points[0]);
                if (p)
                {
                    suspendUpdate(true);

                    // send last selected track to back (= front of the list)
                    trackLayer->sendGeometryToFront(selectedTrack);

                    // select track and point
                    selectTrack(track->getTrackNumber(), track->getTrackSegmentNumber());
                    selectPoint(*p);

                    suspendUpdate(false);
                    updateRequestNew();

                    emit(selectionChanged(track->getTrackNumber(), track->getTrackSegmentNumber(), p->coordinate().y(), p->coordinate().x()));
                }
            }
        }
    }
    selectedTrack = geometry;
    lastPoint = point;
    trackLayer->consumeMouseEvent();
}
