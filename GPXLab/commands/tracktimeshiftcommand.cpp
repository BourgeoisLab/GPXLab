#include "tracktimeshiftcommand.h"

TrackTimeShiftCommand::TrackTimeShiftCommand(GPX_wrapper *gpxmw, int trackNumber, long seconds, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    seconds(seconds)
{
}

void TrackTimeShiftCommand::undo()
{
    // time shift back
    gpxmw->timeShiftTrack(trackNumber, -seconds);
}

void TrackTimeShiftCommand::redo()
{
    // time shift
    gpxmw->timeShiftTrack(trackNumber, seconds);
}
