#ifndef QTREEWIDGETTRACKS_H
#define QTREEWIDGETTRACKS_H

#include <QTreeWidget>
#include "gpx_wrapper.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QTreeWidgetTracks
 *
 * @brief QTreeWidget subclass
 *
 * Extends the QTreeWidget class.
 *
 * @see QTreeWidget
 */
class QTreeWidgetTracks : public QTreeWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    explicit QTreeWidgetTracks(QWidget *parent = nullptr);

    /**
     * @brief Generates the tree
     * @param gpxmw GPX_model wrapper
     */
    void build(const GPX_wrapper *gpxmw);

    /**
     * @brief Selects the track or track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 select track
     */
    void select(int trackNumber, int trackSegmentNumber = -1);

    /**
     * @brief Sets the text of an item
     * @param name New text
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, -1 for track
     */
    void setItemName(const QString& name, int trackNumber, int trackSegmentNumber = -1);
};

/** @} Widgets */

#endif // QTREEWIDGETTRACKS_H
