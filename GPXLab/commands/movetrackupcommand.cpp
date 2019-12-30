#include "movetrackupcommand.h"

MoveTrackUpCommand::MoveTrackUpCommand(GPX_wrapper *gpxmw, int trackNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber)
{
}

void MoveTrackUpCommand::undo()
{
    // move track down
    gpxmw->moveTrackDown(trackNumber);
}

void MoveTrackUpCommand::redo()
{
    // move track up
    gpxmw->moveTrackUp(trackNumber);
}
