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
 
#include "selectcommand.h"

SelectCommand::SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber),
    lat(0.0),
    lon(0.0),
    timestamp(0)
{
}

SelectCommand::SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, double lat, double lon, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(-2),
    lat(lat),
    lon(lon),
    timestamp(0)
{
}

SelectCommand::SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, time_t timestamp, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(-2),
    lat(0.0),
    lon(0.0),
    timestamp(timestamp)
{
}

void SelectCommand::undo()
{
    redo();
}

void SelectCommand::redo()
{
    // temporary copy old values
    int tmpTrackNumber = gpxmw->getSelectedTrackNumber();
    int tmpTrackSegmentNumber = gpxmw->getSelectedTrackSegmentNumber();
    int tmpPointNumber = gpxmw->getSelectedPointNumber();

    // set new values
    if (pointNumber != -2)
        gpxmw->select(trackNumber, trackSegmentNumber, pointNumber);
    else if (timestamp != 0)
        gpxmw->select(trackNumber, trackSegmentNumber, timestamp);
    else
        gpxmw->select(trackNumber, trackSegmentNumber, lat, lon);

    // store old values
    trackNumber = tmpTrackNumber;
    trackSegmentNumber = tmpTrackSegmentNumber;
    pointNumber = tmpPointNumber;
}
