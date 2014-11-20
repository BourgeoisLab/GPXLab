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

#include "qutils.h"

QString QUtils::seconds_to_DHMS(unsigned int seconds, unsigned int milliseconds)
{
    QString str;
    int sec = seconds % 60;
    seconds /= 60;
    int min = seconds % 60;
    seconds /= 60;
    int hours = seconds % 24;
    int days = seconds / 24;
    if(hours == 0 && days == 0)
        str.sprintf("%02d:%02d", min, sec);
    else if (days == 0)
        str.sprintf("%02d:%02d:%02d", hours, min, sec);
    else
        str.sprintf("%dd%02d:%02d:%02d", days, hours, min, sec);
    if (milliseconds > 0)
        str += "." + QString("%1").arg(milliseconds, 3, 10, QChar('0'));
    return str;
}

void QUtils::movingAverage(QVector<double> &values, int order)
{
    int idx = 0;
    double total = 0.0;
    double *window = new double[order];
    for (int n = 0; n < values.size(); ++n)
    {
        if (n >= order)
            total -= window[idx];
        total += values[n];

        window[idx++] = values[n];
        if (idx == order)
            idx = 0;

        if (n >= order)
            values[n] = total / order;
        else
            values[n] = total / (n + 1);
    }
    delete window;
}
