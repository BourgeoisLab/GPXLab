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

#ifndef _LINESTRINGEXT_H_
#define _LINESTRINGEXT_H_

#include "linestring.h"

using namespace qmapcontrol;

/**
 * @class LineStringExt
 *
 * @brief LineString subclass
 *
 * Extends the LineString class by adding two private members trackNumber
 * and trackSegmentNumber.
 *
 * @see LineString
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 7 Nov 2014
 */
class LineStringExt : public LineString
{
    Q_OBJECT
public:

    /**
     * @brief Constructor
     * @param trackNumber Corresponding track number
     * @param trackSegmentNumber Corresponding track segment number
     * @param points A list of points
     * @param name The name of the LineString
     * @param pen A QPen can be used to modify the look of the line
     */
    LineStringExt(int trackNumber, int trackSegmentNumber, QList<Point*> const points, QString name, QPen* pen);

    /**
     * @brief Gets track number
     * @return Track number
     */
    int getTrackNumber() const;

    /**
     * @brief Gets track segment number
     * @return Track segment number
     */
    int getTrackSegmentNumber() const;

    /**
     * @brief Sets line as selected
     * @param selected True is selected
     */
    void setSelected(bool selected);

private:
    const int trackNumber;
    const int trackSegmentNumber;
    bool  selected;

    void draw(QPainter* painter, const MapAdapter* mapadapter, const QRect &screensize, const QPoint offset);
    static int distance(QPoint &A, QPoint &B);
};

#endif // _LINESTRINGEXT_H_
