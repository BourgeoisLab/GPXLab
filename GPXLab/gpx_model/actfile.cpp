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

#include <stdlib.h>
#include <string.h>
#include <string>
#include <time.h>
#include "actfile.h"

#if !defined(_WIN32) && !defined(_WIN64)
// stricmp is Windows-specific, strcasecmp is POSIX-specific, both are not C stndard
  #define stricmp strcasecmp
#endif

extern "C" {
#include "uxmlpars.h"
}

#define BUFFER_SIZE     1024
#define MAX_SEGMENTS    1024

static char gBuffer[BUFFER_SIZE];

////////////////////////////////////////////////////////////////////////////////

#define PARSING_NONE                    0
#define PARSING_TRACKHEADER             1
#define PARSING_TRACKLAP                2
#define PARSING_TRACK                   3
#define PARSING_POINT                   4
#define PARSING_NOTE                    5

static time_t gTime = 0;
static GPX_trksegType *gTrkseg = NULL;
static unsigned int gPointIndex = 0;
static unsigned int gSegementEndIndexes[MAX_SEGMENTS];

static time_t getStartDateTime(int action, char *pVal)
{
    static struct tm timeinfo;
    time_t retVal = 0;
    switch (action)
    {
    case 0:
        memset(&timeinfo, 0, sizeof(struct tm));
        break;
    case 1:
        sscanf(pVal, "%d-%d-%d", &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday);
        timeinfo.tm_year -= 1900;
        --timeinfo.tm_mon;
        break;
    case 2:
        sscanf(pVal, "%d:%d:%d", &timeinfo.tm_hour, &timeinfo.tm_min, &timeinfo.tm_sec);
        timeinfo.tm_isdst = 0;
        retVal = mktime(&timeinfo);
        if (timeinfo.tm_isdst == 1)
            retVal -= 3600;
    }
    return retVal;
}

static int getChar(void* ptr)
{
    return ((ifstream*)((T_uXml*)ptr)->fp)->get();
}

static void openTag(void* pXml, char* pTag)
{
    T_uXml* xml = (T_uXml*)pXml;
    GPX_model *gpxm = (GPX_model*)xml->pObject;

    switch (xml->state)
    {
    case PARSING_NONE:
        if (stricmp(pTag, "trackHeader") == 0)
        {
            xml->state = PARSING_TRACKHEADER;
        }
        else if (stricmp(pTag, "trackLapMaster") == 0)
        {
            GPX_trksegType trkseg;
            gpxm->trk.back().trkseg.push_back(trkseg);
            xml->state = PARSING_TRACKLAP;
        }
        else if (stricmp(pTag, "trackmaster") == 0)
        {
            GPX_trksegType trkseg;
            gpxm->trk.back().trkseg.push_back(trkseg);
            xml->state = PARSING_TRACK;
        }
        else if (stricmp(pTag, "trackpoints") == 0 || stricmp(pTag, "trackLapPoints") == 0)
        {
            size_t i;
            GPX_wptType wpt;
            
            for (i = 0; i < MAX_SEGMENTS; ++i)
            {
                if (gPointIndex <= gSegementEndIndexes[i])
                    break;
            }
            ++gPointIndex;

            if (i < gpxm->trk.back().trkseg.size())
                gTrkseg = &gpxm->trk.back().trkseg.at(i);
            gTrkseg->trkpt.push_back(wpt);

            xml->state = PARSING_POINT;
        }
        else if (stricmp(pTag, "tblnote") == 0)
        {
            xml->state = PARSING_NOTE;
        }
        break;
    }
}

