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
 
#include "selectcommand.h"

SelectCommand::SelectCommand(GPXLab *gpxlab, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber),
    lat(0.0),
    lon(0.0),
    timestamp(0)
{
}

SelectCommand::SelectCommand(GPXLab *gpxlab, int trackNumber, int trackSegmentNumber, double lat, double lon, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(-2),
    lat(lat),
    lon(lon),
    timestamp(0)
{
}

SelectCommand::SelectCommand(GPXLab *gpxlab, int trackNumber, int trackSegmentNumber, time_t timestamp, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
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
    int tmpTrackNumber = gpxlab->gpxmw->getSelectedTrackNumber();
    int tmpTrackSegmentNumber = gpxlab->gpxmw->getSelectedTrackSegmentNumber();
    int tmpPointNumber = gpxlab->gpxmw->getSelectedPointNumber();

    // begin update of track widgets
    gpxlab->beginUpdate();

    // select track
    if (gpxlab->gpxmw->setSelectedTrack(trackNumber, trackSegmentNumber) == GPX_model::GPXM_OK)
    {
        gpxlab->updateTrack();
    }

    // select point
    if (pointNumber != -2)
    {
        if (gpxlab->gpxmw->setSelectedPointByNumber(pointNumber) == GPX_model::GPXM_OK)
        {
            gpxlab->updatePoint();
        }
    }
    else if (timestamp != 0)
    {
        if (gpxlab->gpxmw->setSelectedPointByTimestamp(timestamp) == GPX_model::GPXM_OK)
        {
            gpxlab->updatePoint();
        }
    }
    else
    {
        if (gpxlab->gpxmw->setSelectedPointByCoord(lat, lon) == GPX_model::GPXM_OK)
        {
            gpxlab->updatePoint();
        }
    }

    // end update
    gpxlab->endUpdate();

    trackNumber = tmpTrackNumber;
    trackSegmentNumber = tmpTrackSegmentNumber;
    pointNumber = tmpPointNumber;
}

void SelectCommand::undoWithoutUpdate()
{
    redoWithoutUpdate();
}

void SelectCommand::redoWithoutUpdate()
{
    int tmpTrackNumber = gpxlab->gpxmw->getSelectedTrackNumber();
    int tmpTrackSegmentNumber = gpxlab->gpxmw->getSelectedTrackSegmentNumber();
    int tmpPointNumber = gpxlab->gpxmw->getSelectedPointNumber();

    // select track
    gpxlab->gpxmw->setSelectedTrack(trackNumber, trackSegmentNumber);

    // select point
    if (pointNumber != -2)
    {
        gpxlab->gpxmw->setSelectedPointByNumber(pointNumber);
    }
    else if (timestamp != 0)
    {
        gpxlab->gpxmw->setSelectedPointByTimestamp(timestamp);
    }
    else
    {
        gpxlab->gpxmw->setSelectedPointByCoord(lat, lon);
    }

    trackNumber = tmpTrackNumber;
    trackSegmentNumber = tmpTrackSegmentNumber;
    pointNumber = tmpPointNumber;
}
