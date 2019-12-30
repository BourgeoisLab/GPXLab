#include "selectcommand.h"

SelectCommand::SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber),
    lat(0.0),
    lon(0.0),
    timestamp(0)
{
}

SelectCommand::SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, double lat, double lon, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(-2),
    lat(lat),
    lon(lon),
    timestamp(0)
{
}

SelectCommand::SelectCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, time_t timestamp, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(-2),
    lat(0.0),
    lon(0.0),
    timestamp(timestamp)
{
}

void SelectCommand::undo()
{
    redo();
}

void SelectCommand::redo()
{
    // temporary copy old values
    int tmpTrackNumber = gpxmw->getSelectedTrackNumber();
    int tmpTrackSegmentNumber = gpxmw->getSelectedTrackSegmentNumber();
    int tmpPointNumber = gpxmw->getSelectedPointNumber();

    // set new values
    if (pointNumber != -2)
        gpxmw->select(trackNumber, trackSegmentNumber, pointNumber);
    else if (timestamp != 0)
        gpxmw->select(trackNumber, trackSegmentNumber, timestamp);
    else
        gpxmw->select(trackNumber, trackSegmentNumber, lat, lon);

    // store old values
    trackNumber = tmpTrackNumber;
    trackSegmentNumber = tmpTrackSegmentNumber;
    pointNumber = tmpPointNumber;
}
