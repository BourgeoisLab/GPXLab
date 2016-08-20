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
#include "qutils.h"
#include "osmmapadapter.h"
#include "maplayer.h"
#include "geometrylayer.h"
#include "point.h"
#include "qmaptrack.h"
#include "invisiblepoint.h"
#include "circlepoint.h"
#include "qmapwidget.h"
#include "pointeditcommand.h"
#include "pointinsertcommand.h"
#include "pointdeletecommand.h"

QMapWidget::QMapWidget(QWidget *parent, Qt::WindowFlags windowFlags) :
    MapControl(QSize(200,200), MapControl::PanningNoZoom, true, false, parent, windowFlags),
    selectedTrack(-1),
    followSelection(false),
    showOnlySelectedTrack(false)
{
    // create OpenStreetMap adapter
    MapAdapter* OSMAdapter = new OSMMapAdapter();

    // create a layer for the tracks
    trackLayer = new MapLayer("Tracks", OSMAdapter, true);
    addLayer(trackLayer);
    trackLayer->setVisible(false);

    // connect mouse events of the tracks layer
    connect(trackLayer, SIGNAL(geometryClicked(Geometry*, QPoint)), this, SLOT(geometryClicked(Geometry*, QPoint)));
    connect(trackLayer, SIGNAL(geometryOver(Geometry*, QPoint)), this, SLOT(geometryOver(Geometry*, QPoint)));
    connect(this, SIGNAL(mouseEventCoordinate(const QMouseEvent*, const QPointF)), this, SLOT(mouseEventCoordinate(const QMouseEvent*, const QPointF)));

    // create a layer for the geometries on the "top"
    Layer *topLayer = new GeometryLayer("Top", OSMAdapter, false);
    addLayer(topLayer);
    topLayer->setVisible(true);

    // add selected point to "top" layer
    pointPenSelected = new QPen(GPXLab::appColor);
    pointPenSelected->setWidth(3);
    selectedPoint = new CirclePoint(0.0, 0.0, 10, "", Point::Middle, pointPenSelected);
    selectedPoint->setVisible(false);
    topLayer->addGeometry(selectedPoint);

    // add over point to "top" layer
    overPointWidget = new QLabel(this);
    QPalette sample_palette;
    QFont font( "Arial", 10, QFont::Bold);
    overPointWidget->setFont(font);
    sample_palette.setColor(QPalette::Window, QColor(255,255,255,200));
    sample_palette.setColor(QPalette::WindowText, Qt::blue);
    overPointWidget->setAutoFillBackground(true);
    overPointWidget->setPalette(sample_palette);
    overPointWidget->setText("");
    overPointWidget->setMouseTracking(true);
    overPoint = new Point(0.0, 0.0, overPointWidget, "", Point::BottomLeft);
    overPoint->setVisible(false);
    topLayer->addGeometry(overPoint);

    // disable map at start
    setEnabled(false);
}

void QMapWidget::init(GPX_wrapper *gpxmw, QUndoStack *undoStack, bool doPersistentCaching, QString &cachePath)
{
    this->gpxmw = gpxmw;
    this->undoStack = undoStack;

    // enable persitent cache of tiles
    if (doPersistentCaching)
        enablePersistentCache(QDir(cachePath));
}

