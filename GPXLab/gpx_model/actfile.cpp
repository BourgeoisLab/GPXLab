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
#include "uxmlpars.h"
#include "utils.h"

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
        if (strcmp(pTag, "trackheader") == 0)
        {
            xml->state = PARSING_TRACKHEADER;
        }
        else if (strcmp(pTag, "tracklapmaster") == 0)
        {
            GPX_trksegType trkseg;
            gpxm->trk.back().trkseg.push_back(trkseg);
            xml->state = PARSING_TRACKLAP;
        }
        else if (strcmp(pTag, "trackmaster") == 0)
        {
            GPX_trksegType trkseg;
            gpxm->trk.back().trkseg.push_back(trkseg);
            xml->state = PARSING_TRACK;
        }
        else if (strcmp(pTag, "trackpoints") == 0 || strcmp(pTag, "tracklappoints") == 0)
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
        else if (strcmp(pTag, "tblnote") == 0)
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
        //if (strcmp(pTag, "trackid") == 0)
        //    gpxm->metadata.name = sContent;
        //else
        if (strcmp(pTag, "trackname") == 0)
        {
            getStartDateTime(1, pContent);
            gpxm->trk.back().metadata.name = sContent;
        }
        else if (strcmp(pTag, "starttime") == 0)
            gTime = getStartDateTime(2, pContent);
        //else if (strcmp(pTag, "duration") == 0 || strcmp(pTag, "during") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "totaldist") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "calories") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxspeed") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxhearrate") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "avgheartrate") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "noofpoints") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "nooflaps") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "aupheight") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "adownheight") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "avgcadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxcadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "avgpower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxpower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "minaltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxaltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "multisport") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "sport1") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "sport2") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "sport3") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "sport4") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "sport5") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapno") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "accruedtime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "totaltime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "totaldistance") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "calory") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapmaxspeed") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapmaxhr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapavghr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "startindex") == 0)
        //    gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        else if (strcmp(pTag, "endindex") == 0)
            gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        //else if (strcmp(pTag, "lapavgcad") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapmaxcad") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapavgpwr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapmaxpwr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapminalti") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapmaxalti") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "sporttype") == 0)
        //    gpxm->metadata.name = sContent;
        break;

    case PARSING_TRACKLAP:
        // if (strcmp(pTag, "trackmasterid") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "lapno") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "accruedtime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "totaltime") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "totaldistance") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "calory") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxspeed") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxhr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "avghr") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "startindex") == 0)
        //    gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        //else
        if (strcmp(pTag, "endindex") == 0)
            gSegementEndIndexes[gpxm->trk.back().trkseg.size() - 1] = atoi(pContent);
        //else if (strcmp(pTag, "avgcadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "bestcadence") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "avgpower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxpower") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "minaltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "maxaltitude") == 0)
        //    gpxm->metadata.name = sContent;
        //else if (strcmp(pTag, "multisportindex") == 0)
        //    gpxm->metadata.name = sContent;
        break;

    case PARSING_POINT:
        //if (strcmp(pTag, "slno") == 0 || strcmp(pTag, "sl_x0020_no") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else
        if (strcmp(pTag, "latitude") == 0)
            gTrkseg->trkpt.back().latitude = atof(pContent);
        else if (strcmp(pTag, "longitude") == 0)
            gTrkseg->trkpt.back().longitude = atof(pContent);
        else if (strcmp(pTag, "altitude") == 0)
            gTrkseg->trkpt.back().altitude = atof(pContent);
        else if (strcmp(pTag, "speed") == 0)
            gTrkseg->trkpt.back().speed = (float)atof(pContent);
        else if (strcmp(pTag, "heartrate") == 0 || strcmp(pTag, "heart_x0020_rate") == 0)
            gTrkseg->trkpt.back().extensionsGarmin.heartrate = atoi(pContent);
        else if (strcmp(pTag, "intervaltime") == 0 || strcmp(pTag, "interval_x0020_time") == 0)
        {
            gTime += atoi(pContent);
            gTrkseg->trkpt.back().timestamp = gTime;
        }
        //else if (strcmp(pTag, "index") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else if (strcmp(pTag, "cadence") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else if (strcmp(pTag, "pwrcadence") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        //else if (strcmp(pTag, "power") == 0)
        //    gTrkseg->trkpt.back().altitude = 0;
        break;

    case PARSING_NOTE:
        //if (strcmp(pTag, "column1") == 0)
        //    gpxm->trk.back().metadata.cmt += sContent;
        //else
        if (strcmp(pTag, "column2") == 0)
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
        if (strcmp(pTag, "trackheader") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_TRACKLAP:
        if (strcmp(pTag, "tracklapmaster") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_TRACK:
        if (strcmp(pTag, "trackmaster") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_POINT:
        if (strcmp(pTag, "trackpoints") == 0 || strcmp(pTag, "tracklappoints") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_NOTE:
        if (strcmp(pTag, "tblnote") == 0)
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

    // set timezone temporary to UTC
    char *tz = getenv("TZ");
    UTILS_setenv("TZ", "UTC");
    tzset();

    // parse file
    if (UXML_parseFile(&uXML) != 0)
        return GPX_model::GPXM_ERR_FAILED;

    // change back timezone
    if (tz)
        UTILS_setenv("TZ", tz);
    else
        UTILS_unsetenv("TZ");
    tzset();

    // update track
    gpxm->updateTrack(gpxm->trk.back());

    return GPX_model::GPXM_OK;
}
