#ifndef MOVETRACKDOWNCOMMAND_H
#define MOVETRACKDOWNCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class MoveTrackDownCommand
 *
 * @brief Move track down command
 */
class MoveTrackDownCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number to move down
     * @param parent Parent
     */
    MoveTrackDownCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent = nullptr);

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

#endif // MOVETRACKDOWNCOMMAND_H
