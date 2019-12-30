#ifndef EDITFILEPROPERTIESCOMMAND_H
#define EDITFILEPROPERTIESCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class EditFilePropertiesCommand
 *
 * @brief Edit file properties command
 */
class EditFilePropertiesCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param metadata Metadata of file
     * @param parent Parent
     */
    EditFilePropertiesCommand(GPX_wrapper *gpxmw, GPX_metadataType &metadata, QUndoCommand *parent = nullptr);

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
    GPX_metadataType metadata;
};

/** @} Commands */

#endif // EDITFILEPROPERTIESCOMMAND_H
