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
 
#include "appendtrackcommand.h"

AppendTrackCommand::AppendTrackCommand(GPX_wrapper *gpxmw, const QString &fileName, GPX_model::fileType_e fileType, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    fileName(fileName),
    fileType(fileType)
{
}

void AppendTrackCommand::undo()
{
    // remove tracks
    for (int i = gpxmw->getNumTracks() - 1; i >= numTracksBeforeAppend; --i)
        gpxmw->removeTrack(i);
}

void AppendTrackCommand::redo()
{
    // save number of tracks before appending
    numTracksBeforeAppend = gpxmw->getNumTracks();

    // append tracks
    gpxmw->load(fileName, fileType, false);
}
