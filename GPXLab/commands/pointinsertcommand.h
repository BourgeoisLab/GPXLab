#ifndef POINTINSERTCOMMAND_H
#define POINTINSERTCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class PointInsertCommand
 *
 * @brief Insert point command
 */
class PointInsertCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param wpt Point
     * @param parent Parent
     */
    PointInsertCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, GPX_wptType &wpt, QUndoCommand *parent = nullptr);

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
    GPX_wptType wpt;
};

/** @} Commands */

#endif // POINTINSERTCOMMAND_H
