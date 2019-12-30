#ifndef SETALTITUDECOMMAND_H
#define SETALTITUDECOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class SetAltitudeCommand
 *
 * @brief Set altitude values command
 */
class SetAltitudeCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Set altitude values to this track number
     * @param values Altitude values
     * @param parent Parent
     */
    SetAltitudeCommand(GPX_wrapper *gpxmw, int trackNumber, const QVector<double> &values, QUndoCommand *parent = 0);

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
    QVector<double> values;
};

/** @} Commands */

#endif // SETALTITUDECOMMAND_H
