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
 
#ifndef POINTEDITCOMMAND_H
#define POINTEDITCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class PointEditCommand
 *
 * @brief Edit point property command
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.2
 * @date 9 Jul 2016
 */
class PointEditCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param numProperties Number of properties
     * @param property Property to edit
     * @param value Value
     * @param parent Parent
     */
    PointEditCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, GPX_wrapper::TrackPointProperty property[], QString value[], QUndoCommand *parent = 0);

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param numProperties Number of properties
     * @param property Property to edit
     * @param value Value
     * @param parent Parent
     */
    PointEditCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, GPX_wrapper::TrackPointProperty property[], double value[], QUndoCommand *parent = 0);

    ~PointEditCommand();

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
    size_t numProperties;
    GPX_wrapper::TrackPointProperty *property;
    QString *sValue;
    double *dValue;
};

/** @} Commands */

#endif // POINTEDITCOMMAND_H
