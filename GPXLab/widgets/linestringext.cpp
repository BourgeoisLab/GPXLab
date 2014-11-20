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
 
#include "linestringext.h"

LineStringExt::LineStringExt(int trackNumber, int trackSegmentNumber, QList<Point*> const points, QString name, QPen* pen) :
    LineString(points, name, pen),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    selected(false)
{
    GeometryType = "LineStringExt";
}

int LineStringExt::getTrackNumber() const
{
    return trackNumber;
}

int LineStringExt::getTrackSegmentNumber() const
{
    return trackSegmentNumber;
}

void LineStringExt::setSelected(bool selected)
{
    this->selected = selected;
}

int LineStringExt::distance(QPoint &A, QPoint &B)
{
    QPoint d = A - B;
    return (int)sqrt(d.x()*d.x() + d.y()*d.y());
}

void LineStringExt::draw(QPainter* painter, const MapAdapter* mapadapter, const QRect &screensize, const QPoint offset)
{
    if (!visible)
        return;

    if (mypen != 0)
    {
        painter->save();
        painter->setPen(*mypen);
    }

    if (selected)
    {
        QPolygon p = QPolygon();
        for (int i=0; i<childPoints.size(); i++)
            p.append(mapadapter->coordinateToDisplay(childPoints[i]->coordinate()));
        painter->drawPolyline(p);
    }
    else
    {
        QPolygon p = QPolygon();
        for (int i=0; i<childPoints.size(); i++)
            p.append(mapadapter->coordinateToDisplay(childPoints[i]->coordinate()));
        painter->drawPolyline(p);
    }

    if (mypen != 0)
    {
        painter->restore();
    }

    if (selected)
    {
        int i = 0;
        int dist = 0;
        QPoint A, B;

        // draw first point
        childPoints[i]->draw(painter, mapadapter, screensize, offset);
        A = mapadapter->coordinateToDisplay(childPoints[i]->coordinate());

        for (i = 1; i < childPoints.size() - 1; i++)
        {
            // draw middle points
            if (mapadapter->currentZoom() >= mapadapter->maxZoom() - 1)
            {
                // draw all points if zoom is about maximum
                childPoints[i]->draw(painter, mapadapter, screensize, offset);
            }
            else
            {
                // draw points acording to distance inbetween
                B = A;
                A = mapadapter->coordinateToDisplay(childPoints[i]->coordinate());
                dist += distance(A, B);
                if (dist > 10)
                {
                    dist  = 0;
                    childPoints[i]->draw(painter, mapadapter, screensize, offset);
                }
            }
        }

        // draw last point
        childPoints[i]->draw(painter, mapadapter, screensize, offset);
    }
}
