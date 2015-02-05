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
 
#include "splittrackcommand.h"

SplitTrackCommand::SplitTrackCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber)
{
}

void SplitTrackCommand::undo()
{
    // combine track segments
    gpxmw->combineTrack(trackNumber, trackSegmentNumber, pointNumber);
}

void SplitTrackCommand::redo()
{
    // split track segments
    gpxmw->splitTrack(trackNumber, trackSegmentNumber, pointNumber);
}
