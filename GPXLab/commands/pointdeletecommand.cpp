#include "pointdeletecommand.h"

PointDeleteCommand::PointDeleteCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber)
{
    const GPX_wptType *ptr = gpxmw->getPoint(trackNumber, trackSegmentNumber, pointNumber);
    if (ptr)
        wpt = *ptr;
}

void PointDeleteCommand::undo()
{
    // insert point
    gpxmw->insertPoint(trackNumber, trackSegmentNumber, pointNumber, wpt);
}

void PointDeleteCommand::redo()
{
    // delete point
    gpxmw->deletePoint(trackNumber, trackSegmentNumber, pointNumber);
}
