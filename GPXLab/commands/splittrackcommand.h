#ifndef SPLITTRACKCOMMAND_H
#define SPLITTRACKCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class SplitTrackCommand
 *
 * @brief Split track command
 */
class SplitTrackCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 for whole track
     * @param pointNumber Point number at which the track is combined
     * @param parent Parent
     */
    SplitTrackCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent = 0);

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
};

/** @} Commands */

#endif // SPLITTRACKCOMMAND_H
