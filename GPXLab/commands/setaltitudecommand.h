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
 
#ifndef SETALTITUDECOMMAND_H
#define SETALTITUDECOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class SetAltitudeCommand
 *
 * @brief Set altitude values command
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 4 Jan 2015
 */
class SetAltitudeCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Set altitude values to this track number
     * @param values Altitude values
     * @param parent Parent
     */
    SetAltitudeCommand(GPX_wrapper *gpxmw, int trackNumber, const QVector<double> &values, QUndoCommand *parent = 0);

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
    QVector<double> values;
};

/** @} Commands */

#endif // SETALTITUDECOMMAND_H
