#include "setaltitudecommand.h"

SetAltitudeCommand::SetAltitudeCommand(GPX_wrapper *gpxmw, int trackNumber, const QVector<double> &values, QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    values(values)
{
}

void SetAltitudeCommand::undo()
{
    redo();
}

void SetAltitudeCommand::redo()
{
    // generate values
    gpxmw->generateDiagramValues(trackNumber, -1, GPX_wrapper::altitude, GPX_wrapper::none);

    // temporary copy old values
    QVector<double> tmpValues = gpxmw->getAltitudeValues();

    // set new values
    gpxmw->setAltitudeValues(values, trackNumber);

    // store old values
    values = tmpValues;
}
