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
 
#include "editfilepropertiescommand.h"

EditFilePropertiesCommand::EditFilePropertiesCommand(GPX_wrapper *gpxmw, GPX_metadataType &metadata, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    metadata(metadata)
{
}

void EditFilePropertiesCommand::undo()
{
    redo();
}

void EditFilePropertiesCommand::redo()
{
    // temporary copy old values
    GPX_metadataType tmpMetadata = *gpxmw->getModelMetadata();

    // set new values
    gpxmw->setModelMetadata(metadata);

    // store old values
    metadata = tmpMetadata;
}
