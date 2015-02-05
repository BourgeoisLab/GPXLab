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

#include "qmaptrack.h"
#include "imagepoint.h"
#include "arrowpoint.h"
#include "gpxlab.h"

/////////////////////////////////////////////////////////////////////////////

QPixmapExt::QPixmapExt() :
    QPixmap(),
    dx(0),
    dy(0),
    isFromImg(false)
{
}

QPixmapExt::QPixmapExt(int width, int height, Point::Alignment alignment) :
    QPixmap(width, height),
    isFromImg(false)
{
    alignToTopLeft(alignment);
}

QPixmapExt::QPixmapExt(const QString &fileName, Point::Alignment alignment) :
    QPixmap(fileName),
    isFromImg(true)
{
    alignToTopLeft(alignment);
}

int QPixmapExt::getAlignmentX() const
{
    return dx;
}

int QPixmapExt::getAlignmentY() const
{
    return dy;
}

void QPixmapExt::alignToTopLeft(Point::Alignment alignment)
{
    switch (alignment)
    {
    case Point::Middle:
        dx = -width()/2;
        dy = -height()/2;
        break;

    case Point::TopLeft:
        dx = 0;
        dy = 0;
        break;

    case Point::TopRight:
        dx = -width();
        dy = 0;
        break;

    case Point::BottomLeft:
        dx = 0;
        dy = -height();
        break;

    case Point::BottomRight:
        dx = -width();
        dy = -height();
        break;

    case Point::BottomMiddle:
        dx = -width()/2;
        dy = -height();
        break;

    case Point::TopMiddle:
        dx = -width()/2;
        dy = 0;
        break;
    }
}

bool QPixmapExt::isFromImage() const
{
    return isFromImg;
}


/////////////////////////////////////////////////////////////////////////////

QMapTrack::QMapTrack(const GPX_wrapper *gpxmw, int trackNumber, const GPX_trkType *track) :
    Curve(),
    gpxmw(gpxmw),
    track(track),
    trackNumber(trackNumber),
    mouseOver(false)
{
    GeometryType = "QMapTrack";

    // set pen properties
    linePen = new QPen(QColor(0, 0, 255, 100));
    linePen->setWidth(5);
    linePenSelected = new QPen(QColor(255, 0, 0, 255));
    linePenSelected->setWidth(5);
    linePenSameTrack = new QPen(QColor(255, 0, 0, 150));
    linePenSameTrack->setWidth(5);

    linePenOver = new QPen(QColor(0, 200, 200, 100));
    linePenOver->setWidth(8);
    pointsMiddle = new QPen(QColor(100, 0, 0, 255));
    pointPenSelected = new QPen(GPXLab::appColor);
    pointPenSelected->setWidth(3);

    mypen = linePen;

    pointPixmap = new QPixmapExt[track->stats.points];
    QPixmapExt *pixmap = pointPixmap;

    int trackSegmentNumber = 0;
    int pointNumber = 0;

    int numPoints;
    const GPX_trksegType* trkseg;
    const GPX_wptType* trkpt;

    int numTrackSegments = gpxmw->initTrksegIteration(trackNumber);
    if (numTrackSegments  > 0)
    {
        while ((trkseg = gpxmw->getNextTrkseg()))
        {
            pointNumber = 0;
            numPoints = gpxmw->initTrkptIteration(trackNumber, trackSegmentNumber);
            if (numPoints > 0)
            {
                while ((trkpt = gpxmw->getNextTrkpt()))
                {
                    if (pointNumber == 0)
                    {
                        if (trackSegmentNumber == 0)
                        {
                            // first point of first segment
                            *pixmap++ = QPixmapExt(":/images/flag_green.png", Point::BottomLeft);
                        }
                        else
                        {
                            // first point of other segments
                            *pixmap++ = QPixmapExt(":/images/flag_blue.png", Point::BottomLeft);
                        }
                    }
                    else if (pointNumber == (numPoints - 1))
                    {
                        if (trackSegmentNumber == (numTrackSegments - 1))
                        {
                            // last point of last segment
                            *pixmap++ = QPixmapExt(":/images/flag_finish.png", Point::BottomLeft);
                        }
                        else
                        {
                            // last point of other segments
                            *pixmap++ = QPixmapExt(":/images/flag_red.png", Point::BottomLeft);
                        }
                    }
                    else
                    {
                        // middle points
                        *pixmap++ = drawArrow(6, -trkpt->heading, pointsMiddle);
                    }
                    ++pointNumber;
                }
            }
            ++trackSegmentNumber;
        }
    }
}

QMapTrack::~QMapTrack()
{
    delete[] pointPixmap;
}

QPixmapExt QMapTrack::drawArrow(int sideLength, qreal heading, QPen* pen)
{
    QPixmapExt pixmap = QPixmapExt(sideLength, sideLength);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);

