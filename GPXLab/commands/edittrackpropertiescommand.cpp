#include "edittrackpropertiescommand.h"

EditTrackPropertiesCommand::EditTrackPropertiesCommand(GPX_wrapper *gpxmw, int trackNumber, GPX_trkMetadataType &metadata, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    metadata(metadata)
{
}

void EditTrackPropertiesCommand::undo()
{
    redo();
}

void EditTrackPropertiesCommand::redo()
{
    // temporary copy old values
    GPX_trkMetadataType tmpMetadata = *gpxmw->getTrackMetadata(trackNumber);

    // set new values
    gpxmw->setTrackMetadata(trackNumber, metadata);

    // store old values
    metadata = tmpMetadata;
}
