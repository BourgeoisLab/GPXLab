#include "pointeditcommand.h"

PointEditCommand::PointEditCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, GPX_wrapper::TrackPointProperty property[], QString value[], QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber),
    numProperties(numProperties),
    dValue(nullptr)
{
    size_t i;
    this->property = new GPX_wrapper::TrackPointProperty[numProperties];
    sValue = new QString[numProperties];
    for (i = 0; i < numProperties; ++i)
    {
        this->property[i] = property[i];
        sValue[i] = value[i];
    }
}

PointEditCommand::PointEditCommand(GPX_wrapper *gpxmw, int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, GPX_wrapper::TrackPointProperty property[], double value[], QUndoCommand *parent) :
    QUndoCommand(parent),
    gpxmw(gpxmw),
    trackNumber(trackNumber),
    trackSegmentNumber(trackSegmentNumber),
    pointNumber(pointNumber),
    numProperties(numProperties),
    sValue(nullptr)
{
    size_t i;
    this->property = new GPX_wrapper::TrackPointProperty[numProperties];
    dValue = new double[numProperties];
    for (i = 0; i < numProperties; ++i)
    {
        this->property[i] = property[i];
        dValue[i] = value[i];
    }
}

PointEditCommand::~PointEditCommand()
{
    if (sValue)
        for (size_t i = 0; i < numProperties; ++i)
            delete &sValue[i];
    delete[] property;
    delete[] sValue;
    delete[] dValue;
}

void PointEditCommand::undo()
{
    redo();
}

void PointEditCommand::redo()
{
    const GPX_wptType *trkpt = gpxmw->getPoint(trackNumber, trackSegmentNumber, pointNumber);
    if (trkpt)
    {
        size_t i;
        if (dValue)
        {
            // temporary copy old values
            double *tmpValue = new double[numProperties];
            for (i = 0; i < numProperties; ++i)
                tmpValue[i] = gpxmw->getTrackPointPropertyAsDouble(trkpt, property[i]);

            // set new values
            gpxmw->setTrackPointProperty(trackNumber, trackSegmentNumber, pointNumber, numProperties, property, dValue);

            // store old values
            for (i = 0; i < numProperties; ++i)
                dValue[i] = tmpValue[i];

            delete[] tmpValue;
        }
        else
        {
            // temporary copy old values
            QString *tmpValue = new QString[numProperties];
            for (i = 0; i < numProperties; ++i)
                tmpValue[i] = gpxmw->getTrackPointPropertyAsString(trkpt, property[i]);

            // set new values
            gpxmw->setTrackPointProperty(trackNumber, trackSegmentNumber, pointNumber, numProperties, property, sValue);

            // store old values
            for (i = 0; i < numProperties; ++i)
                sValue[i] = tmpValue[i];

            delete[] tmpValue;
        }
    }
}
