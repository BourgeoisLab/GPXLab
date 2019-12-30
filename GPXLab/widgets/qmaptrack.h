#ifndef QMAPTRACK_H
#define QMAPTRACK_H

#include "point.h"
#include "curve.h"
#include "gpx_wrapper.h"
#include "qpixmapext.h"

using namespace qmapcontrol;

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QMapTrack
 *
 * @brief Curve subclass
 *
 * Extends the Curve class.
 *
 * @see Curve
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

    const QPixmapExt pointPixmapFirst;
    const QPixmapExt pointPixmapFirstMiddle;
    const QPixmapExt pointPixmapLastMiddle;
    const QPixmapExt pointPixmapLast;

    const GPX_wrapper *gpxmw;
    const GPX_trkType *track;
    int trackNumber;

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
