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
 
#include "movetrackupcommand.h"

MoveTrackUpCommand::MoveTrackUpCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber)
{
}

void MoveTrackUpCommand::undo()
{
    // move track down
    gpxmw->moveTrackDown(trackNumber);
}

void MoveTrackUpCommand::redo()
{
    // move track up
    gpxmw->moveTrackUp(trackNumber);
}
