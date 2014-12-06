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
 
#include "removetrackcommand.h"

RemoveTrackCommand::RemoveTrackCommand(GPXLab *gpxlab, int trackNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
    trackNumber(trackNumber),
    trk(trackNumber)
{
    trk = *gpxlab->gpxmw->getTrack(trackNumber);
}

void RemoveTrackCommand::undo()
{
    if (gpxlab->gpxmw->insertTrack(trackNumber, trk) == GPX_model::GPXM_OK)
    {
        // update file properties
        gpxlab->updateFile();

        // begin update of track widgets
        gpxlab->beginUpdate();

        // rebuild tracks
        gpxlab->build(true);

        // end update
        gpxlab->endUpdate();
    }
}

void RemoveTrackCommand::redo()
{
    if (gpxlab->gpxmw->removeTrack(trackNumber) == GPX_model::GPXM_OK)
    {
        // update file properties
        gpxlab->updateFile();

        // begin update of track widgets
        gpxlab->beginUpdate();

        // rebuild tracks
        gpxlab->build(true);

        // end update
        gpxlab->endUpdate();
    }
}
