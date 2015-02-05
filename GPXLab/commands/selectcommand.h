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
 
#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class SelectCommand
 *
 * @brief Command to select a track, track segment and/or track point
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 4 Jan 2015
 */
class SelectCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 whole track
     * @param pointNumber Point number, if -1 no point will be selected
     * @param parent Parent
     */
    SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber = -1, int pointNumber = -1, QUndoCommand *parent = 0);

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 whole track
     * @param lat Latitude coordinate of the point to select
     * @param lon Longitude coordinate of the point to select
     * @param parent Parent
     */
    SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, double lat, double lon, QUndoCommand *parent = 0);

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 whole track
     * @param timestamp Timestamp of the point to select
     * @param parent Parent
     */
    SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, time_t timestamp, QUndoCommand *parent = 0);

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
    int trackNumber;
    int trackSegmentNumber;
    int pointNumber;
    double lat;
    double lon;
    time_t timestamp;
};

/** @} Commands */

#endif // SELECTCOMMAND_H
