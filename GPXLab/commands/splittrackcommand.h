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
 
#ifndef SPLITTRACKCOMMAND_H
#define SPLITTRACKCOMMAND_H

#include <QUndoCommand>
#include "gpxlab.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class SplitTrackCommand
 *
 * @brief Split track command
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 28 Nov 2014
 */
class SplitTrackCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxlab Pointer to application
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 for whole track
     * @param pointNumber Point number at which the track is combined
     * @param parent Parent
     */
    SplitTrackCommand(GPXLab *gpxlab, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent = 0);

    /**
     * @brief Undo the command
     */
    void undo();

    /**
     * @brief Redo the command
     */
    void redo();

private:
    GPXLab *gpxlab;
    int trackNumber;
    int trackSegmentNumber;
    int pointNumber;
};

/** @} Commands */

#endif // SPLITTRACKCOMMAND_H