static void tagContent(void* pXml, char* pTag, char* pContent)
{
    T_uXml* xml = (T_uXml*)pXml;
    GPX_model *gpxm = (GPX_model*)xml->pObject;
    string sContent = pContent;

    switch (xml->state)
    {       
    case PARSING_TRACKHEADER:
    case PARSING_TRACK:
        //if (stricmp(pTag, "TrackID") == 0)
        //    gpxm->metadata.name = sContent;
        //else
        if (stricmp(pTag, "TrackName") == 0)
        {
            getStartDateTime(1, pContent);
            gpxm->trk.back().metadata.name = sContent;
        }
        else if (stricmp(pTag, "StartTime") == 0)
            gTime = getStartDateTime(2, pContent);
        //else if (stricmp(pTag, "Duration") == 0 || stricmp(pTag, "During") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "TotalDist") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Calories") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxSpeed") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxHearRate") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AvgHeartRate") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "NoOfPoints") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "NoOfLaps") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AUpheight") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "ADownheight") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AvgCadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxCadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AvgPower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxPower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MinAltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxAltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MultiSport") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Sport1") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Sport2") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Sport3") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Sport4") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Sport5") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapNo") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AccruedTime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "TotalTime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "TotalDistance") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Calory") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapMaxSpeed") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapMaxHR") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapAvgHR") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "startIndex") == 0)
        //    gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        else if (stricmp(pTag, "endIndex") == 0)
            gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        //else if (stricmp(pTag, "LapAvgCad") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapMaxCad") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapAvgPwr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapMaxPwr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapMinAlti") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapMaxAlti") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "SportType") == 0)
        //    gpxm->metadata.name = sContent;
        break;

    case PARSING_TRACKLAP:
        // if (stricmp(pTag, "TrackMasterID") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "LapNo") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AccruedTime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "TotalTime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "TotalDistance") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "Calory") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxSpeed") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxHR") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AvgHR") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "startIndex") == 0)
        //    gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        //else
        if (stricmp(pTag, "endIndex") == 0)
            gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        //else if (stricmp(pTag, "AvgCadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "BestCadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "AvgPower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxPower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MinAltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MaxAltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (stricmp(pTag, "MultiSportIndex") == 0)
        //    gpxm->metadata.name = sContent;
        break;

    case PARSING_POINT:
        //if (stricmp(pTag, "SLNo") == 0 || stricmp(pTag, "Sl_x0020_No") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else
        if (stricmp(pTag, "Latitude") == 0)
            gTrkseg->trkpt.back().latitude = atof(pContent);
        else if (stricmp(pTag, "Longitude") == 0)
            gTrkseg->trkpt.back().longitude = atof(pContent);
        else if (stricmp(pTag, "Altitude") == 0)
            gTrkseg->trkpt.back().altitude = atof(pContent);
        else if (stricmp(pTag, "Speed") == 0)
            gTrkseg->trkpt.back().speed = (float)atof(pContent);
        else if (stricmp(pTag, "HeartRate") == 0 || stricmp(pTag, "Heart_x0020_Rate") == 0)
            gTrkseg->trkpt.back().extensionsGarmin.heartrate = atoi(pContent);
        else if (stricmp(pTag, "IntervalTime") == 0 || stricmp(pTag, "Interval_x0020_Time") == 0)
        {
            gTime += atoi(pContent);
            gTrkseg->trkpt.back().timestamp = gTime;
        }
        //else if (stricmp(pTag, "Index") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else if (stricmp(pTag, "Cadence") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else if (stricmp(pTag, "PwrCadence") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else if (stricmp(pTag, "Power") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        break;

    case PARSING_NOTE:
        //if (stricmp(pTag, "Column1") == 0)
        //    gpxm->trk.back().metadata.cmt += sContent;
        //else
        if (stricmp(pTag, "Column2") == 0)
            gpxm->trk.back().metadata.cmt += sContent;
        break;
    }
}

static void closeTag(void* pXml, char* pTag)
{
    T_uXml* xml = (T_uXml*)pXml;

    switch (xml->state)
    {
    case PARSING_TRACKHEADER:
        if (stricmp(pTag, "trackHeader") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_TRACKLAP:
        if (stricmp(pTag, "trackLapMaster") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_TRACK:
        if (stricmp(pTag, "trackmaster") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_POINT:
        if (stricmp(pTag, "trackpoints") == 0 || stricmp(pTag, "trackLapPoints") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_NOTE:
        if (stricmp(pTag, "tblnote") == 0)
            xml->state = PARSING_NONE;
        break;
    }
}

GPX_model::retCode_e ACTFile::load(ifstream* fp, GPX_model* gpxm)
{
    GPX_trkType trk(gpxm->trk.size());

    T_uXml uXML;

    UXML_init(&uXML);
    uXML.fp = fp;
    uXML.getChar = getChar;
    uXML.pContent = gBuffer;
    uXML.maxContent = BUFFER_SIZE;
    uXML.openTag = openTag;
    uXML.closeTag = closeTag;
    uXML.setContent = tagContent;
    uXML.state = PARSING_NONE;
    uXML.pObject = gpxm;

    gTime = getStartDateTime(0, NULL);
    gTrkseg = NULL;
    gPointIndex = 0;
    memset(gSegementEndIndexes, 0, sizeof(unsigned int) * MAX_SEGMENTS);

    // add track
    gpxm->trk.push_back(trk);

    // parse file
    if (UXML_parseFile(&uXML) != 0)
        return GPX_model::GPXM_ERR_FAILED;

    // update track
    gpxm->updateTrack(gpxm->trk.back());

    return GPX_model::GPXM_OK;
}
