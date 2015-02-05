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

#include <string.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include "gpx_model.h"
#include "nmeafile.h"
#include "gpxfile.h"
#include "actfile.h"

////////////////////////////////////////////////////////////////////////////////

GPX_model::fileType_e GPX_model::getFileType(const string& fileName)
{
    string ext;
    size_t dotPos;

    dotPos = fileName.find_last_of(".");
    if (dotPos == string::npos)
        ext = "";
    else
        ext = fileName.substr(dotPos + 1);

    for (size_t i = 0; i < ext.length(); ++i)
        ext[i] = tolower(ext[i]);

    if (ext == "gpx")
        return GPXM_FILE_GPX;
    if (ext == "txt" || ext == "nmea")
        return GPXM_FILE_NMEA;
    if (ext == "act")
        return GPXM_FILE_ACT;
    return GPXM_FILE_NOT_SUPPORTED;
}

GPX_model::GPX_model(const string& creator) :
    creator(creator)
{
    clearMetadata();
    memset(&stats, 0, sizeof(GPX_statsType));
}

GPX_model::GPX_model(const string& creator, const string& fileName) :
    creator(creator)
{
    clearMetadata();
    memset(&stats, 0, sizeof(GPX_statsType));
    load(fileName, GPXM_FILE_AUTOMATIC, true);
}

void GPX_model::clearMetadata()
{
    metadata.name.clear();
    metadata.desc.clear();
    metadata.author.name.clear();
    metadata.author.email.id.clear();
    metadata.author.email.domain.clear();
    metadata.author.link.href.clear();
    metadata.author.link.text.clear();
    metadata.author.link.type.clear();
    metadata.copyright.author.clear();
    metadata.copyright.year.clear();
    metadata.copyright.license.clear();
    metadata.links.clear();
    metadata.timestamp = 0;
    metadata.millisecond = 0;
    metadata.keywords.clear();
    metadata.bounds.minlat = 0.0;
    metadata.bounds.minlon = 0.0;
    metadata.bounds.maxlat = 0.0;
    metadata.bounds.maxlon = 0.0;
    metadata.extensions.extension.clear();
}

GPX_model::retCode_e GPX_model::load(const string& fileName, fileType_e fileType, bool overwriteMetadata)
{
    ifstream fp;
    string name;
    size_t dotPos, slashPos;
    retCode_e ret = GPXM_OK;

    // get file type
    if (fileType == GPXM_FILE_AUTOMATIC)
        fileType = getFileType(fileName);
    if (fileType == GPXM_FILE_NOT_SUPPORTED)
        return GPXM_ERR_INVALID_ARG;

    // get name without extension and path
    dotPos = fileName.find_last_of(".");
    if (dotPos == string::npos)
        dotPos = fileName.length();
    slashPos = fileName.find_last_of("\\/");
    if (slashPos == string::npos)
        slashPos = 0;
    else
        slashPos++;
    name = fileName.substr(slashPos, dotPos);

    // open file for read
    fp.open(fileName.c_str());
    if (!fp)
        return GPXM_ERR_FILEOPEN;

    // clear metadata
    if (overwriteMetadata)
    {
        clearMetadata();
        metadata.name = name;
    }

    // parse file
    if (fileType == GPXM_FILE_GPX)
    {
        ret = GPXFile::load(&fp, this, overwriteMetadata);
    }
    else if (fileType == GPXM_FILE_NMEA)
    {
        ret = NMEAFile::load(&fp, this, name);
    }
    else if (fileType == GPXM_FILE_ACT)
    {
        ret = ACTFile::load(&fp, this);
    }

    // close file
    fp.close();

    // update model metadata and statistic
    if (ret == GPXM_OK)
    {
        update(false);
    }

    return ret;
}

GPX_model::retCode_e GPX_model::save(const string& fileName)
{
    ofstream fp;
    fileType_e fileType;
    string name;
    size_t dotPos, slashPos;
    retCode_e ret = GPXM_OK;

    // get file type
    fileType = getFileType(fileName);
    if (fileType != GPXM_FILE_GPX)
        return GPXM_ERR_INVALID_ARG;

    // get name without extension and path
    dotPos = fileName.find_last_of(".");
    if (dotPos == string::npos)
        dotPos = fileName.length();
    slashPos = fileName.find_last_of("\\/");
    if (slashPos == string::npos)
        slashPos = 0;
    else
        slashPos++;
    name = fileName.substr(slashPos);

    // open file for write
    fp.open(fileName.c_str());
    if (!fp)
        return GPXM_ERR_FILEOPEN;

    metadata.name = name;
    metadata.timestamp = time(NULL);
    metadata.millisecond = 0;

    // write file
    ret = GPXFile::save(&fp, this);

    // close file
    fp.close();

    return ret;
}

