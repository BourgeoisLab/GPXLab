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

#ifndef QMAPTRACK_H
#define QMAPTRACK_H

#include "point.h"
#include "curve.h"
#include "gpx_wrapper.h"

using namespace qmapcontrol;

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QPixmapExt
 *
 * @brief QPixmap subclass
 *
 * Extends the QPixmap class by adding two values to algn the pixmap.
 *
 * @see QPixmap
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 1 Feb 2015
 */
class QPixmapExt : public QPixmap
{
public:

    /**
     * @brief Creates an empty QPixmapExt
     */
    QPixmapExt();

    /**
     * @brief Creates a QPixmapExt with the given size and alingment
     * @param width Width
     * @param height Height
     * @param alignment Alignment
     */
    QPixmapExt(int width, int height, Point::Alignment alignment = Point::Middle);

    /**
     * @brief Creates a QPixmapExt from a image file
     * @param fileName Image file name
     * @param alignment Alignment
     */
    QPixmapExt(const QString & fileName, Point::Alignment alignment = Point::Middle);

    /**
     * @brief Gets the alignment offset for the x coordinate
     * @return Alignment offset
     */
    int getAlignmentX() const;

    /**
     * @brief Gets the alignment offset for the y coordinate
     * @return Alignment offset
     */
    int getAlignmentY() const;

    /**
     * @brief Checks if pixmap is from an image
     * @return True if pixmap is from an image
     */
    bool isFromImage() const;

private:

    /**
     * @brief Calculates the offsets
     * @param alignment Alignment
     */
    void alignToTopLeft(Point::Alignment alignment);

    /** @brief Offset for the x coordinate */
    int dx;

    /** @brief Offset for the y coordinate */
    int dy;

    /** @brief True if pixmap is from an image */
    bool isFromImg;
};


/**
 * @class QMapTrack
 *
 * @brief Curve subclass
 *
 * Extends the Curve class.
 *
 * @see Curve
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 1 Feb 2015
 */
class QMapTrack : public Curve
{
    Q_OBJECT

public:

    /**
     * @brief Creates a new QMapTrack
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number of the new track
     * @param track Track
     */
    QMapTrack(const GPX_wrapper *gpxmw, int trackNumber, const GPX_trkType *track);
    ~QMapTrack();

    /**
     * @brief Returns the bounding box of the track
     * @return Bounding box
     */
    QRectF boundingBox();

    /**
     * @brief Gets the track number
     * @return Track number
     */
    int getTrackNumber() const;

    /**
     * @brief Sets the track number
     * @param number Track number
     */
    void setTrackNumber(int number);

    /**
     * @brief Gets the track segment touched by a mouse click
     * @return Track segment
     */
    int getTouchedSegmentNumber() const;

    /**
     * @brief Gets the point touched by a mouse click
     * @return Touched point
     */
    const GPX_wptType *getTouchedPoint() const;

    /**
     * @brief Sets if the mouse is currently above the track
     * @param over True if mouse over the track
     */
    void setMouseOver(bool over);

    /**
     * @brief Redraws a point
     * @param pointNumber Point number
     */
    void redrawPoint(int pointNumber);

private:

    /**
     * @brief Draws a arrow pixmap
     * @param sideLength Side length of arrow
     * @param heading Heading of arrow
     * @param pen Pen
     * @return Pixmap
     */
    static QPixmapExt drawArrow(int sideLength, qreal heading, QPen* pen = 0);

    /**
     * @brief Computes the euclid distance between two points
     * @param A Point A
     * @param B Point B
     * @return Distance
     */
    static int distance(QPoint &A, QPoint &B);

    /**
     * @brief Point in the curve
     * @return Point list
     */
    QList<Point*> points();

    /**
     * @brief Draws the track
     * @param painter Painter
     * @param mapadapter Map adapter
     * @param screensize Screen size
     * @param offset Offet
     */
    void draw(QPainter* painter, const MapAdapter* mapadapter, const QRect &screensize, const QPoint offset);

    /**
     * @brief Touches
     * @param geom
     * @param mapadapter
     * @return
     */
    bool Touches(Point* geom, const MapAdapter* mapadapter);


    /**
     * @brief Draws a point
     * @param painter Painter
     * @param mapadapter Map adapter
     * @param viewport View port
     * @param pixmap Pixmap of point
     * @param latitude Latitude of point
     * @param longitude Longitude of point
     */
    void drawPoint(QPainter* painter, const MapAdapter* mapadapter, const QRect &viewport, const QPixmapExt *pixmap, double latitude, double longitude);

private:

    const GPX_wrapper *gpxmw;
    const GPX_trkType *track;
    int trackNumber;

    QPixmapExt *pointPixmap;

    int touchedSegmentNumber;
    const GPX_wptType *touchedPoint;
    bool mouseOver;

    QPen* linePen;
    QPen* linePenSelected;
    QPen* linePenSameTrack;
    QPen* linePenOver;
    QPen* pointPenSelected;
    QPen* pointsMiddle;

    QPointF clickedCoord;
};

/** @} Widgets */

#endif // QMAPTRACK_H
