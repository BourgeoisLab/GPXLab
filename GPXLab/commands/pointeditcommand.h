#ifndef POINTEDITCOMMAND_H
#define POINTEDITCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class PointEditCommand
 *
 * @brief Edit point property command
 */
class PointEditCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param numProperties Number of properties
     * @param property Property to edit
     * @param value Value
     * @param parent Parent
     */
    PointEditCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, GPX_wrapper::TrackPointProperty property[], QString value[], QUndoCommand *parent = 0);

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param numProperties Number of properties
     * @param property Property to edit
     * @param value Value
     * @param parent Parent
     */
    PointEditCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, GPX_wrapper::TrackPointProperty property[], double value[], QUndoCommand *parent = 0);

    ~PointEditCommand();

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
    size_t numProperties;
    GPX_wrapper::TrackPointProperty *property;
    QString *sValue;
    double *dValue;
};

/** @} Commands */

#endif // POINTEDITCOMMAND_H
