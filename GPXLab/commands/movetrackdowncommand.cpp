#include "movetrackdowncommand.h"

MoveTrackDownCommand::MoveTrackDownCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber)
{
}

void MoveTrackDownCommand::undo()
{
    // move track up
    gpxmw->moveTrackUp(trackNumber);
}

void MoveTrackDownCommand::redo()
{
    // move track down
    gpxmw->moveTrackDown(trackNumber);
}
