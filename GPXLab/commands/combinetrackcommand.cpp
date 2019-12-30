#include "combinetrackcommand.h"

CombineTrackCommand::CombineTrackCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber)
{
}

void CombineTrackCommand::undo()
{
    // split track segments
    gpxmw->splitTrack(trackNumber, trackSegmentNumber, pointNumber);
}

void CombineTrackCommand::redo()
{
    // combine track segments
    gpxmw->combineTrack(trackNumber, trackSegmentNumber, pointNumber);
}