void GPX_model::update(bool propagate)
{
    // reset values
    memset(&stats, 0, sizeof(GPX_statsType));

    if (!trk.empty())
    {
        int n = 0;
        vector<GPX_trkType>::iterator itrk;
        for (itrk = trk.begin(); itrk != trk.end(); ++itrk)
        {
            // update track
            if (propagate)
            {
                itrk->update(true);
                itrk->metadata.number = n++;
            }

            // update self
            if (itrk == trk.begin())
            {
                stats.bounds.minlat = itrk->stats.bounds.minlat;
                stats.bounds.maxlat = itrk->stats.bounds.maxlat;
                stats.bounds.minlon = itrk->stats.bounds.minlon;
                stats.bounds.maxlon = itrk->stats.bounds.maxlon;
                stats.startTime = itrk->stats.startTime;
                stats.endTime = itrk->stats.endTime;
                stats.minhei = itrk->stats.minhei;
                stats.maxhei = itrk->stats.maxhei;
            }
            else
            {
                if (itrk->stats.bounds.minlat < stats.bounds.minlat)
                    stats.bounds.minlat = itrk->stats.bounds.minlat;
                if (itrk->stats.bounds.maxlat > stats.bounds.maxlat)
                    stats.bounds.maxlat = itrk->stats.bounds.maxlat;
                if (itrk->stats.bounds.minlon < stats.bounds.minlon)
                    stats.bounds.minlon = itrk->stats.bounds.minlon;
                if (itrk->stats.bounds.maxlon > stats.bounds.maxlon)
                    stats.bounds.maxlon = itrk->stats.bounds.maxlon;
                if (itrk->stats.startTime < stats.startTime)
                    stats.startTime = itrk->stats.startTime;
                if (itrk->stats.endTime > stats.endTime)
                    stats.endTime = itrk->stats.endTime;
                if (itrk->stats.minhei < stats.minhei)
                    stats.minhei = itrk->stats.minhei;
                if (itrk->stats.maxhei > stats.maxhei)
                    stats.maxhei = itrk->stats.maxhei;
            }
            stats.points += itrk->stats.points;
            stats.duration += itrk->stats.duration;
            stats.distance += itrk->stats.distance;
            stats.heightIntUp += itrk->stats.heightIntUp;
            stats.heightIntDown += itrk->stats.heightIntDown;
        }
        stats.speed = (float)((stats.distance / stats.duration) * 3600.0f);
        metadata.bounds = stats.bounds;
    }
}

void GPX_model::updateTrack(GPX_trkType &track, bool propagate)
{
    track.update(propagate);
}

////////////////////////////////////////////////////////////////////////////////

GPX_trkType::GPX_trkType(size_t number)
{
    memset(&stats, 0, sizeof(GPX_statsType));
    metadata.number = number;
}

