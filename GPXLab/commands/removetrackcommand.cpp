#include "removetrackcommand.h"

RemoveTrackCommand::RemoveTrackCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trk(trackNumber)
{
    const GPX_trkType *ptr = gpxmw->getTrack(trackNumber);
    if (ptr)
        trk = *ptr;
}

void RemoveTrackCommand::undo()
{
    // insert track
    gpxmw->insertTrack(trackNumber, trk);
}

void RemoveTrackCommand::redo()
{
    // remove track
    gpxmw->removeTrack(trackNumber);
}
