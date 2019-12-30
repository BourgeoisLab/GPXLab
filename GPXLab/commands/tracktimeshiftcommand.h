#ifndef TRACKTIMESHIFTCOMMAND_H
#define TRACKTIMESHIFTCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class TrackTimeShiftCommand
 *
 * @brief Time shift track command
 */
class TrackTimeShiftCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param seconds Amount of seconds to shift
     * @param parent Parent
     */
    TrackTimeShiftCommand(GPX_wrapper *gpxmw, int trackNumber, long seconds, QUndoCommand *parent = nullptr);

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
    long seconds;
};

/** @} Commands */

#endif // TRACKTIMESHIFTCOMMAND_H
