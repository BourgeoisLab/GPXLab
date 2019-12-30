#ifndef SELECTCOMMAND_H
#define SELECTCOMMAND_H

#include <QUndoCommand>
#include "gpx_wrapper.h"

/**
 * @addtogroup Commands Commands
 * @brief Command functions implementing QUndoCommand
 * @{
 */

/**
 * @class SelectCommand
 *
 * @brief Command to select a track, track segment and/or track point
 */
class SelectCommand : public QUndoCommand
{
public:

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 whole track
     * @param pointNumber Point number, if -1 no point will be selected
     * @param parent Parent
     */
    SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber = -1, int pointNumber = -1, QUndoCommand *parent = nullptr);

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 whole track
     * @param lat Latitude coordinate of the point to select
     * @param lon Longitude coordinate of the point to select
     * @param parent Parent
     */
    SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, double lat, double lon, QUndoCommand *parent = nullptr);

    /**
     * @brief Constructor
     * @param gpxmw GPX_model wrapper
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 whole track
     * @param timestamp Timestamp of the point to select
     * @param parent Parent
     */
    SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, time_t timestamp, QUndoCommand *parent = nullptr);

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
    double lat;
    double lon;
    time_t timestamp;
};

/** @} Commands */

#endif // SELECTCOMMAND_H