void QMapWidget::build(bool zoomIn)
{
    const GPX_trkType *trk;

    suspendUpdate(true);

    // clear if necessary
    clear();

    // enable and set layers visible
    setEnabled(true);
    trackLayer->setVisible(true);

    // add new tracks
    if (gpxmw->initTrkIteration() > 0)
    {
        int trackNumber = 0;
        while ((trk = gpxmw->getNextTrk()))
            trackLayer->addGeometry(new QMapTrack(gpxmw, trackNumber++, trk));
    }

    if (zoomIn)
    {
        const GPX_metadataType *metadata = gpxmw->getModelMetadata();
        setViewAndZoomIn(metadata->bounds);
    }

    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::clear()
{
    setEnabled(false);
    trackLayer->setVisible(false);
    trackLayer->clearGeometries(true);
    selectedTrack = -1;
    selectedPointSetVisible(false);
}

void QMapWidget::keyPressEvent(QKeyEvent *event)
{
    int numPoints = gpxmw->getNumPoints(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
    int pointNumber = gpxmw->getSelectedPointNumber();
    switch (event->key())
    {
    case Qt::Key_Left:
    case Qt::Key_Up:
        if (pointNumber == -1)
        {
            emit selectionChanged(numPoints - 1);
        }
        else if (pointNumber > 0)
        {
            if (event->modifiers() & Qt::ControlModifier)
            {
                int newPointNumber = pointNumber - 10;
                if (newPointNumber < 0)
                    newPointNumber = 0;
                emit selectionChanged(newPointNumber);
            }
            else
            {
                emit selectionChanged(pointNumber - 1);
            }
        }
        break;

    case Qt::Key_Right:
    case Qt::Key_Down:
        if (pointNumber == -1)
        {
            emit selectionChanged(0);
        }
        else if (pointNumber < numPoints - 1)
        {
            if (event->modifiers() & Qt::ControlModifier)
            {
                int newPointNumber = pointNumber + 10;
                if (newPointNumber > numPoints - 1)
                    newPointNumber = numPoints - 1;
                emit selectionChanged(newPointNumber);
            }
            else
            {
                emit selectionChanged(pointNumber + 1);
            }
        }
        break;

    case Qt::Key_Insert:
        if (pointNumber < numPoints - 1)
        {
            GPX_wptType wpt;
            const GPX_wptType *wptA = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber);
            const GPX_wptType *wptB = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber + 1);
            wpt.altitude = wptA->altitude + (wptB->altitude - wptA->altitude)/2;
            wpt.latitude = wptA->latitude + (wptB->latitude - wptA->latitude)/2;
            wpt.longitude = wptA->longitude + (wptB->longitude - wptA->longitude)/2;
            wpt.timestamp = wptA->timestamp + (wptB->timestamp - wptA->timestamp)/2;
            undoStack->push(new PointInsertCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber + 1, wpt));
        }
        break;

    case Qt::Key_Delete:
        if (pointNumber >= 0)
            undoStack->push(new PointDeleteCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), pointNumber));
        break;
    }
}

void QMapWidget::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    geometryOver(NULL, QPoint());
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

