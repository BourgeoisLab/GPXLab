/****************************************************************************
 *   Copyright (c) 2014 Frederic Bourgeois <bourgeoislab@gmail.com>         *
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

#include "gpx_wrapper.h"

GPX_wrapper::GPX_wrapper(const QString &creator) :
    modified(false),
    selectedPointNumber(-1),
    selectedPoint(NULL)
{
    gpxm = new GPX_model(creator.toStdString());
    setSelectedTrack(-1, -1);
}

GPX_wrapper::~GPX_wrapper()
{
    delete gpxm;
}

GPX_model::retCode_e GPX_wrapper::load(const QString &fileName, bool overwriteMetadata)
{
    GPX_model::retCode_e ret = gpxm->load(fileName.toStdString(), overwriteMetadata);
    if (ret != GPX_model::GPXM_ERR_FILEOPEN && ret != GPX_model::GPXM_ERR_INVALID_ARG)
    {
        if (overwriteMetadata)
        {
            if (!this->fileName.isEmpty())
                modified = true;
            this->fileName = fileName;
        }
        else
        {
            modified = true;
        }
    }
    return ret;
}

GPX_model::retCode_e GPX_wrapper::save(const QString &fileName)
{
    GPX_model::retCode_e ret = gpxm->save(fileName.toStdString());
    if (ret != GPX_model::GPXM_ERR_FILEOPEN && ret != GPX_model::GPXM_ERR_INVALID_ARG)
    {
        modified = false;
        this->fileName = fileName;
    }
    return ret;
}

const QString &GPX_wrapper::getFileName() const
{
    return fileName;
}

bool GPX_wrapper::isModified() const
{
    return modified;
}

const GPX_statsType* GPX_wrapper::getModelStats() const
{
    return &gpxm->stats;
}

const GPX_metadataType* GPX_wrapper::getModelMetadata() const
{
    return &gpxm->metadata;
}

GPX_model::retCode_e GPX_wrapper::setModelMetadata(const GPX_metadataType &metadata)
{
    gpxm->metadata = metadata;
    modified = true;
    return GPX_model::GPXM_OK;
}

int GPX_wrapper::getNumTracks() const
{
    return gpxm->trk.size();
}

const GPX_statsType* GPX_wrapper::getTrackStats(int trackNumber) const
{
    if ((size_t)trackNumber < gpxm->trk.size())
        return &gpxm->trk[trackNumber].stats;
    return NULL;
}

const GPX_trkMetadataType* GPX_wrapper::getTrackMetadata(int trackNumber) const
{
    if ((size_t)trackNumber < gpxm->trk.size())
        return &gpxm->trk[trackNumber].metadata;
    return NULL;
}

GPX_model::retCode_e GPX_wrapper::setTrackMetadata(int trackNumber, const GPX_trkMetadataType &metadata)
{
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        gpxm->trk[trackNumber].metadata = metadata;
        modified = true;
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

const GPX_statsType* GPX_wrapper::getItemStats(int trackNumber, int trackSegmentNumber) const
{
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
    if ((size_t)trackNumber < gpxm->trk.size())
        return &gpxm->trk[trackNumber];
    return NULL;
}

const GPX_trksegType *GPX_wrapper::getTrackSegment(int trackNumber, int trackSegmentNumber) const
{
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

int GPX_wrapper::getNumPoints(int trackNumber, int trackSegmentNumber) const
{
    int numPoints = 0;
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if (trackSegmentNumber == -1)
        {
            vector<GPX_trksegType>::iterator trkseg;
            for (trkseg = gpxm->trk[trackNumber].trkseg.begin(); trkseg != gpxm->trk[trackNumber].trkseg.end(); ++trkseg)
                numPoints += trkseg->trkpt.size();
        }
        else if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            numPoints = gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size();
        }
    }
    return numPoints;
}

GPX_model::retCode_e GPX_wrapper::removeTrack(int trackNumber)
{
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        gpxm->trk.erase(gpxm->trk.begin() + trackNumber);
        if ((size_t)selectedTrackNumber >= gpxm->trk.size())
            selectedTrackNumber = (int)gpxm->trk.size() - 1;
        gpxm->update(false);
        modified = true;
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

GPX_model::retCode_e GPX_wrapper::insertTrack(int position, const GPX_trkType &trk)
{
    if ((size_t)position < gpxm->trk.size())
    {
        gpxm->trk.insert(gpxm->trk.begin() + position, trk);
    }
    else
    {
        gpxm->trk.insert(gpxm->trk.end(), trk);
    }
    gpxm->update(false);
    modified = true;
    return GPX_model::GPXM_OK;
}

GPX_model::retCode_e GPX_wrapper::moveTrackUp(int trackNumber)
{
    if ((size_t)trackNumber < gpxm->trk.size() && trackNumber > 0)
    {
        iter_swap(gpxm->trk.begin() + trackNumber, gpxm->trk.begin() + trackNumber - 1);
        modified = true;
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::moveTrackDown(int trackNumber)
{
    if ((size_t)trackNumber < gpxm->trk.size() - 1)
    {
        iter_swap(gpxm->trk.begin() + trackNumber, gpxm->trk.begin() + trackNumber + 1);
        modified = true;
        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::splitTrack(int trackNumber, int &trackSegmentNumber, int &pointNumber)
{
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
            setSelectedTrack(trackNumber, trackSegmentNumber);
            setSelectedPointByNumber(pointNumber);

            modified = true;
            return GPX_model::GPXM_OK;
        }
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::combineTrack(int trackNumber, int &trackSegmentNumber, int &pointNumber)
{
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
                    pointNumber += (itTrkSeg-1)->trkpt.size();

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
                    setSelectedTrack(trackNumber, trackSegmentNumber);
                    setSelectedPointByNumber(pointNumber);
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
                    setSelectedTrack(trackNumber, trackSegmentNumber);
                    setSelectedPointByNumber(0);
                    setSelectedPointByNumber(pointNumber);
                }

                // update track
                gpxm->updateTrack(gpxm->trk[trackNumber]);

                modified = true;
                return GPX_model::GPXM_OK;
            }
        }
    }
    return GPX_model::GPXM_ERR_INVALID_ARG;
}

GPX_model::retCode_e GPX_wrapper::setSelectedTrack(int trackNumber, int trackSegmentNumber)
{
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

int GPX_wrapper::getSelectedTrackNumber() const
{
    return selectedTrackNumber;
}

int GPX_wrapper::getSelectedTrackSegmentNumber() const
{
    return selectedTrackSegmentNumber;
}

GPX_model::retCode_e GPX_wrapper::setSelectedPointByNumber(int pointNumber)
{
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

GPX_model::retCode_e GPX_wrapper::setSelectedPointByCoord(double lat, double lon)
{
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

GPX_model::retCode_e GPX_wrapper::setSelectedPointByTimestamp(time_t timestamp)
{
    if (selectedPoint)
    if (timestamp == selectedPoint->timestamp)
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

const GPX_wptType *GPX_wrapper::getSelectedPoint() const
{
    return selectedPoint;
}

int GPX_wrapper::getSelectedPointNumber() const
{
    return selectedPointNumber;
}

GPX_model::retCode_e GPX_wrapper::generateDiagramValues(int trackNumber, int trackSegmentNumber)
{
    // clear old values
    timeValues.clear();
    speedValues.clear();
    altitudeValues.clear();

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
                    timeValues.append(trkpt->timestamp);
                    speedValues.append(trkpt->speed);
                    altitudeValues.append(trkpt->altitude);
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
                    timeValues.append(trkpt->timestamp);
                    speedValues.append(trkpt->speed);
                    altitudeValues.append(trkpt->altitude);
                }
                return GPX_model::GPXM_OK;
            }
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

const QVector<double>& GPX_wrapper::getDiagramTimeValues() const
{
    return timeValues;
}

const QVector<double>& GPX_wrapper::getDiagramAltitudeValues() const
{
    return altitudeValues;
}

const QVector<double>& GPX_wrapper::getDiagramSpeedValues() const
{
    return speedValues;
}

GPX_model::retCode_e GPX_wrapper::setAltitudeValues(const QVector<double> &values, int trackNumber, int trackSegmentNumber)
{
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

        modified = true;

        return GPX_model::GPXM_OK;
    }
    return GPX_model::GPXM_ERR_FAILED;
}

int GPX_wrapper::initTrkIteration() const
{
    iterator_trk = gpxm->trk.begin();
    return gpxm->trk.size();
}

const GPX_trkType *GPX_wrapper::getNextTrk() const
{
    const GPX_trkType* trk = &*iterator_trk;
    if (iterator_trk == gpxm->trk.end())
        return NULL;
    iterator_trk++;
    return trk;
}

int GPX_wrapper::initTrksegIteration(int trackNumber) const
{
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        iterator_trkseg = gpxm->trk[trackNumber].trkseg.begin();
        return gpxm->trk[trackNumber].trkseg.size();
    }
    return GPX_model::GPXM_ERR_FAILED;
}

const GPX_trksegType* GPX_wrapper::getNextTrkseg(int trackNumber) const
{
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        const GPX_trksegType* trkseg = &*iterator_trkseg;
        if (iterator_trkseg == gpxm->trk[trackNumber].trkseg.end())
            return NULL;
        iterator_trkseg++;
        return trkseg;
    }
    return NULL;
}

int GPX_wrapper::initWptIteration(int trackNumber, int trackSegmentNumber) const
{
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            iterator_wpt = gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.begin();
            return gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.size();
        }
    }
    return GPX_model::GPXM_ERR_FAILED;
}

const GPX_wptType *GPX_wrapper::getNextWpt(int trackNumber, int trackSegmentNumber) const
{
    if ((size_t)trackNumber < gpxm->trk.size())
    {
        if ((size_t)trackSegmentNumber < gpxm->trk[trackNumber].trkseg.size())
        {
            const GPX_wptType* wpt = &*iterator_wpt;
            if (iterator_wpt == gpxm->trk[trackNumber].trkseg[trackSegmentNumber].trkpt.end())
                return NULL;
            iterator_wpt++;
            return wpt;
        }
    }
    return NULL;
}
