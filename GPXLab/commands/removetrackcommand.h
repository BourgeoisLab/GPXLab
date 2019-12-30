#ifndef REMOVETRACKCOMMAND_H
#define REMOVETRACKCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class RemoveTrackCommand
 *
 * @brief Remove track command
 */
class RemoveTrackCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number to remove
     * @param parent Parent
     */
    RemoveTrackCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent = nullptr);

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
    size_t trackNumber;
    GPX_trkType trk;
};

/** @} Commands */

#endif // REMOVETRACKCOMMAND_H