void GPX_trkType::update(bool propagate)
{
    // reset values
    memset(&stats, 0, sizeof(GPX_statsType));

    if (!trkseg.empty())
    {
        time_t durationBetweenSegment = 0;
        vector<GPX_trksegType>::iterator itrkseg;
        for (itrkseg = trkseg.begin(); itrkseg != trkseg.end(); ++itrkseg)
        {
            if (itrkseg == trkseg.begin())
            {
                // update track segment
                if (propagate)
                    itrkseg->update(propagate, 0, NULL);

                // update self
                stats.bounds.minlat = itrkseg->stats.bounds.minlat;
                stats.bounds.maxlat = itrkseg->stats.bounds.maxlat;
                stats.bounds.minlon = itrkseg->stats.bounds.minlon;
                stats.bounds.maxlon = itrkseg->stats.bounds.maxlon;
                stats.startTime = itrkseg->stats.startTime;
                stats.minhei = itrkseg->stats.minhei;
                stats.maxhei = itrkseg->stats.maxhei;
            }
            else
            {
                // update track segment
                if (propagate)
                    itrkseg->update(propagate, stats.startTime, &(itrkseg-1)->trkpt.back());

                // update self
                if (itrkseg->stats.bounds.minlat < stats.bounds.minlat)
                    stats.bounds.minlat = itrkseg->stats.bounds.minlat;
                if (itrkseg->stats.bounds.maxlat > stats.bounds.maxlat)
                    stats.bounds.maxlat = itrkseg->stats.bounds.maxlat;
                if (itrkseg->stats.bounds.minlon < stats.bounds.minlon)
                    stats.bounds.minlon = itrkseg->stats.bounds.minlon;
                if (itrkseg->stats.bounds.maxlon > stats.bounds.maxlon)
                    stats.bounds.maxlon = itrkseg->stats.bounds.maxlon;
                if (itrkseg->stats.minhei < stats.minhei)
                    stats.minhei = itrkseg->stats.minhei;
                if (itrkseg->stats.maxhei > stats.maxhei)
                    stats.maxhei = itrkseg->stats.maxhei;
                durationBetweenSegment = itrkseg->stats.startTime - durationBetweenSegment;
            }
            stats.points += itrkseg->stats.points;
            stats.duration += itrkseg->stats.duration + (unsigned int)durationBetweenSegment;
            stats.distance += itrkseg->stats.distance;
            stats.heightIntUp += itrkseg->stats.heightIntUp;
            stats.heightIntDown += itrkseg->stats.heightIntDown;
            durationBetweenSegment = itrkseg->stats.endTime;
        }

        itrkseg--;
        stats.endTime = itrkseg->stats.endTime;
        stats.speed = (float)((stats.distance / stats.duration) * 3600.0f);
    }
}

////////////////////////////////////////////////////////////////////////////////

GPX_trksegType::GPX_trksegType()
{
    memset(&stats, 0, sizeof(GPX_statsType));
}

void GPX_trksegType::update(bool propagate, time_t trackStartTime, const GPX_wptType* prevWpt)
{
    // reset values
    memset(&stats, 0, sizeof(GPX_statsType));

    if (!trkpt.empty())
    {
        const GPX_wptType* nextWpt = NULL;
        float lasthei = 0.0f, deltahei;
        vector<GPX_wptType>::iterator itrkpt;
        stats.points = (int)trkpt.size();
        for (itrkpt = trkpt.begin(); itrkpt != trkpt.end(); ++itrkpt)
        {
            if ((itrkpt + 1) == trkpt.end())
                nextWpt = NULL;
            else
                nextWpt = &*(itrkpt + 1);

            if (itrkpt == trkpt.begin())
            {
                // update track point
                if (propagate)
                    itrkpt->update(trackStartTime, prevWpt, nextWpt);

                // update self
                if (prevWpt == NULL)
                    trackStartTime = itrkpt->timestamp;
                stats.bounds.minlat = itrkpt->latitude;
                stats.bounds.maxlat = itrkpt->latitude;
                stats.bounds.minlon = itrkpt->longitude;
                stats.bounds.maxlon = itrkpt->longitude;
                stats.maxhei = (float)itrkpt->altitude;
                stats.minhei = (float)itrkpt->altitude;
                stats.startTime = itrkpt->timestamp;
                lasthei = (float)itrkpt->altitude;
            }
            else
            {
                // update track point
                if (propagate)
                    itrkpt->update(trackStartTime, prevWpt, nextWpt);

                // update self
                if (itrkpt->latitude < stats.bounds.minlat)
                    stats.bounds.minlat = itrkpt->latitude;
                if (itrkpt->latitude > stats.bounds.maxlat)
                    stats.bounds.maxlat = itrkpt->latitude;
                if (itrkpt->longitude < stats.bounds.minlon)
                    stats.bounds.minlon = itrkpt->longitude;
                if (itrkpt->longitude > stats.bounds.maxlon)
                    stats.bounds.maxlon = itrkpt->longitude;
                stats.distance += itrkpt->leglength/1000;
                deltahei = (float)itrkpt->altitude - lasthei;
                lasthei = (float)itrkpt->altitude;
                if (deltahei > 0)
                    stats.heightIntUp += deltahei;
                else
                    stats.heightIntDown -= deltahei;
                if (itrkpt->altitude < stats.minhei)
                    stats.minhei = (float)itrkpt->altitude;
                if (itrkpt->altitude > stats.maxhei)
                    stats.maxhei = (float)itrkpt->altitude;
            }

            prevWpt = &*(itrkpt);
        }

        itrkpt--;
        stats.endTime = itrkpt->timestamp;
        stats.duration = (unsigned int)(stats.endTime - stats.startTime);
        stats.speed = (float)((stats.distance / stats.duration) * 3600.0f);
    }
}

