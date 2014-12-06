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
 
#include "combinetrackcommand.h"

CombineTrackCommand::CombineTrackCommand(GPXLab *gpxlab, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber)
{
}

void CombineTrackCommand::undo()
{
    // split track segments
    if (gpxlab->gpxmw->splitTrack(trackNumber, trackSegmentNumber, pointNumber) == GPX_model::GPXM_OK)
    {
        // begin update of track widgets
        gpxlab->beginUpdate();

        // rebuild tracks
        gpxlab->build(true);

        // update track properties
        gpxlab->updateTrack();
        
        // update track point properties
        gpxlab->updatePoint();
        
        // end update
        gpxlab->endUpdate();
    }
}

void CombineTrackCommand::redo()
{
    // combine track segments
    if (gpxlab->gpxmw->combineTrack(trackNumber, trackSegmentNumber, pointNumber) == GPX_model::GPXM_OK)
    {
        // begin update of track widgets
        gpxlab->beginUpdate();

        // rebuild tracks
        gpxlab->build(true);

        // update track properties
        gpxlab->updateTrack();
        
        // update track point properties
        gpxlab->updatePoint();
        
        // end update
        gpxlab->endUpdate();
    }
}
