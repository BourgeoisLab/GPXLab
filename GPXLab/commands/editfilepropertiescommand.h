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
 
#ifndef EDITFILEPROPERTIESCOMMAND_H
#define EDITFILEPROPERTIESCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class EditFilePropertiesCommand
 *
 * @brief Edit file properties command
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 4 Jan 2015
 */
class EditFilePropertiesCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param metadata Metadata of file
     * @param parent Parent
     */
    EditFilePropertiesCommand(GPX_wrapper *gpxmw, GPX_metadataType &metadata, QUndoCommand *parent = 0);

    /**
     * @brief Undo the command
     */
    void undo();

    /**
     * @brief Redo the command
     */
    void redo();

private:

    GPX_wrapper *gpxmw;
    GPX_metadataType metadata;
};

/** @} Commands */

#endif // EDITFILEPROPERTIESCOMMAND_H
