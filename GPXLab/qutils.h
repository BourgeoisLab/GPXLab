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

#ifndef _QUTILS_H_
#define _QUTILS_H_

#include <QString>
#include <QVector>

/**
 * @ingroup GPXLab
 * @{
 */

/**
 * @namespace QUtils
 *
 * @brief Utility functions.
 *
 * Utility functions.
 * These functions may use Qt stuff.
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 25 Oct 2014
 */
namespace QUtils
{
    /**
     * @brief Converts an amount of seconds in a human readable string.
     * @param seconds Amount of seconds
     * @param milliseconds Amount of milliseconds, default 0
     * @return String in format "m:s[.mmm]", "h:m:s[.mmm]" or "ddh:m:s[.mmm]"
     */
    QString seconds_to_DHMS(unsigned int seconds, unsigned int milliseconds = 0);


    /**
     * @brief Averages the values with a moving average algorithm
     * @param values Values to average
     * @param order Order of moving average, default = 3
     */
    void movingAverage(QVector<double> &values, int order = 3);
}

/** @} GPXLab */

#endif // _QUTILS_H_
