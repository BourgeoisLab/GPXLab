#ifndef APPENDTRACKCOMMAND_H
#define APPENDTRACKCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class AppendTrackCommand
 *
 * @brief Append track command
 */
class AppendTrackCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param fileName File containing tracks to append
     * @param fileType File type, GPXM_FILE_AUTOMATIC for automatic detection
     * @param parent Parent
     */
    AppendTrackCommand(GPX_wrapper *gpxmw, const QString &fileName, GPX_model::fileType_e fileType, QUndoCommand *parent = nullptr);

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
    const QString fileName;
    int numTracksBeforeAppend;
    GPX_model::fileType_e fileType;
};

/** @} Commands */

#endif // APPENDTRACKCOMMAND_H
