#ifndef MOVETRACKUPCOMMAND_H
#define MOVETRACKUPCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class MoveTrackUpCommand
 *
 * @brief Move track up command
 */
class MoveTrackUpCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number to move up
     * @param parent Parent
     */
    MoveTrackUpCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent = nullptr);

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
};

/** @} Commands */

#endif // MOVETRACKUPCOMMAND_H
