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
 
#include "appendtrackcommand.h"

AppendTrackCommand::AppendTrackCommand(GPXLab *gpxlab, const QString &fileName, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
    fileName(fileName)
{
}

void AppendTrackCommand::undo()
{
    // remove tracks
    for (int i = gpxlab->gpxmw->getNumTracks() - 1; i >= numTracksBeforeAppend; --i)
        gpxlab->gpxmw->removeTrack(i);

    // update file properties
    gpxlab->updateFile();

    // begin update of track widgets
    gpxlab->beginUpdate();

    // rebuild map and tree and select track
    gpxlab->build(true);

    // end update
    gpxlab->endUpdate();
}

void AppendTrackCommand::redo()
{
    // save number of tracks before appending
    numTracksBeforeAppend = gpxlab->gpxmw->getNumTracks();

    // append tracks
    if (gpxlab->gpxmw->load(fileName, false) == GPX_model::GPXM_OK)
    {
        // update file properties
        gpxlab->updateFile();

        // begin update of widgets
        gpxlab->beginUpdate();

        // rebuild map and tree
        gpxlab->build();

        // end update
        gpxlab->endUpdate();
    }
}