////////////////////////////////////////////////////////////////////////////////

GPX_wptType::GPX_wptType()
{
    clear();
}

void GPX_wptType::clear()
{
    fix.clear();
    sat = 0;
    timestamp = 0;
    millisecond = 0;
    magvar = 0.0f;
    altitude = 0.0;
    hdop = 0.0f;
    vdop = 0.0f;
    pdop = 0.0f;
    ageofdgpsdata = 0.0f;
    dgpsid = 0;
    geoidheight = 0.0f;
    latitude = 0.0;
    longitude = 0.0;
    name.clear();
    cmt.clear();
    desc.clear(); 
    src.clear();
    links.clear(); 
    sym.clear();
    type.clear();
    extensions.extension.clear();
    speed = 0.0f;
    heading = 0.0f;
    leglength = 0.0;
    distanceTot = 0.0;
    elapsedTime = 0;
    extensionsGarmin.heartrate = 0;
}

void GPX_wptType::update(time_t trackStartTime, const GPX_wptType* prevWpt, const GPX_wptType *nextWpt)
{
    if (prevWpt)
    {
        unsigned int deltaT = (unsigned int)(timestamp - prevWpt->timestamp) * 1000;
        deltaT += (millisecond - prevWpt->millisecond);
        setDistance(prevWpt->latitude, prevWpt->longitude);
        speed = (float)(leglength / deltaT)*3600.0f;
        distanceTot = leglength/1000 + prevWpt->distanceTot;
        elapsedTime = (unsigned int)(timestamp - trackStartTime);
    }
    else
    {
        leglength = 0.0;
        distanceTot = 0.0;
        speed = 0.0;
        elapsedTime = 0;
    }

    if (nextWpt)
    {
        setHeading(nextWpt->latitude, nextWpt->longitude);
    }
    else
    {
        heading = 0.0f;
    }
}

double GPX_wptType::getTime() const
{
    return timestamp + (double)millisecond/1000;
}

bool GPX_wptType::isSameTime(const GPX_wptType &wptRef) const
{
    if (timestamp != wptRef.timestamp)
        return false;
    if (millisecond != wptRef.millisecond)
        return false;
    return true;
}

void GPX_wptType::setDistance(double latitudeFrom, double longitudeFrom)
{
    const double Deg2Rad = 0.01745329252;
    const double EarthRadius = 6372795;
    double latitudeArc = (latitude - latitudeFrom) * Deg2Rad;
    double longitudeArc = (longitude - longitudeFrom) * Deg2Rad;
    double latitudeH = sin(latitudeArc / 2.0f);
    double lontitudeH = sin(longitudeArc / 2.0f);
    double tmp;
    latitudeH *= latitudeH;
    lontitudeH *= lontitudeH;
    tmp = latitudeH + cos(latitude * Deg2Rad) * cos(latitudeFrom * Deg2Rad) * lontitudeH;
    leglength = 2.0f * asin(sqrt(tmp)) * EarthRadius;
}

void GPX_wptType::setHeading(double latitudeTo, double longitudeTo)
{
    const double Deg2Rad = 0.01745329252;
    double lonArc = (longitudeTo - longitude) * Deg2Rad;
    double latARad = latitude * Deg2Rad;
    double latBRad = latitudeTo * Deg2Rad;
    double cosLatB = cos(latBRad);
    double y = sin(lonArc) * cosLatB;
    double x = cos(latARad)*sin(latBRad) - sin(latARad)*cosLatB*cos(lonArc);
    heading = (float)(atan2(y, x) / Deg2Rad);
    if (heading < 0.0f)
        heading += 360.0f;
}

const string& GPX_wptType::cardinal() const
{
    static const string directions[16] =
      { "N", "NNE", "NE", "ENE",
        "E", "ESE", "SE", "SSE",
        "S", "SSW", "SW", "WSW",
        "W", "WNW", "NW", "NNW" };
    int direction = (int)((heading + 11.25f) / 22.5f);
    return directions[direction % 16];
}
