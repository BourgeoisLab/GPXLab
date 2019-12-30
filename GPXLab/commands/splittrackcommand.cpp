#include "splittrackcommand.h"

SplitTrackCommand::SplitTrackCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber)
{
}

void SplitTrackCommand::undo()
{
    // combine track segments
    gpxmw->combineTrack(trackNumber, trackSegmentNumber, pointNumber);
}

void SplitTrackCommand::redo()
{
    // split track segments
    gpxmw->splitTrack(trackNumber, trackSegmentNumber, pointNumber);
}
