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

#ifndef APPENDTRACKCOMMAND_H
#define APPENDTRACKCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class AppendTrackCommand
 *
 * @brief Append track command
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 4 Jan 2015
 */
class AppendTrackCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param fileName File containing tracks to append
     * @param fileType File type, GPXM_FILE_AUTOMATIC for automatic detection
     * @param parent Parent
     */
    AppendTrackCommand(GPX_wrapper *gpxmw, const QString &fileName, GPX_model::fileType_e fileType, QUndoCommand *parent = 0);

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
    const QString fileName;
    int numTracksBeforeAppend;
    GPX_model::fileType_e fileType;
};

/** @} Commands */

#endif // APPENDTRACKCOMMAND_H
