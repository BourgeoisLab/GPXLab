#include "editfilepropertiescommand.h"

EditFilePropertiesCommand::EditFilePropertiesCommand(GPX_wrapper *gpxmw, GPX_metadataType &metadata, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    metadata(metadata)
{
}

void EditFilePropertiesCommand::undo()
{
    redo();
}

void EditFilePropertiesCommand::redo()
{
    // temporary copy old values
    GPX_metadataType tmpMetadata = *gpxmw->getModelMetadata();

    // set new values
    gpxmw->setModelMetadata(metadata);

    // store old values
    metadata = tmpMetadata;
}
