#include "appendtrackcommand.h"

AppendTrackCommand::AppendTrackCommand(GPX_wrapper *gpxmw, const QString &fileName, GPX_model::fileType_e fileType, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    fileName(fileName),
    fileType(fileType)
{
}

void AppendTrackCommand::undo()
{
    // remove tracks
    for (int i = gpxmw->getNumTracks() - 1; i >= numTracksBeforeAppend; --i)
        gpxmw->removeTrack(i);
}

void AppendTrackCommand::redo()
{
    // save number of tracks before appending
    numTracksBeforeAppend = gpxmw->getNumTracks();

    // append tracks
    gpxmw->load(fileName, fileType, false);
}
