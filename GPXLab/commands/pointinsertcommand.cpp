#include "pointinsertcommand.h"

PointInsertCommand::PointInsertCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, GPX_wptType &wpt, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber),
    wpt(wpt)
{
}

void PointInsertCommand::undo()
{
    // delete point
    gpxmw->deletePoint(trackNumber, trackSegmentNumber, pointNumber);
}

void PointInsertCommand::redo()
{
    // insert point
    gpxmw->insertPoint(trackNumber, trackSegmentNumber, pointNumber, wpt);
}
