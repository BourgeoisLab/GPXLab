#ifndef EDITTRACKPROPERTIESCOMMAND_H
#define EDITTRACKPROPERTIESCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class EditTrackPropertiesCommand
 *
 * @brief Edit track properties command
 */
class EditTrackPropertiesCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param metadata Metadata of track
     * @param parent Parent
     */
    EditTrackPropertiesCommand(GPX_wrapper *gpxmw, int trackNumber, GPX_trkMetadataType &metadata, QUndoCommand *parent = 0);

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
    GPX_trkMetadataType metadata;
};

/** @} Commands */

#endif // EDITTRACKPROPERTIESCOMMAND_H
