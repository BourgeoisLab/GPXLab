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
 
#include "setaltitudecommand.h"

SetAltitudeCommand::SetAltitudeCommand(GPXLab *gpxlab, int trackNumber, const QVector<double> &values, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxlab(gpxlab),
    trackNumber(trackNumber),
    values(values)
{
}

void SetAltitudeCommand::undo()
{
    redo();
}

void SetAltitudeCommand::redo()
{
    QVector<double> tmpValues;

    // generate values
    gpxlab->gpxmw->generateDiagramValues(trackNumber);

    // store the values temporarily
    tmpValues = gpxlab->gpxmw->getDiagramAltitudeValues();

    // set new altitude values
    if (gpxlab->gpxmw->setAltitudeValues(values, trackNumber) == GPX_model::GPXM_OK)
    {
        // update file properties
        gpxlab->updateFile();

        // begin update of track widgets
        gpxlab->beginUpdate();

        // update track properties
        gpxlab->updateTrack();

        // end update
        gpxlab->endUpdate();
    }

    // store values
    values = tmpValues;
}
