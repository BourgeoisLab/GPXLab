#ifndef QPIXMAPEXT_H
#define QPIXMAPEXT_H

#include <QPixmap>
#include "point.h"

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

/** @} Widgets */

#endif // QPIXMAPEXT_H