//#if !defined Q_WS_MAEMO_5  //FIXME Maemo has a bug - it will antialias our point out of existence
    painter.setRenderHints(QPainter::Antialiasing|QPainter::HighQualityAntialiasing);
//#endif

    if(pen)
    {
        painter.setPen(*pen);
        painter.setBrush(QBrush(pen->color()));
    }
    else
    {
        painter.setBrush(QBrush(painter.pen().color()));
    }

    painter.setWindow(-(sideLength / 2), -(sideLength / 2), sideLength, sideLength);
    QTransform transform;
    transform.rotate(-heading);
    transform.scale(0.4, 0.75);
    painter.setWorldTransform(transform);

    QPolygon arrow;
    arrow << QPoint(0, -(sideLength / 2));
    arrow << QPoint(-(sideLength / 2), +(sideLength / 2));
    arrow << QPoint(0, 0);
    arrow << QPoint(+(sideLength / 2), +(sideLength / 2));

    painter.drawPolygon(arrow);

    return pixmap;
}

void QMapTrack::redrawPoint(int pointNumber)
{
    const GPX_wptType *trkpt = gpxmw->getPoint(trackNumber, -1, pointNumber);
    if (trkpt)
    {
        if (!pointPixmap[pointNumber].isFromImage())
            pointPixmap[pointNumber] = drawArrow(6, -trkpt->heading, pointsMiddle);
    }
}

int QMapTrack::getTrackNumber() const
{
    return trackNumber;
}

void QMapTrack::setTrackNumber(int number)
{
    trackNumber = number;
}

int QMapTrack::getTouchedSegmentNumber() const
{
    return touchedSegmentNumber;
}
const GPX_wptType *QMapTrack::getTouchedPoint() const
{
    return touchedPoint;
}

void QMapTrack::setMouseOver(bool over)
{
    mouseOver = over;
}

QRectF QMapTrack::boundingBox()
{
    QPointF topLeft = QPointF(track->stats.bounds.minlon, track->stats.bounds.minlat);
    QPointF bottomRight = QPointF(track->stats.bounds.maxlon, track->stats.bounds.maxlat);
    return QRectF(topLeft, bottomRight);
}

QList<Point*> QMapTrack::points()
{
    return QList<Point*>();
}

int QMapTrack::distance(QPoint &A, QPoint &B)
{
    QPoint d = A - B;
    return (int)sqrt(d.x()*d.x() + d.y()*d.y());
}

void QMapTrack::drawPoint(QPainter *painter, const MapAdapter *mapadapter, const QRect &viewport, const QPixmapExt *pixmap, double latitude, double longitude)
{
    if (pixmap)
    {
        QPoint point = mapadapter->coordinateToDisplay(QPointF(longitude, latitude));
        if (viewport.contains(point))
            painter->drawPixmap(point.x() + pixmap->getAlignmentX(), point.y() + pixmap->getAlignmentY(), pixmap->width(), pixmap->height(), *pixmap);
    }
}

void QMapTrack::draw(QPainter* painter, const MapAdapter* mapadapter, const QRect &screensize, const QPoint offset)
{
    Q_UNUSED(offset);

    const GPX_trksegType* trkseg;
    const GPX_wptType* trkpt;
    int trackSegmentNumber, pointNumber, numPoints;

    if (!visible)
        return;

    if (track->stats.points == 0)
        return;

    if (track->stats.points == 1)
    {
        if (trackNumber == gpxmw->getSelectedTrackNumber())
        {
            trkpt = gpxmw->getPoint(trackNumber, 0, 0);
            drawPoint(painter, mapadapter, screensize, &pointPixmap[0], trkpt->latitude, trkpt->longitude);
        }
        return;
    }

    // draw polygons
    if (gpxmw->initTrksegIteration(trackNumber))
    {
        trackSegmentNumber = 0;
        while ((trkseg = gpxmw->getNextTrkseg()))
        {            
            if (gpxmw->initTrkptIteration(trackNumber, trackSegmentNumber))
            {
                if (trackNumber == gpxmw->getSelectedTrackNumber())
                {
                    if (trackSegmentNumber == gpxmw->getSelectedTrackSegmentNumber() || gpxmw->getSelectedTrackSegmentNumber() == -1)
                        mypen = linePenSelected;
                    else
                        mypen = linePenSameTrack;
                }
                else
                {
                    if (mouseOver)
                        mypen = linePenOver;
                    else
                        mypen = linePen;
                }

                QPolygon p = QPolygon();
                while ((trkpt = gpxmw->getNextTrkpt()))
                    p.append(mapadapter->coordinateToDisplay(QPointF(trkpt->longitude, trkpt->latitude)));
                painter->setPen(*mypen);
                painter->drawPolyline(p);
            }
            ++trackSegmentNumber;
        }
    }

    // draw points
    if (trackNumber == gpxmw->getSelectedTrackNumber())
    {
        if (gpxmw->initTrksegIteration(trackNumber))
        {
            QPoint A, B;
            int dist = 0;

            trkpt = gpxmw->getPoint(trackNumber, 0, 0);
            A = mapadapter->coordinateToDisplay(QPointF(trkpt->longitude, trkpt->latitude));

            trackSegmentNumber = 0;
            QPixmapExt *pixmap = pointPixmap;
            while ((trkseg = gpxmw->getNextTrkseg()))
            {
                if (trackSegmentNumber == gpxmw->getSelectedTrackSegmentNumber() || gpxmw->getSelectedTrackSegmentNumber() == -1)
                {
                    pointNumber = 0;
                    numPoints = gpxmw->initTrkptIteration(trackNumber, trackSegmentNumber);
                    if (numPoints > 0)
                    {
                        while ((trkpt = gpxmw->getNextTrkpt()))
                        {
                            if (pointNumber == 0 || pointNumber == (numPoints - 1))
                            {
                                drawPoint(painter, mapadapter, screensize, pixmap, trkpt->latitude, trkpt->longitude);
                            }
                            else
                            {
                                // draw middle points
                                if (mapadapter->currentZoom() >= mapadapter->maxZoom() - 1)
                                {
                                    // draw all points if zoom is about maximum
                                    drawPoint(painter, mapadapter, screensize, pixmap, trkpt->latitude, trkpt->longitude);
                                }
                                else
                                {
                                    // draw points acording to distance inbetween
                                    B = A;
                                    A = mapadapter->coordinateToDisplay(QPointF(trkpt->longitude, trkpt->latitude));
                                    dist += distance(A, B);
                                    if (dist > 12)
                                    {
                                        dist  = 0;
                                        drawPoint(painter, mapadapter, screensize, pixmap, trkpt->latitude, trkpt->longitude);
                                    }
                                }
                            }
                            ++pixmap;
                            ++pointNumber;
                        }
                    }
                }
                else
                {
                    pixmap += trkseg->trkpt.size();
                }
                ++trackSegmentNumber;
            }
        }
    }
}