void QMapWidget::selectTrack(int trackNumber)
{
    QMapTrack* track;
    QList<Geometry*> tracks = trackLayer->getGeometries(); // copy list
    QList<QPointF> view = QList<QPointF>();

    // suspend redraw
    suspendUpdate(true);

    for(QList<Geometry*>::iterator iter = tracks.begin(); iter != tracks.end(); ++iter)
    {
        track = dynamic_cast<QMapTrack*>(*iter);
        if (track)
        {
            if (track->getTrackNumber() == trackNumber)
            {
                // send selected track to front (= back of the list)
                trackLayer->sendGeometryToBack(*iter);
                track->setVisible(true);
                selectedTrack = track->getTrackNumber();

                // bounding box for followSelection
                if (followSelection)
                {
                    QRectF box = track->boundingBox();
                    view.append(box.topLeft());
                    view.append(box.bottomRight());
                }
            }
            else
            {
                // other track number
                if (showOnlySelectedTrack)
                    track->setVisible(false);
                else
                    track->setVisible(true);
            }
        }
    }

    // set view and zoom if followSelection
    if (followSelection && view.size() > 0)
        MapControl::setViewAndZoomIn(view);

    // redraw
    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::selectedPointSetVisible(bool visible, double lat, double lon)
{
    if (visible)
        selectedPoint->setCoordinate(QPointF(lon, lat));
    selectedPoint->setVisible(visible);
}

void QMapWidget::overPointSetVisible(bool visible, const GPX_wptType *trkpt)
{
    if (visible)
    {
            QString text = QUtils::seconds_to_DHMS(trkpt->elapsedTime);
            text += "\n" + QString::number(trkpt->distanceTot, 'f', 1) + " km";
            overPointWidget->setText(text);
            overPoint->setCoordinate(QPointF(trkpt->longitude, trkpt->latitude));
            overPoint->setVisible(true);
            return;
    }
    else
    {
        overPoint->setVisible(false);
    }
}

void QMapWidget::geometryClicked(Geometry* geometry, QPoint point)
{
    static QPoint lastPoint(0,0);

    QMapTrack* track = dynamic_cast<QMapTrack*>(geometry);
    if (track)
    {
        if (track->getTrackNumber() == selectedTrack && lastPoint == point)
            return;

        if (selectedTrack != -1 && track->getTrackNumber() != selectedTrack)
        {
            // send last selected track and track segments to back (= front of the list)
            QListIterator<Geometry*> iter(trackLayer->getGeometries());
            iter.toBack();
            while (iter.hasPrevious())
            {
                QMapTrack* trackLoop = dynamic_cast<QMapTrack*>(iter.previous());
                if (trackLoop)
                {
                    if (trackLoop->getTrackNumber() == selectedTrack)
                    {
                        trackLayer->sendGeometryToFront(trackLoop);
                        break;
                    }
                }
            }
        }

        // emit selection changed
        const GPX_wptType *trkpt = track->getTouchedPoint();
        emit(selectionChanged(track->getTrackNumber(), track->getTouchedSegmentNumber(), trkpt->latitude, trkpt->longitude));
    }
    lastPoint = point;
    trackLayer->consumeMouseEvent();
}

void QMapWidget::geometryOver(Geometry* geometry, QPoint point)
{
    Q_UNUSED(point);

    QMapTrack* trackOver = dynamic_cast<QMapTrack*>(geometry);

    // suspend redraw
    suspendUpdate(true);

    // highlight track
    QList<Geometry*> tracks = trackLayer->getGeometries();
    for(QList<Geometry*>::iterator iter = tracks.begin(); iter != tracks.end(); ++iter)
    {
        QMapTrack* track = dynamic_cast<QMapTrack*>(*iter);
        if (track)
        {
            if (track->getTrackNumber() != selectedTrack)
            {
                if (trackOver && track->getTrackNumber() == trackOver->getTrackNumber())
                    track->setMouseOver(true);
                else
                    track->setMouseOver(false);
            }
        }
    }

    // highlight point of selected track
    if (trackOver && trackOver->getTrackNumber() == selectedTrack)
        overPointSetVisible(true, trackOver->getTouchedPoint());
    else
        overPointSetVisible(false);

    // redraw
    suspendUpdate(false);
    updateRequestNew();

    trackLayer->consumeMouseEvent();
}

void QMapWidget::insertTrack(int trackNumber, const GPX_trkType &track)
{
    // suspend redraw
    suspendUpdate(true);

    // change track numbers
    QList<Geometry*> &tracks = trackLayer->getGeometries();
    for(QList<Geometry*>::iterator itTrack = tracks.begin(); itTrack != tracks.end(); ++itTrack)
    {
        QMapTrack* track = dynamic_cast<QMapTrack*>(*itTrack);
        if (track)
        {
            if (track->getTrackNumber() >= trackNumber)
                track->setTrackNumber(track->getTrackNumber() + 1);
        }
    }

    // insert new track
    trackLayer->addGeometry(new QMapTrack(gpxmw, trackNumber, &track));

    // redraw
    suspendUpdate(false);
    updateRequestNew();

}

void QMapWidget::deleteTrack(int trackNumber)
{
    Geometry* trackToDelete = NULL;

    // suspend redraw
    suspendUpdate(true);

    // change track numbers
    QList<Geometry*> &tracks = trackLayer->getGeometries();
    for(QList<Geometry*>::iterator itTrack = tracks.begin(); itTrack != tracks.end(); ++itTrack)
    {
        QMapTrack* track = dynamic_cast<QMapTrack*>(*itTrack);
        if (track)
        {
            if (track->getTrackNumber() == trackNumber)
                trackToDelete = *itTrack;
            if (track->getTrackNumber() > trackNumber)
                track->setTrackNumber(track->getTrackNumber() - 1);
        }
    }

    // erase track
    if (trackToDelete)
        trackLayer->removeGeometry(trackToDelete);

    // redraw
    suspendUpdate(false);
    updateRequestNew();
}

void QMapWidget::moveTrackUp(int trackNumber)
{
    // change track numbers
    QList<Geometry*> &tracks = trackLayer->getGeometries();
    for(QList<Geometry*>::iterator itTrack = tracks.begin(); itTrack != tracks.end(); ++itTrack)
    {
        QMapTrack* track = dynamic_cast<QMapTrack*>(*itTrack);
        if (track)
        {
            if (track->getTrackNumber() == trackNumber)
                track->setTrackNumber(trackNumber - 1);
            else if (track->getTrackNumber() == trackNumber - 1)
                track->setTrackNumber(trackNumber);
        }
    }
}

void QMapWidget::moveTrackDown(int trackNumber)
{
    // change track numbers
    QList<Geometry*> &tracks = trackLayer->getGeometries();
    for(QList<Geometry*>::iterator itTrack = tracks.begin(); itTrack != tracks.end(); ++itTrack)
    {
        QMapTrack* track = dynamic_cast<QMapTrack*>(*itTrack);
        if (track)
        {
            if (track->getTrackNumber() == trackNumber)
                track->setTrackNumber(trackNumber + 1);
            else if (track->getTrackNumber() == trackNumber + 1)
                track->setTrackNumber(trackNumber);
        }
    }
}

void QMapWidget::editPoint(int trackNumber, int pointNumber)
{
    QList<Geometry*> &tracks = trackLayer->getGeometries();
    for(QList<Geometry*>::iterator itTrack = tracks.begin(); itTrack != tracks.end(); ++itTrack)
    {
        QMapTrack* track = dynamic_cast<QMapTrack*>(*itTrack);
        if (track)
        {
            if (track->getTrackNumber() == trackNumber)
            {
                track->redrawPoint(pointNumber - 1);
                track->redrawPoint(pointNumber);
                break;
            }
        }
    }
}

void QMapWidget::mouseEventCoordinate(const QMouseEvent* evnt, const QPointF coordinate)
{
    if (isMousePressed() && gpxmw->getSelectedPointNumber() >= 0 &&
        evnt->button() == 1 && evnt->modifiers() & Qt::ShiftModifier)
    {
        GPX_wrapper::TrackPointProperty properties[3] = {GPX_wrapper::latitude, GPX_wrapper::longitude, GPX_wrapper::timestamp};
        double values[3];

        // set new coordinates
        values[0] = coordinate.y();
        values[1] = coordinate.x();

        // interpolate new timestamp
        if (gpxmw->getSelectedPointNumber() > 0 &&
            gpxmw->getSelectedPointNumber() < gpxmw->getNumPoints(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber()) - 1)
        {
            const GPX_wptType *wptA = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber() - 1);
            const GPX_wptType *wptB = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber() + 1);

            double dLat, dLon;
            dLat = coordinate.y() - wptA->latitude;
            dLon = coordinate.x() - wptA->longitude;
            double dAX = sqrt(dLat * dLat + dLon * dLon);
            dLat = wptB->latitude - coordinate.y();
            dLon = wptB->longitude - coordinate.x();
            double dXB = sqrt(dLat * dLat + dLon * dLon);

            values[2] = (wptB->getTime() - wptA->getTime())* dAX / (dAX + dXB) + wptA->getTime();

            undoStack->push(new PointEditCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber(),
                                                 3, properties, values));
        }
        else
        {
            undoStack->push(new PointEditCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), gpxmw->getSelectedPointNumber(),
                                                 2, properties, values));
        }
        selectedPointSetVisible(true, coordinate.y(), coordinate.x());
    }
}
