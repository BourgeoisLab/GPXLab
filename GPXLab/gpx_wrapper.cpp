/****************************************************************************
 *   Copyright (c) 2014 - 2015 Frederic Bourgeois <bourgeoislab@gmail.com>  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with This program. If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

#include <QDateTime>
#include "qutils.h"
#include "gpx_wrapper.h"

GPX_wrapper::GPX_wrapper(const QString &creator) :
    gpxm(NULL)
{
    init(creator);
}

GPX_wrapper::~GPX_wrapper()
{
    clear();
}

void GPX_wrapper::init(const QString& creator)
{
    clear();
    gpxm = new GPX_model(creator.toStdString());
}

void GPX_wrapper::clear()
{
    fileName = "";
    selectedTrackNumber = -1;
    selectedTrackSegmentNumber = -1;
    selectedPointNumber = -1;
    selectedPoint = NULL;
    timeValues.clear();
    altitudeValues.clear();
    curveMainValues.clear();
    curveSecondaryValues.clear();
    if (gpxm)
    {
        delete gpxm;
        gpxm = NULL;
        emit modelCleared();
    }
}

GPX_model::retCode_e GPX_wrapper::load(const QString &fileName, GPX_model::fileType_e fileType, bool overwriteMetadata)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    GPX_model::retCode_e ret = gpxm->load(fileName.toStdString(), fileType, overwriteMetadata);
    if (ret != GPX_model::GPXM_ERR_FILEOPEN && ret != GPX_model::GPXM_ERR_INVALID_ARG)
    {
        if (overwriteMetadata)
            this->fileName = fileName;
        emit fileLoaded();
    }
    return ret;
}

GPX_model::retCode_e GPX_wrapper::save(const QString &fileName)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    GPX_model::retCode_e ret = gpxm->save(fileName.toStdString());
    if (ret != GPX_model::GPXM_ERR_FILEOPEN && ret != GPX_model::GPXM_ERR_INVALID_ARG)
    {
        this->fileName = fileName;
        emit fileSaved();
    }
    return ret;
}

const QString &GPX_wrapper::getFileName() const
{
    return fileName;
}

const GPX_statsType* GPX_wrapper::getModelStats() const
{
    if (!gpxm)
        return NULL;
    return &gpxm->stats;
}

const GPX_metadataType* GPX_wrapper::getModelMetadata() const
{
    if (!gpxm)
        return NULL;
    return &gpxm->metadata;
}

GPX_model::retCode_e GPX_wrapper::setModelMetadata(const GPX_metadataType &metadata)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    gpxm->metadata = metadata;
    emit modelMetadataChanged();
    return GPX_model::GPXM_OK;
}

int GPX_wrapper::getNumTracks() const
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;
    return (int)gpxm->trk.size();
}

const GPX_statsType* GPX_wrapper::getTrackStats(int trackNumber) const
{
    if (!gpxm)
        return NULL;
    if ((size_t)trackNumber < gpxm->trk.size())
        return &gpxm->trk[trackNumber].stats;
    return NULL;
}

const GPX_trkMetadataType* GPX_wrapper::getTrackMetadata(int trackNumber) const
{
    if (!gpxm)
        return NULL;
    if ((size_t)trackNumber < gpxm->trk.size())
        return &gpxm->trk[trackNumber].metadata;
    return NULL;
}

GPX_model::retCode_e GPX_wrapper::setTrackMetadata(int trackNumber, const GPX_trkMetadataType &metadata)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        gpxm->trk[trackNumber].metadata = metadata;
        emit trackMetadataChanged(trackNumber);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

const GPX_statsType* GPX_wrapper::getItemStats(int trackNumber, int trackSegmentNumber) const
{
    if (!gpxm)
        return NULL;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if (trackSegmentNumber == -1)
        {
            return &gpxm->trk[trackNumber].stats;
        }
        else
        {
            if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
                return &gpxm->trk[trackNumber].trkseg[trackSegmentNumber].stats;
        }
    }
    return NULL;
}

const QString GPX_wrapper::getItemName(int trackNumber, int trackSegmentNumber) const
{
    QString str = "";
    if (!gpxm)
        return str;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if (trackSegmentNumber == -1)
        {
            return str.fromStdString(gpxm->trk[trackNumber].metadata.name);
        }
        else
        {
            if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
                return str.fromStdString(gpxm->trk[trackNumber].metadata.name) + " (#" + QString::number(trackSegmentNumber+1) + ")";
        }
    }
    return str;
}

const GPX_trkType *GPX_wrapper::getTrack(int trackNumber) const
{
    if (!gpxm)
        return NULL;
    if ((size_t)trackNumber < gpxm->trk.size())
        return &gpxm->trk[trackNumber];
    return NULL;
}

const GPX_trksegType *GPX_wrapper::getTrackSegment(int trackNumber, int trackSegmentNumber) const
{
    if (!gpxm)
        return NULL;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            return &gpxm->trk[trackNumber].trkseg[trackSegmentNumber];
        }
    }
    return NULL;
}

const GPX_wptType *GPX_wrapper::getPoint(int trackNumber, int trackSegmentNumber, int pointNumber) const
{
    if (!gpxm)
        return NULL;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if (trackSegmentNumber == -1)
        {
            vector<GPX_trksegType>::iterator trkseg;
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); ++trkseg)
            {
                if ((size_t)pointNumber >= trkseg->trkpt.size())
                    pointNumber -= (int)trkseg->trkpt.size();
                else
                    return &trkseg->trkpt[pointNumber];
            }
        }
        else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            if ((size_t)pointNumber < gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size())
                return &gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt[pointNumber];
        }
    }
    return NULL;
}

QString GPX_wrapper::getTrackPointPropertyLabel(TrackPointProperty property, bool newLines) const
{
    switch(property)
    {
    case none:
        return tr("None");
    case fix:
        return tr("Fix");
    case sat:
        return tr("Satellites");
    case timestamp:
        return tr("Date & Time");
    case magvar:
        return newLines ? tr("Magnetic\nvariation") : tr("Magnetic variation");
    case altitude:
        return newLines ? tr("Elevation\n[m]") : tr("Elevation [m]");
    case hdop:
        return tr("HDOP");
    case vdop:
        return tr("VDOP");
    case pdop:
        return tr("PDOP");
    case ageofdgpsdata:
        return newLines ? tr("Age of\nDGPS") : tr("Age of DGPS");
    case dgpsid:
        return newLines ? tr("DGPS\nID") : tr("DGPS ID");
    case geoidheight:
        return newLines ? tr("Geoid height\n[m]") : tr("Geoid height [m]");
    case latitude:
        return newLines ? tr("Latitude\n[deg]") : tr("Latitude [deg]");
    case longitude:
        return newLines ? tr("Longitude\n[deg]") : tr("Longitude [deg]");
    case name:
        return tr("Name");
    case cmt:
        return tr("Comment");
    case desc:
        return tr("Description");
    case src:
        return tr("Source");
    case links:
        return tr("Links");
    case sym:
        return tr("Symbol");
    case type:
        return tr("Type");
    case heartrate:
        return newLines ? tr("Heart rate\n[bpm]") : tr("Heart rate [bpm]");
    case speed:
        return newLines ? tr("Speed\n[km/h]") : tr("Speed [km/h]");
    case heading:
        return tr("Heading");
    case leglength:
        return newLines ? tr("Leg length\n[m]") : tr("Leg length [m]");
    case distance:
        return newLines ? tr("Distance\n[km]") : tr("Distance [km]");
    case elapsedTime:
        return newLines ? tr("Elapsed time\n[s]") : tr("Elapsed time [s]");
    default:
        return "";
    }
}

double GPX_wrapper::getTrackPointPropertyAsDouble(const GPX_wptType *trkpt, TrackPointProperty property) const
{
    switch(property)
    {
    case none:
        return 0.0;
    case fix:
        return 0.0;
    case sat:
        return (double)trkpt->sat;
    case timestamp:
        return (double)(trkpt->timestamp + ((double)trkpt->millisecond/1000));
    case magvar:
        return (double)trkpt->magvar;
    case altitude:
        return trkpt->altitude;
    case hdop:
        return (double)trkpt->hdop;
    case vdop:
        return (double)trkpt->vdop;
    case pdop:
        return (double)trkpt->pdop;
    case ageofdgpsdata:
        return (double)trkpt->ageofdgpsdata;
    case dgpsid:
        return (double)trkpt->dgpsid;
    case geoidheight:
        return (double)trkpt->geoidheight;
    case latitude:
        return trkpt->latitude;
    case longitude:
        return trkpt->longitude;
    case name:
        return 0.0;
    case cmt:
        return 0.0;
    case desc:
        return 0.0;
    case src:
        return 0.0;
    case links:
        return (double)trkpt->links.size();
    case sym:
        return 0.0;
    case type:
        return 0.0;
    case heartrate:
        return (double)trkpt->extensionsGarmin.heartrate;
    case speed:
        return (double)trkpt->speed;
    case heading:
        return (double)trkpt->heading;
    case leglength:
        return trkpt->leglength;
    case distance:
        return trkpt->distanceTot;
    case elapsedTime:
        return (double)trkpt->elapsedTime;
    default:
        return 0.0;
    }
}

QString GPX_wrapper::getTrackPointPropertyAsString(const GPX_wptType *trkpt, TrackPointProperty property) const
{
    QDateTime ts;
    QString sTmp;
    switch(property)
    {
    case none:
        return "";
    case fix:
        return QString::fromStdString(trkpt->fix);
    case sat:
        return QString::number(trkpt->sat);
    case timestamp:
        ts.setTime_t(trkpt->timestamp);
        sTmp = ts.toString("dd.MM.yyyy H:mm:ss");
        if (trkpt->millisecond)
            sTmp += "." + QString("%1").arg(trkpt->millisecond, 3, 10, QChar('0'));
        return sTmp;;
    case magvar:
        return QString::number(trkpt->magvar, 'f', 1);
    case altitude:
        return QString::number(trkpt->altitude, 'f', 0);
    case hdop:
        return QString::number(trkpt->hdop, 'f', 1);
    case vdop:
        return QString::number(trkpt->vdop, 'f', 1);
    case pdop:
        return QString::number(trkpt->pdop, 'f', 1);
    case ageofdgpsdata:
        return QString::number(trkpt->ageofdgpsdata, 'f', 1);
    case dgpsid:
        return QString::number(trkpt->dgpsid);
    case geoidheight:
        return QString::number(trkpt->geoidheight, 'f', 1);
    case latitude:
        return QString::number(trkpt->latitude, 'f', 6);
    case longitude:
        return QString::number(trkpt->longitude, 'f', 6);
    case name:
        return QString::fromStdString(trkpt->name);
    case cmt:
        return QString::fromStdString(trkpt->cmt);
    case desc:
        return QString::fromStdString(trkpt->desc);
    case src:
        return QString::fromStdString(trkpt->src);
    case links:
        return "";
    case sym:
        return QString::fromStdString(trkpt->sym);
    case type:
        return QString::fromStdString(trkpt->type);
    case heartrate:
        return QString::number(trkpt->extensionsGarmin.heartrate);
    case speed:
        return QString::number(trkpt->speed, 'f', 1);
    case heading:
        return QString::fromStdString(trkpt->cardinal());
    case leglength:
        return QString::number(trkpt->leglength, 'f', 1);
    case distance:
        return QString::number(trkpt->distanceTot, 'f', 1);
    case elapsedTime:
        return QUtils::seconds_to_DHMS(trkpt->elapsedTime);
    default:
        return "";
    }
}

GPX_model::retCode_e GPX_wrapper::setTrackPointProperty(int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, TrackPointProperty property[], QString value[])
{
    GPX_wptType *trkpt = (GPX_wptType *)getPoint(trackNumber, trackSegmentNumber, pointNumber);
    if (trkpt)
    {
        TrackPointProperty properties = none;
        for (size_t i = 0; i < numProperties; ++i)
        {
            properties = (TrackPointProperty)(properties | property[i]);
            switch(property[i])
            {
            case none:
                break;
            case fix:
                trkpt->fix = value[i].toStdString();
                break;
            case sat:
                trkpt->sat = value[i].toInt();
                break;
            case timestamp:
                trkpt->timestamp = value[i].toULong();
                trkpt->millisecond = 0;
                break;
            case magvar:
                trkpt->magvar = value[i].toFloat();
                break;
            case altitude:
                trkpt->altitude = value[i].toDouble();
                break;
            case hdop:
                trkpt->hdop = value[i].toFloat();
                break;
            case vdop:
                trkpt->vdop = value[i].toFloat();
                break;
            case pdop:
                trkpt->pdop = value[i].toFloat();
                break;
            case ageofdgpsdata:
                trkpt->ageofdgpsdata = value[i].toFloat();
                break;
            case dgpsid:
                trkpt->dgpsid = value[i].toInt();
                break;
            case geoidheight:
                trkpt->geoidheight = value[i].toFloat();
                break;
            case latitude:
                trkpt->latitude = value[i].toDouble();
                properties = (TrackPointProperty)(properties | speed | heading | leglength | distance | elapsedTime);
                break;
            case longitude:
                trkpt->longitude = value[i].toDouble();
                properties = (TrackPointProperty)(properties | speed | heading | leglength | distance | elapsedTime);
                break;
            case name:
                trkpt->name = value[i].toStdString();
                break;
            case cmt:
                trkpt->cmt = value[i].toStdString();
                break;
            case desc:
                trkpt->desc = value[i].toStdString();
                break;
            case src:
                trkpt->src = value[i].toStdString();
                break;
            case links:
                break;
            case sym:
                trkpt->sym = value[i].toStdString();
                break;
            case type:
                trkpt->type = value[i].toStdString();
                break;
            case heartrate:
                trkpt->extensionsGarmin.heartrate = value[i].toInt();
                break;
            case speed:
                trkpt->speed = value[i].toFloat();
                break;
            case heading:
                trkpt->heading = value[i].toFloat();
                break;
            case leglength:
                trkpt->leglength = value[i].toDouble();
                break;
            case distance:
                trkpt->distanceTot = value[i].toDouble();
                break;
            case elapsedTime:
                trkpt->elapsedTime = value[i].toUInt();
                break;
            default:
                break;
            }
        }
        gpxm->updateTrack(gpxm->trk[trackNumber]);
        gpxm->update(false);
        emit pointEdited(trackNumber, trackSegmentNumber, pointNumber, properties);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

GPX_model::retCode_e GPX_wrapper::setTrackPointProperty(int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, TrackPointProperty property[], double value[])
{
    GPX_wptType *trkpt = (GPX_wptType *)getPoint(trackNumber, trackSegmentNumber, pointNumber);
    if (trkpt)
    {
        TrackPointProperty properties = none;
        for (size_t i = 0; i < numProperties; ++i)
        {
            properties = (TrackPointProperty)(properties | property[i]);
            switch(property[i])
            {
            case none:
                break;
            case fix:
                trkpt->fix = 0.0;
                break;
            case sat:
                trkpt->sat = (int)value[i];
                break;
            case timestamp:
                trkpt->timestamp = (time_t)value[i];
                trkpt->millisecond = (int)((value[i] - trkpt->timestamp) * 1000);
                break;
            case magvar:
                trkpt->magvar = (float)value[i];
                break;
            case altitude:
                trkpt->altitude = value[i];
                break;
            case hdop:
                trkpt->hdop = (float)value[i];
                break;
            case vdop:
                trkpt->vdop = (float)value[i];
                break;
            case pdop:
                trkpt->pdop = (float)value[i];
                break;
            case ageofdgpsdata:
                trkpt->ageofdgpsdata = (float)value[i];
                break;
            case dgpsid:
                trkpt->dgpsid = (int)value[i];
                break;
            case geoidheight:
                trkpt->geoidheight = (float)value[i];
                break;
            case latitude:
                trkpt->latitude = value[i];
                properties = (TrackPointProperty)(properties | speed | heading | leglength | distance | elapsedTime);
                break;
            case longitude:
                trkpt->longitude = value[i];
                properties = (TrackPointProperty)(properties | speed | heading | leglength | distance | elapsedTime);
                break;
            case name:
                trkpt->name = 0.0;
                break;
            case cmt:
                trkpt->cmt = 0.0;
                break;
            case desc:
                trkpt->desc = 0.0;
                break;
            case src:
                trkpt->src = 0.0;
                break;
            case links:
                break;
            case sym:
                trkpt->sym = 0.0;
                break;
            case type:
                trkpt->type = 0.0;
                break;
            case heartrate:
                trkpt->extensionsGarmin.heartrate = (int)value[i];
                break;
            case speed:
                trkpt->speed = (float)value[i];
                break;
            case heading:
                trkpt->heading = (float)value[i];
                break;
            case leglength:
                trkpt->leglength = value[i];
                break;
            case distance:
                trkpt->distanceTot = value[i];
                break;
            case elapsedTime:
                trkpt->elapsedTime = (unsigned int)value[i];
                break;
            default:
                break;
            }
        }
        gpxm->updateTrack(gpxm->trk[trackNumber]);
        gpxm->update(false);
        emit pointEdited(trackNumber, trackSegmentNumber, pointNumber, properties);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

int GPX_wrapper::getNumPoints(int trackNumber, int trackSegmentNumber) const
{
    if (!gpxm)
        return 0;

    int numPoints = 0;
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if (trackSegmentNumber == -1)
        {
            vector<GPX_trksegType>::iterator trkseg;
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); ++trkseg)
                numPoints += (int)trkseg->trkpt.size();
        }
        else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            numPoints = (int)gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size();
        }
    }
    return numPoints;
}

GPX_model::retCode_e GPX_wrapper::removeTrack(int trackNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        gpxm->trk.erase(gpxm->trk.begin() + trackNumber);
        updateTrackNumbers();
        gpxm->update(false);
        emit trackDeleted(trackNumber);
        if (trackNumber >= getNumTracks())
            select(getNumTracks() - 1, -1, -1, true);
        else
            select(trackNumber, -1, -1, true);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

GPX_model::retCode_e GPX_wrapper::insertTrack(int trackNumber, const GPX_trkType &trk)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        gpxm->trk.insert(gpxm->trk.begin() + trackNumber, trk);
    }
    else
    {
        gpxm->trk.insert(gpxm->trk.end(), trk);
    }
    updateTrackNumbers();
    gpxm->update(false);
    emit trackInserted(trackNumber, trk);
    select(trackNumber, -1, -1, true);
    return GPX_model::GPXM_OK;
}

GPX_model::retCode_e GPX_wrapper::moveTrackUp(int &trackNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size() && trackNumber > 0)
    {
        iter_swap(gpxm->trk.begin() + trackNumber, gpxm->trk.begin() + trackNumber - 1);
        updateTrackNumbers();
        emit trackMovedUp(trackNumber);
        if (trackNumber == selectedTrackNumber)
            select(trackNumber - 1, selectedTrackSegmentNumber, selectedPointNumber, true);
        else
            select(trackNumber - 1, -1, -1, true);
        --trackNumber;
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::moveTrackDown(int &trackNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size() - 1)
    {
        iter_swap(gpxm->trk.begin() + trackNumber, gpxm->trk.begin() + trackNumber + 1);
        updateTrackNumbers();
        emit trackMovedDown(trackNumber);
        if (trackNumber == selectedTrackNumber)
            select(trackNumber + 1, selectedTrackSegmentNumber, selectedPointNumber, true);
        else
            select(trackNumber + 1, -1, -1, true);
        ++trackNumber;
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::splitTrack(int trackNumber, int &trackSegmentNumber, int &pointNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size() && pointNumber != -1)
    {
        vector<GPX_trksegType>::iterator itTrkSeg = gpxm->trk[trackNumber].trkseg.end();

        if (trackSegmentNumber == -1)
        {
            trackSegmentNumber = 0;
            for (itTrkSeg = gpxm->trk[trackNumber].trkseg.begin(); itTrkSeg != gpxm->trk[trackNumber].trkseg.end(); ++itTrkSeg)
            {
                if ((size_t)pointNumber >= itTrkSeg->trkpt.size())
                {
                    pointNumber -= (int)itTrkSeg->trkpt.size();
                    ++trackSegmentNumber;
                }
                else
                {
                    break;
                }
            }
        }
        else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            if ((size_t)pointNumber < gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size())
                itTrkSeg = gpxm->trk[trackNumber].trkseg.begin() + trackSegmentNumber;
        }

        if (itTrkSeg != gpxm->trk[trackNumber].trkseg.end())
        {
            GPX_trksegType newTrkSeg;

            // copy second half to new track segment
            vector<GPX_wptType>::iterator itTrkPt;
            for (itTrkPt = itTrkSeg->trkpt.begin() + pointNumber; itTrkPt != itTrkSeg->trkpt.end(); ++itTrkPt)
                newTrkSeg.trkpt.push_back(*itTrkPt);

            // delete second half from current segment
            itTrkSeg->trkpt.erase(itTrkSeg->trkpt.begin() + pointNumber, itTrkSeg->trkpt.end());

            // insert new track segment
            gpxm->trk[trackNumber].trkseg.insert(itTrkSeg + 1, newTrkSeg);

            // update track
            gpxm->updateTrack(gpxm->trk[trackNumber]);

            // select new created segment
            ++trackSegmentNumber;
            pointNumber = 0;
            emit trackSplited(trackNumber);
            select(trackNumber, trackSegmentNumber, pointNumber, true);
            return GPX_model::GPXM_OK;
        }
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::combineTrack(int trackNumber, int &trackSegmentNumber, int &pointNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size() && pointNumber != -1)
    {
        vector<GPX_trksegType>::iterator itTrkSeg = gpxm->trk[trackNumber].trkseg.end();

        if (gpxm->trk[trackNumber].trkseg.size() > 1)
        {
            if (trackSegmentNumber == -1)
            {
                trackSegmentNumber = 0;
                for (itTrkSeg = gpxm->trk[trackNumber].trkseg.begin(); itTrkSeg != gpxm->trk[trackNumber].trkseg.end(); ++itTrkSeg)
                {
                    if ((size_t)pointNumber >= itTrkSeg->trkpt.size())
                    {
                        pointNumber -= (int)itTrkSeg->trkpt.size();
                        ++trackSegmentNumber;
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
            {
                if ((size_t)pointNumber < gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size())
                    itTrkSeg = gpxm->trk[trackNumber].trkseg.begin() + trackSegmentNumber;
            }

            if (itTrkSeg != gpxm->trk[trackNumber].trkseg.end())
            {
                bool combineWithPrevious;

                if (itTrkSeg == gpxm->trk[trackNumber].trkseg.begin())
                    combineWithPrevious = false;
                else if (itTrkSeg == gpxm->trk[trackNumber].trkseg.end() - 1)
                    combineWithPrevious = true;
                else if ((size_t)pointNumber > itTrkSeg->trkpt.size()/2)
                    combineWithPrevious = false;
                else
                    combineWithPrevious = true;

                if (combineWithPrevious)
                {
                    pointNumber += (int)(itTrkSeg-1)->trkpt.size();

                    // combine with previous segment
                    vector<GPX_wptType>::iterator itTrkPt;
                    for (itTrkPt = itTrkSeg->trkpt.begin(); itTrkPt != itTrkSeg->trkpt.end(); ++itTrkPt)
                        (itTrkSeg-1)->trkpt.push_back(*itTrkPt);

                    // delete track segment
                    gpxm->trk[trackNumber].trkseg.erase(itTrkSeg);

                    // select combined segment
                    if (gpxm->trk[trackNumber].trkseg.size() > 1)
                        --trackSegmentNumber;
                    else
                        trackSegmentNumber = -1;
                }
                else
                {
                    // combine with next segment
                    vector<GPX_wptType>::iterator itTrkPt;
                    for (itTrkPt = (itTrkSeg+1)->trkpt.begin(); itTrkPt != (itTrkSeg+1)->trkpt.end(); ++itTrkPt)
                        itTrkSeg->trkpt.push_back(*itTrkPt);

                    // copy extensions
                    itTrkSeg->extensions = (itTrkSeg+1)->extensions;

                    // delete track segment
                    gpxm->trk[trackNumber].trkseg.erase(itTrkSeg+1);

                    // select combined segment
                    if (gpxm->trk[trackNumber].trkseg.size() == 1)
                        trackSegmentNumber = -1;
                }

                // update track
                gpxm->updateTrack(gpxm->trk[trackNumber]);

                emit trackCombined(trackNumber);
                select(trackNumber, trackSegmentNumber, pointNumber, true);
                return GPX_model::GPXM_OK;
            }
        }
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::deletePoint(int trackNumber, int &trackSegmentNumber, int pointNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        vector <GPX_wptType> *v;
        if (trackSegmentNumber == -1)
        {
            int orgTrackSegmentNumber = trackSegmentNumber;
            int orgPointNumber = pointNumber;
            trackSegmentNumber = 0;
            vector<GPX_trksegType>::iterator trkseg;
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); ++trkseg)
            {
                if ((size_t)pointNumber >= trkseg->trkpt.size())
                {
                    pointNumber -= (int)trkseg->trkpt.size();
                }
                else
                {
                    v = &trkseg->trkpt;
                    v->erase(v->begin() + pointNumber);

                    // update track
                    gpxm->updateTrack(gpxm->trk[trackNumber]);

                    emit pointDeleted(trackNumber, trackSegmentNumber, pointNumber);
                    if (pointNumber >= getNumPoints(trackNumber, trackSegmentNumber))
                    {
                        pointNumber = getNumPoints(trackNumber, trackSegmentNumber) - 1;
                        orgPointNumber = getNumPoints(trackNumber, orgTrackSegmentNumber) - 1;
                    }
                    select(trackNumber, orgTrackSegmentNumber, orgPointNumber, true);
                    return GPX_model::GPXM_OK;
                }
                ++trackSegmentNumber;
            }
        }
        else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            if ((size_t)pointNumber < gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size())
            {
                v = &gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt;
                v->erase(v->begin() + pointNumber);

                // update track
                gpxm->updateTrack(gpxm->trk[trackNumber]);

                emit pointDeleted(trackNumber, trackSegmentNumber, pointNumber);
                if (pointNumber >= getNumPoints(trackNumber, trackSegmentNumber))
                    pointNumber = getNumPoints(trackNumber, trackSegmentNumber) - 1;
                select(trackNumber, trackSegmentNumber, pointNumber, true);
                return GPX_model::GPXM_OK;
            }
        }
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::insertPoint(int trackNumber, int &trackSegmentNumber, int pointNumber, const GPX_wptType& wpt)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        vector <GPX_wptType> *v;
        if (trackSegmentNumber == -1)
        {
            int orgTrackSegmentNumber = trackSegmentNumber;
            int orgPointNumber = pointNumber;
            trackSegmentNumber = 0;
            vector<GPX_trksegType>::iterator trkseg;
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); ++trkseg)
            {
                if ((size_t)pointNumber > trkseg->trkpt.size())
                {
                    pointNumber -= (int)trkseg->trkpt.size();
                }
                else
                {
                    v = &trkseg->trkpt;
                    v->insert(v->begin() + pointNumber, wpt);

                    // update track
                    gpxm->updateTrack(gpxm->trk[trackNumber]);

                    emit pointInserted(trackNumber, trackSegmentNumber, pointNumber, wpt);
                    select(trackNumber, orgTrackSegmentNumber, orgPointNumber, true);
                    return GPX_model::GPXM_OK;
                }
                ++trackSegmentNumber;
            }
        }
        else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            if ((size_t)pointNumber < gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size() + 1)
            {
                v = &gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt;
                v->insert(v->begin() + pointNumber, wpt);

                // update track
                gpxm->updateTrack(gpxm->trk[trackNumber]);

                emit pointInserted(trackNumber, trackSegmentNumber, pointNumber, wpt);
                select(trackNumber, trackSegmentNumber, pointNumber, true);
                return GPX_model::GPXM_OK;
            }
        }
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::timeShiftTrack(int trackNumber, long seconds)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        vector<GPX_trksegType>::iterator itTrkseg;
        vector<GPX_wptType>::iterator itTrkpt;
        for (itTrkseg = gpxm->trk[trackNumber].trkseg.begin(); itTrkseg != gpxm->trk[trackNumber].trkseg.end(); ++itTrkseg)
        {
            for (itTrkpt = itTrkseg->trkpt.begin(); itTrkpt != itTrkseg->trkpt.end(); ++itTrkpt)
            {
                itTrkpt->timestamp += seconds;
            }
        }
        gpxm->update(true);
        emit trackTimeShifted(trackNumber, seconds);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

GPX_model::retCode_e GPX_wrapper::select(int trackNumber, int trackSegmentNumber, int pointNumber, bool forceSelect)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    GPX_model::retCode_e newTrack = setSelectedTrack(trackNumber, trackSegmentNumber, forceSelect);
    GPX_model::retCode_e newPoint = setSelectedPointByNumber(pointNumber, forceSelect);
    if (newTrack == GPX_model::GPXM_OK)
    {
        emit trackSelectionChanged(selectedTrackNumber, selectedTrackSegmentNumber, selectedPointNumber, selectedPoint);
        return GPX_model::GPXM_OK;
    }
    else if (newPoint == GPX_model::GPXM_OK)
    {
        emit pointSelectionChanged(selectedPointNumber, selectedPoint);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_WARN_NOTHING_CHANGED;
}

GPX_model::retCode_e GPX_wrapper::select(int trackNumber, int trackSegmentNumber, time_t timestamp, bool forceSelect)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    GPX_model::retCode_e newTrack = setSelectedTrack(trackNumber, trackSegmentNumber, forceSelect);
    GPX_model::retCode_e newPoint = setSelectedPointByTimestamp(timestamp, forceSelect);
    if (newTrack == GPX_model::GPXM_OK)
    {
        emit trackSelectionChanged(selectedTrackNumber, selectedTrackSegmentNumber, selectedPointNumber, selectedPoint);
        return GPX_model::GPXM_OK;
    }
    else if (newPoint == GPX_model::GPXM_OK)
    {
        emit pointSelectionChanged(selectedPointNumber, selectedPoint);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_WARN_NOTHING_CHANGED;
}

GPX_model::retCode_e GPX_wrapper::select(int trackNumber, int trackSegmentNumber, double lat, double lon, bool forceSelect)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    GPX_model::retCode_e newTrack = setSelectedTrack(trackNumber, trackSegmentNumber, forceSelect);
    GPX_model::retCode_e newPoint = setSelectedPointByCoord(lat, lon, forceSelect);
    if (newTrack == GPX_model::GPXM_OK)
    {
        emit trackSelectionChanged(selectedTrackNumber, selectedTrackSegmentNumber, selectedPointNumber, selectedPoint);
        return GPX_model::GPXM_OK;
    }
    else if (newPoint == GPX_model::GPXM_OK)
    {
        emit pointSelectionChanged(selectedPointNumber, selectedPoint);
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_WARN_NOTHING_CHANGED;
}

int GPX_wrapper::getSelectedTrackNumber() const
{
    return selectedTrackNumber;
}

int GPX_wrapper::getSelectedTrackSegmentNumber() const
{
    return selectedTrackSegmentNumber;
}

int GPX_wrapper::getSelectedPointNumber() const
{
    return selectedPointNumber;
}

const GPX_wptType *GPX_wrapper::getSelectedPoint() const
{
    return selectedPoint;
}

GPX_model::retCode_e GPX_wrapper::generateDiagramValues(int trackNumber, int trackSegmentNumber, TrackPointProperty curveMain, TrackPointProperty curveSecondary) const
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    // clear old values
    timeValues.clear();
    altitudeValues.clear();
    curveMainValues.clear();
    curveSecondaryValues.clear();

    // generate new values
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        vector<GPX_trksegType>::const_iterator trkseg;
        vector<GPX_wptType>::const_iterator trkpt;
        if (trackSegmentNumber == -1)
        {
            // track
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); trkseg++)
            {
                for (trkpt = trkseg->trkpt.begin(); trkpt != trkseg->trkpt.end(); trkpt++)
                {
                    timeValues.append(trkpt->elapsedTime);
                    altitudeValues.append(trkpt->altitude);
                    curveMainValues.append(getTrackPointPropertyAsDouble(&*trkpt, curveMain));
                    curveSecondaryValues.append(getTrackPointPropertyAsDouble(&*trkpt, curveSecondary));
                }
            }
            return GPX_model::GPXM_OK;
        }
        else
        {
            if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
            {
                // track segment
                for (trkpt = gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.begin(); trkpt != gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.end(); trkpt++)
                {
                    timeValues.append(trkpt->elapsedTime);
                    altitudeValues.append(trkpt->altitude);
                    curveMainValues.append(getTrackPointPropertyAsDouble(&*trkpt, curveMain));
                    curveSecondaryValues.append(getTrackPointPropertyAsDouble(&*trkpt, curveSecondary));
                }
                return GPX_model::GPXM_OK;
            }
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

const QVector<double>& GPX_wrapper::getTimeValues() const
{
    return timeValues;
}

const QVector<double>& GPX_wrapper::getAltitudeValues() const
{
    return altitudeValues;
}

const QVector<double>& GPX_wrapper::getCurveMainValues() const
{
    return curveMainValues;
}

const QVector<double>& GPX_wrapper::getCurveSecondaryValues() const
{
    return curveSecondaryValues;
}

GPX_model::retCode_e GPX_wrapper::setAltitudeValues(const QVector<double> &values, int trackNumber, int trackSegmentNumber)
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        int i = 0;
        vector<GPX_trksegType>::iterator trkseg;
        vector<GPX_wptType>::iterator trkpt;
        if (trackSegmentNumber == -1)
        {
            if (values.size() != gpxm->trk[trackNumber].stats.points)
                return GPX_model::GPXM_ERR_FAILED;

            // track
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); ++trkseg)
            {
                for (trkpt = trkseg->trkpt.begin(); trkpt != trkseg->trkpt.end(); ++trkpt)
                {
                    trkpt->altitude = values[i++];
                }
            }
        }
        else
        {
            if ((size_t)trackSegmentNumber >= gpxm->trk[trackNumber].trkseg.size())
                return GPX_model::GPXM_ERR_FAILED;

            if (values.size() != gpxm->trk[trackNumber].trkseg[trackSegmentNumber].stats.points)
                return GPX_model::GPXM_ERR_FAILED;

            // track segment
            for (trkpt = gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.begin(); trkpt != gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.end(); ++trkpt)
            {
                trkpt->altitude = values[i++];
            }
        }

        // update file
        gpxm->update();

        emit modelPropertiesChanged();
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

int GPX_wrapper::initTrkIteration() const
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    itTrk = gpxm->trk.begin();
    itTrkEnd = gpxm->trk.end();
    return (int)gpxm->trk.size();
}

const GPX_trkType *GPX_wrapper::getNextTrk() const
{
    if (!gpxm)
        return NULL;
    if (itTrk != itTrkEnd)
        return &*itTrk++;
    return NULL;
}

int GPX_wrapper::initTrksegIteration(int trackNumber) const
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        itTrkseg = gpxm->trk[trackNumber].trkseg.begin();
        itTrksegEnd = gpxm->trk[trackNumber].trkseg.end();
        return (int)gpxm->trk[trackNumber].trkseg.size();
    }
    return GPX_model::GPXM_ERR_FAILED;
}

const GPX_trksegType* GPX_wrapper::getNextTrkseg() const
{
    if (!gpxm)
        return NULL;
    if (itTrkseg != itTrksegEnd)
        return &*itTrkseg++;
    return NULL;
}

int GPX_wrapper::initTrkptIteration(int trackNumber, int trackSegmentNumber) const
{
    if (!gpxm)
        return GPX_model::GPXM_ERR_FAILED;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            itTrkpt = gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.begin();
            itTrkptEnd = gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.end();
            return (int)gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size();
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

const GPX_wptType *GPX_wrapper::getNextTrkpt() const
{
    if (!gpxm)
        return NULL;
    if (itTrkpt != itTrkptEnd)
        return &*itTrkpt++;
    return NULL;
}

GPX_model::retCode_e GPX_wrapper::setSelectedTrack(int trackNumber, int trackSegmentNumber, bool forceSelect)
{
    if (trackNumber < -1)
        trackNumber = -1;
    if (trackSegmentNumber < -1)
        trackSegmentNumber = -1;

    if (!forceSelect)
    if (trackNumber == selectedTrackNumber && trackSegmentNumber == selectedTrackSegmentNumber)
        return GPX_model::GPXM_WARN_NOTHING_CHANGED;

    // clear selected point
    selectedPointNumber = -1;
    selectedPoint = NULL;

    if ((size_t)trackNumber < gpxm->trk.size())
    {
        selectedTrackNumber = trackNumber;
        if ((size_t)trackSegmentNumber < gpxm->trk[selectedTrackNumber].trkseg.size())
            selectedTrackSegmentNumber = trackSegmentNumber;
        else
            selectedTrackSegmentNumber = -1;
    }
    else
    {
        selectedTrackNumber = -1;
        selectedTrackSegmentNumber = -1;
    }
    return GPX_model::GPXM_OK;
}

GPX_model::retCode_e GPX_wrapper::setSelectedPointByNumber(int pointNumber, bool forceSelect)
{
    if (pointNumber < -1)
        pointNumber = -1;

    if (!forceSelect)
    if (pointNumber == selectedPointNumber)
        return GPX_model::GPXM_WARN_NOTHING_CHANGED;

    selectedPointNumber = -1;
    selectedPoint = NULL;
    if (pointNumber == -1)
        return GPX_model::GPXM_OK;

    if (selectedTrackNumber == -1)
    {
        return GPX_model::GPXM_ERR_FAILED;
    }
    else if (selectedTrackSegmentNumber == -1)
    {
        int n = pointNumber;
        vector<GPX_trksegType>::iterator trkseg;
        for (trkseg = gpxm->trk[selectedTrackNumber].trkseg.begin(); trkseg != gpxm->trk[selectedTrackNumber].trkseg.end(); ++trkseg)
        {
            if ((size_t)n >= trkseg->trkpt.size())
            {
                n -= (int)trkseg->trkpt.size();
            }
            else
            {
                selectedPointNumber = pointNumber;
                selectedPoint = &trkseg->trkpt[n];
                return GPX_model::GPXM_OK;
            }
        }
    }
    else
    {
        if ((size_t)pointNumber < gpxm->trk[selectedTrackNumber].trkseg[selectedTrackSegmentNumber].trkpt.size())
        {
            selectedPointNumber = pointNumber;
            selectedPoint = &gpxm->trk[selectedTrackNumber].trkseg[selectedTrackSegmentNumber].trkpt[selectedPointNumber];
            return GPX_model::GPXM_OK;
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

GPX_model::retCode_e GPX_wrapper::setSelectedPointByCoord(double lat, double lon, bool forceSelect)
{
    if (!forceSelect)
    if (selectedPoint)
    if (lat == selectedPoint->latitude && lon == selectedPoint->longitude)
        return GPX_model::GPXM_WARN_NOTHING_CHANGED;

    selectedPointNumber = -1;
    selectedPoint = NULL;
    if (selectedTrackNumber == -1)
    {
        return GPX_model::GPXM_ERR_FAILED;
    }
    else if (selectedTrackSegmentNumber == -1)
    {
        int number = 0;
        vector<GPX_trksegType>::iterator trkseg;
        vector<GPX_wptType>::iterator  trkpt;
        for (trkseg = gpxm->trk[selectedTrackNumber].trkseg.begin(); trkseg != gpxm->trk[selectedTrackNumber].trkseg.end(); ++trkseg)
        {
            for (trkpt = trkseg->trkpt.begin(); trkpt != trkseg->trkpt.end(); ++trkpt)
            {
                if (lat == trkpt->latitude && lon == trkpt->longitude)
                {
                    selectedPointNumber = number;
                    selectedPoint = &*trkpt;
                    return GPX_model::GPXM_OK;
                }
                number++;
            }
        }
    }
    else
    {
        int number = 0;
        vector<GPX_wptType>::iterator  trkpt;
        for (trkpt = gpxm->trk[selectedTrackNumber].trkseg[selectedTrackSegmentNumber].trkpt.begin(); trkpt != gpxm->trk[selectedTrackNumber].trkseg[selectedTrackSegmentNumber].trkpt.end(); ++trkpt)
        {
            if (lat == trkpt->latitude && lon == trkpt->longitude)
            {
                selectedPointNumber = number;
                selectedPoint = &*trkpt;
                return GPX_model::GPXM_OK;
            }
            number++;
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

GPX_model::retCode_e GPX_wrapper::setSelectedPointByTimestamp(time_t timestamp, bool forceSelect)
{
    if (!forceSelect)
    if (selectedPoint && timestamp == selectedPoint->timestamp)
        return GPX_model::GPXM_WARN_NOTHING_CHANGED;

    selectedPointNumber = -1;
    selectedPoint = NULL;
    if (selectedTrackNumber == -1)
    {
        return GPX_model::GPXM_ERR_FAILED;
    }
    else if (selectedTrackSegmentNumber == -1)
    {
        int number = 0;
        vector<GPX_trksegType>::iterator trkseg;
        vector<GPX_wptType>::iterator  trkpt;
        for (trkseg = gpxm->trk[selectedTrackNumber].trkseg.begin(); trkseg != gpxm->trk[selectedTrackNumber].trkseg.end(); ++trkseg)
        {
            for (trkpt = trkseg->trkpt.begin(); trkpt != trkseg->trkpt.end(); ++trkpt)
            {
                if (timestamp <= trkpt->timestamp)
                {
                    selectedPointNumber = number;
                    selectedPoint = &*trkpt;
                    return GPX_model::GPXM_OK;
                }
                number++;
            }
        }
    }
    else
    {
        int number = 0;
        vector<GPX_wptType>::iterator  trkpt;
        for (trkpt = gpxm->trk[selectedTrackNumber].trkseg[selectedTrackSegmentNumber].trkpt.begin(); trkpt != gpxm->trk[selectedTrackNumber].trkseg[selectedTrackSegmentNumber].trkpt.end(); ++trkpt)
        {
            if (timestamp <= trkpt->timestamp)
            {
                selectedPointNumber = number;
                selectedPoint = &*trkpt;
                return GPX_model::GPXM_OK;
            }
            number++;
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

void GPX_wrapper::updateTrackNumbers()
{
    size_t number = 0;
    vector<GPX_trkType>::iterator itTrk;
    for (itTrk = gpxm->trk.begin(); itTrk != gpxm->trk.end(); ++itTrk)
        itTrk->metadata.number = number++;
}