bool QMapTrack::Touches(Point* geom, const MapAdapter* mapadapter)
{
    const GPX_trksegType* trkseg;
    const GPX_wptType* trkpt;

    if (track->stats.points < 2)
    {
        return false;
    }

    QPointF clickPt = mapadapter->coordinateToDisplay(geom->coordinate());

    qreal halfwidth = 2; // use 2 pixels by default
    if (mypen && mypen->width() > 0)
    {
        halfwidth = static_cast<qreal> (mypen->width())/ static_cast<qreal> (2);
    }

    trkpt = gpxmw->getPoint(trackNumber, 0, 0);
    QPointF pt1 = mapadapter->coordinateToDisplay(QPointF(trkpt->longitude, trkpt->latitude));
    qreal pt1x1 = pt1.x() - halfwidth;
    qreal pt1x2 = pt1.x() + halfwidth;
    qreal pt1y1 = pt1.y() - halfwidth;
    qreal pt1y2 = pt1.y() + halfwidth;

    int numSegments = gpxmw->initTrksegIteration(trackNumber);
    if (numSegments > 0)
    {
        int trackSegmentNumber = 0;
        while ((trkseg = gpxmw->getNextTrkseg()))
        {
            if (gpxmw->initTrkptIteration(trackNumber, trackSegmentNumber))
            {
                while ((trkpt = gpxmw->getNextTrkpt()))
                {
                    QPointF pt2 = mapadapter->coordinateToDisplay(QPointF(trkpt->longitude, trkpt->latitude));
                    qreal pt2x1 = pt2.x() - halfwidth;
                    qreal pt2x2 = pt2.x() + halfwidth;
                    qreal pt2y1 = pt2.y() - halfwidth;
                    qreal pt2y2 = pt2.y() + halfwidth;

                    // build lazy bounding box
                    qreal upperLeftX = qMin(pt1x1, qMin(pt1x2, qMin(pt2x1, pt2x2)));
                    qreal upperLeftY = qMin(pt1y1, qMin(pt1y2, qMin(pt2y1, pt2y2)));
                    qreal lowerRightX = qMax(pt1x1, qMax(pt1x2, qMax(pt2x1, pt2x2)));
                    qreal lowerRightY = qMax(pt1y1, qMax(pt1y2, qMax(pt2y1, pt2y2)));

                    QRectF bounds(QPointF(upperLeftX, upperLeftY),
                                  QPointF(lowerRightX,lowerRightY));

                    if (bounds.contains(clickPt))
                    {
                        touchedSegmentNumber = (numSegments > 1) ? trackSegmentNumber : -1;
                        touchedPoint = trkpt;
                        return true;
                    }

                    pt1x1 = pt2x1;
                    pt1x2 = pt2x2;
                    pt1y1 = pt2y1;
                    pt1y2 = pt2y2;
                }
            }
            ++trackSegmentNumber;
        }
    }
    return false;
}
