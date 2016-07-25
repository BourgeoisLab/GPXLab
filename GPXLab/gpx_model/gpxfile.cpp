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
#include "gpxfile.h"

#if !defined(_WIN32) && !defined(_WIN64)
// stricmp is Windows-specific, strcasecmp is POSIX-specific, both are not C stndard
  #define stricmp strcasecmp
#endif



extern "C" {
#include "uxmlpars.h"
}

#define BUFFER_SIZE                     1024
#define DEPTH_INDENT                    2

static char gBuffer[BUFFER_SIZE];

////////////////////////////////////////////////////////////////////////////////

#define PARSING_NONE                        0
#define PARSING_GPX                         1
#define PARSING_FILE_EXTENSIONS             2
#define PARSING_METADATA                    3
#define PARSING_METADATA_LINK               4
#define PARSING_METADATA_AUTHOR             5
#define PARSING_METADATA_EXTENSIONS         6
#define PARSING_TRK                         7
#define PARSING_TRK_LINK                    8
#define PARSING_TRK_EXTENSIONS              9
#define PARSING_TRKSEG                      10
#define PARSING_TRKSEG_EXTENSIONS           11
#define PARSING_TRKPT                       12
#define PARSING_TRKPT_LINK                  13
#define PARSING_TRKPT_EXTENSIONS            14
#define PARSING_TRKPT_EXTENSIONS_GPXTPX     15  // Garmin TrackPointExtension extension

static bool gOverwriteMetadata = false;
static int gVersion = 0;

static vector <string> *gExtensionVector = NULL;
static string gExtensionStr = "";
static bool gExtensionHasContent = false;
static int gExtensionPrevState = PARSING_NONE;
static int gExtensionLevelDepth = 0;
static bool gExtensionDoubleOpen = false;

static void SYS_setenv(const char *name, const char *value)
{
  #ifdef WIN32
    char str[128];
    sprintf(str, "%s=%s", name, value);
    putenv(str);
  #else
    setenv(name, value, 1);
  #endif
}

static void SYS_unsetenv(const char *name)
{
  #ifdef WIN32
    char str[128];
    sprintf(str, "%s=", name);
    putenv(str);
  #else
    unsetenv(name);
  #endif
}

static int getChar(void* ptr)
{
    return ((ifstream*)((T_uXml*)ptr)->fp)->get();
}

static time_t strToTime(const string str)
{
    struct tm timeinfo;
    timeinfo.tm_year = atoi(str.substr(0, 4).c_str()) - 1900;
    timeinfo.tm_mon = atoi(str.substr(5, 2).c_str()) - 1;
    timeinfo.tm_mday = atoi(str.substr(8, 2).c_str());
    timeinfo.tm_hour = atoi(str.substr(11, 2).c_str());
    timeinfo.tm_min = atoi(str.substr(14, 2).c_str());
    timeinfo.tm_sec = atoi(str.substr(17, 2).c_str());
    timeinfo.tm_isdst = 0;
    return mktime(&timeinfo);
}

static int strToMilliseconds(const string str)
{
    size_t len = str.length();
    if (len > 20)
    {
        int iVal = atoi(str.substr(20).c_str());
        if (len == 23)
            iVal *= 10;
        else if (len == 22)
            iVal *= 100;
        return iVal;
    }
    return 0;
}

static void strInitExtensionStr(int depth)
{
    gExtensionStr = "";
    depth *= DEPTH_INDENT;
    while (depth--)
        gExtensionStr += " ";
}

static void openTag(void* pXml, char* pTag)
{
    T_uXml* xml = (T_uXml*)pXml;
    GPX_model *gpxm = (GPX_model*)xml->pObject;

    switch (xml->state)
    {
    case PARSING_NONE:
        if (stricmp(pTag, "gpx") == 0)
        {
            xml->state = PARSING_GPX;
        }
        break;

    case PARSING_GPX:
        if (stricmp(pTag, "metadata") == 0)
        {
            xml->state = PARSING_METADATA;
        }
        else if (stricmp(pTag, "trk") == 0)
        {
            // add new track
            GPX_trkType trk(gpxm->trk.size());
            gpxm->trk.push_back(trk);
            xml->state = PARSING_TRK;
        }
        else if (stricmp(pTag, "extensions") == 0)
        {
            gExtensionPrevState = PARSING_GPX;
            gExtensionVector = &gpxm->extensions.extension;
            gExtensionStr = "";
            gExtensionLevelDepth = xml->recursionDepth + 1;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_FILE_EXTENSIONS;
        }
        break;

    case PARSING_METADATA:
        if (stricmp(pTag, "author") == 0 && gVersion != 10)
        {
            xml->state = PARSING_METADATA_AUTHOR;
        }
        else if (stricmp(pTag, "link") == 0)
        {
            // add new link
            GPX_linkType link;
            gpxm->metadata.links.push_back(link);
            xml->state = PARSING_METADATA_LINK;
        }
        else if (stricmp(pTag, "trk") == 0 && gVersion == 10)
        {
            // add new track
            GPX_trkType trk(gpxm->trk.size());
            gpxm->trk.push_back(trk);
            xml->state = PARSING_TRK;
        }
        else if (stricmp(pTag, "extensions") == 0)
        {
            gExtensionPrevState = PARSING_METADATA;
            gExtensionVector = &gpxm->metadata.extensions.extension;
            gExtensionStr = "";
            gExtensionLevelDepth = xml->recursionDepth + 1;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_METADATA_EXTENSIONS;
        }

        if (gVersion == 10)
        {
            if (stricmp(pTag, "url") == 0)
            {
                // add new link
                GPX_linkType link;
                gpxm->metadata.links.push_back(link);
            }
        }
        break;

    case PARSING_METADATA_LINK:
        break;

    case PARSING_METADATA_AUTHOR:
        break;

    case PARSING_TRK:
        if (stricmp(pTag, "trkseg") == 0)
        {
            // add new track segment
            GPX_trksegType trkseg;
            gpxm->trk.back().trkseg.push_back(trkseg);
            xml->state = PARSING_TRKSEG;
        }
        else if (stricmp(pTag, "link") == 0)
        {
            // add new link
            GPX_linkType link;
            gpxm->trk.back().metadata.links.push_back(link);
            xml->state = PARSING_TRK_LINK;
        }
        else if (stricmp(pTag, "extensions") == 0)
        {
            gExtensionPrevState = PARSING_TRK;
            gExtensionVector = &gpxm->trk.back().extensions.extension;
            gExtensionStr = "";
            gExtensionLevelDepth = xml->recursionDepth + 1;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_TRK_EXTENSIONS;
        }

        if (gVersion == 10)
        {
            if (stricmp(pTag, "url") == 0)
            {
                // add new link
                GPX_linkType link;
                gpxm->trk.back().metadata.links.push_back(link);
            }
        }
        break;

    case PARSING_TRK_LINK:
        break;

    case PARSING_TRKSEG:
        if (stricmp(pTag, "trkpt") == 0)
        {
            // add new track point
            GPX_wptType wpt;
            gpxm->trk.back().trkseg.back().trkpt.push_back(wpt);
            xml->state = PARSING_TRKPT;
        }
        else if (stricmp(pTag, "extensions") == 0)
        {
            gExtensionPrevState = PARSING_TRKSEG;
            gExtensionVector = &gpxm->trk.back().trkseg.back().extensions.extension;
            gExtensionStr = "";
            gExtensionLevelDepth = xml->recursionDepth + 1;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_TRKSEG_EXTENSIONS;
        }
        break;

    case PARSING_TRKPT:
        if (stricmp(pTag, "link") == 0)
        {
            // add new link
            GPX_linkType link;
            gpxm->trk.back().trkseg.back().trkpt.back().links.push_back(link);
            xml->state = PARSING_TRKPT_LINK;
        }
        else if (stricmp(pTag, "extensions") == 0)
        {
            gExtensionPrevState = PARSING_TRKPT;
            gExtensionVector = &gpxm->trk.back().trkseg.back().trkpt.back().extensions.extension;
            gExtensionStr = "";
            gExtensionLevelDepth = xml->recursionDepth + 1;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_TRKPT_EXTENSIONS;
        }

        if (gVersion == 10)
        {
            if (stricmp(pTag, "url") == 0)
            {
                // add new link
                GPX_linkType link;
                gpxm->trk.back().trkseg.back().trkpt.back().links.push_back(link);
            }
        }
        break;

    case PARSING_TRKPT_LINK:
        break;

    case PARSING_TRKPT_EXTENSIONS:
        if (stricmp(pTag, "gpxtpx:TrackPointExtension") == 0)
        {
            gExtensionVector = &gpxm->trk.back().trkseg.back().trkpt.back().extensionsGarmin.other;
            gExtensionStr = "";
            gExtensionLevelDepth++;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_TRKPT_EXTENSIONS_GPXTPX;
            break;
        }
    case PARSING_TRKPT_EXTENSIONS_GPXTPX:
        if (stricmp(pTag, "gpxtpx:hr") == 0)
        {
            break;
        }
    case PARSING_FILE_EXTENSIONS:
    case PARSING_METADATA_EXTENSIONS:
    case PARSING_TRK_EXTENSIONS:
    case PARSING_TRKSEG_EXTENSIONS:
        if (gExtensionDoubleOpen)
        {
            if (gExtensionVector)
                gExtensionVector->push_back(gExtensionStr);
            strInitExtensionStr(xml->recursionDepth - gExtensionLevelDepth);
        }
        gExtensionDoubleOpen = true;

        if (gExtensionStr == "")
            strInitExtensionStr(xml->recursionDepth - gExtensionLevelDepth);

        gExtensionStr += "<";
        gExtensionStr += pTag;
        gExtensionStr += ">";
        gExtensionHasContent = false;
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
        case PARSING_GPX:
            break;

        case PARSING_METADATA:
            if (gOverwriteMetadata)
            {
                if (stricmp(pTag, "name") == 0)
                    gpxm->metadata.name = sContent;
                else if (stricmp(pTag, "desc") == 0)
                    gpxm->metadata.desc = sContent;
                else if (stricmp(pTag, "year") == 0)
                    gpxm->metadata.copyright.year = sContent;
                else if (stricmp(pTag, "license") == 0)
                    gpxm->metadata.copyright.license = sContent;
                else if (stricmp(pTag, "time") == 0)
                {
                    gpxm->metadata.timestamp = strToTime(sContent);
                    gpxm->metadata.millisecond = strToMilliseconds(sContent);
                }
                else if (stricmp(pTag, "keywords") == 0)
                    gpxm->metadata.keywords = sContent;

                if (gVersion == 10)
                {
                    if (stricmp(pTag, "author") == 0)
                        gpxm->metadata.author.name = sContent;
                    else if (stricmp(pTag, "email") == 0)
                    {
                        size_t atPos = sContent.find_last_of("@");
                        if (atPos > 0)
                        {
                            gpxm->metadata.author.email.id = sContent.substr(0, atPos);
                            gpxm->metadata.author.email.domain = sContent.substr(atPos);
                        }
                    }
                    else if (stricmp(pTag, "url") == 0)
                        gpxm->metadata.links.back().href = sContent;
                    else if (stricmp(pTag, "urlname") == 0)
                        gpxm->metadata.links.back().text = sContent;
                }
            }
            break;

        case PARSING_METADATA_LINK:
            if (gOverwriteMetadata)
            {
                if (stricmp(pTag, "text") == 0)
                    gpxm->metadata.links.back().text = sContent;
                else if (stricmp(pTag, "type") == 0)
                    gpxm->metadata.links.back().type = sContent;
            }
            break;

        case PARSING_METADATA_AUTHOR:
            if (gOverwriteMetadata)
            {
                if (stricmp(pTag, "name") == 0)
                    gpxm->metadata.author.name = sContent;
                else if (stricmp(pTag, "text") == 0)
                    gpxm->metadata.author.link.text = sContent;
                else if (stricmp(pTag, "type") == 0)
                    gpxm->metadata.author.link.type = sContent;
            }
            break;

        case PARSING_TRK:
            if (stricmp(pTag, "name") == 0)
                gpxm->trk.back().metadata.name = sContent;
            else if (stricmp(pTag, "cmt") == 0)
                gpxm->trk.back().metadata.cmt = sContent;
            else if (stricmp(pTag, "desc") == 0)
                gpxm->trk.back().metadata.desc = sContent;
            else if (stricmp(pTag, "src") == 0)
                gpxm->trk.back().metadata.src = sContent;
            //else if (stricmp(pTag, "number") == 0)
            //    gpxm->trk.back().metadata.number = atoi(pContent);
            else if (stricmp(pTag, "type") == 0)
                gpxm->trk.back().metadata.type = sContent;

            if (gVersion == 10)
            {
                if (stricmp(pTag, "url") == 0)
                    gpxm->trk.back().metadata.links.back().href = sContent;
                else if (stricmp(pTag, "urlname") == 0)
                    gpxm->trk.back().metadata.links.back().text = sContent;
            }
            break;

        case PARSING_TRK_LINK:
            if (stricmp(pTag, "text") == 0)
                gpxm->trk.back().metadata.links.back().text = sContent;
            else if (stricmp(pTag, "type") == 0)
                gpxm->trk.back().metadata.links.back().type = sContent;
            break;

        case PARSING_TRKSEG:
            break;

        case PARSING_TRKPT:
            if (stricmp(pTag, "ele") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().altitude = atof(pContent);
            else if (stricmp(pTag, "time") == 0)
            {
                gpxm->trk.back().trkseg.back().trkpt.back().timestamp = strToTime(sContent);
                gpxm->trk.back().trkseg.back().trkpt.back().millisecond = strToMilliseconds(sContent);
            }
            else if (stricmp(pTag, "magvar") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().magvar = (float)atof(pContent);
            else if (stricmp(pTag, "geoidheight") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().geoidheight = (float)atof(pContent);
            else if (stricmp(pTag, "name") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().name = sContent;
            else if (stricmp(pTag, "cmt") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().cmt = sContent;
            else if (stricmp(pTag, "desc") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().desc = sContent;
            else if (stricmp(pTag, "src") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().src = sContent;
            else if (stricmp(pTag, "sym") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().sym = sContent;
            else if (stricmp(pTag, "type") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().type = sContent;
            else if (stricmp(pTag, "fix") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().fix = sContent;
            else if (stricmp(pTag, "sat") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().sat = atoi(pContent);
            else if (stricmp(pTag, "hdop") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().hdop = (float)atof(pContent);
            else if (stricmp(pTag, "vdop") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().vdop = (float)atof(pContent);
            else if (stricmp(pTag, "pdop") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().pdop = (float)atof(pContent);
            else if (stricmp(pTag, "ageofdgpsdata") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().ageofdgpsdata = (float)atof(pContent);
            else if (stricmp(pTag, "dgpsid") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().dgpsid = atoi(pContent);

            if (gVersion == 10)
            {
                if (stricmp(pTag, "url") == 0)
                    gpxm->trk.back().trkseg.back().trkpt.back().links.back().href = sContent;
                else if (stricmp(pTag, "urlname") == 0)
                    gpxm->trk.back().trkseg.back().trkpt.back().links.back().text = sContent;
            }
            break;

        case PARSING_TRKPT_LINK:
            if (stricmp(pTag, "text") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().links.back().text = sContent;
            else if (stricmp(pTag, "type") == 0)
                gpxm->trk.back().trkseg.back().trkpt.back().links.back().type = sContent;
            break;

        case PARSING_TRKPT_EXTENSIONS_GPXTPX:
            if (stricmp(pTag, "gpxtpx:hr") == 0)
            {
                gpxm->trk.back().trkseg.back().trkpt.back().extensionsGarmin.heartrate = atoi(pContent);
                break;
            }
        case PARSING_FILE_EXTENSIONS:
        case PARSING_METADATA_EXTENSIONS:
        case PARSING_TRK_EXTENSIONS:
        case PARSING_TRKSEG_EXTENSIONS:
        case PARSING_TRKPT_EXTENSIONS:
            gExtensionHasContent = true;
            gExtensionStr += pContent;
            break;
    }
}

static void tagAttribute(void* pXml, char* pTag, char *pAttribute, char* pContent)
{
    T_uXml* xml = (T_uXml*)pXml;
    GPX_model *gpxm = (GPX_model*)xml->pObject;
    string sContent = pContent;

    switch (xml->state)
    {
        case PARSING_GPX:
            if (stricmp(pTag, "gpx") == 0)
            {
                if (stricmp(pAttribute, "version") == 0)
                {
                    gVersion = (int)(10 * atof(pContent));
                    if (gVersion == 10)
                        xml->state = PARSING_METADATA;
                }
            }
            break;

        case PARSING_METADATA:
            if (gOverwriteMetadata)
            {
                if (stricmp(pTag, "copyright") == 0)
                {
                    if (stricmp(pAttribute, "author") == 0)
                        gpxm->metadata.copyright.author = sContent;
                }
                else if (stricmp(pTag, "bounds") == 0)
                {
                    if (stricmp(pAttribute, "minlat") == 0)
                        gpxm->metadata.bounds.minlat = atof(pContent);
                    else if (stricmp(pAttribute, "minlon") == 0)
                        gpxm->metadata.bounds.minlon = atof(pContent);
                    else if (stricmp(pAttribute, "maxlat") == 0)
                        gpxm->metadata.bounds.maxlat = atof(pContent);
                    else if (stricmp(pAttribute, "maxlon") == 0)
                        gpxm->metadata.bounds.maxlon = atof(pContent);
                }
            }
            break;

        case PARSING_METADATA_LINK:
            if (gOverwriteMetadata)
            {
                if (stricmp(pTag, "link") == 0)
                {
                    if (stricmp(pAttribute, "href") == 0)
                        gpxm->metadata.links.back().href = sContent;
                }
            }
            break;

        case PARSING_METADATA_AUTHOR:
            if (gOverwriteMetadata)
            {
                if (stricmp(pTag, "email") == 0)
                {
                    if (stricmp(pAttribute, "id") == 0)
                        gpxm->metadata.author.email.id = sContent;
                    else if (stricmp(pAttribute, "domain") == 0)
                        gpxm->metadata.author.email.domain = sContent;
                }
                else if (stricmp(pTag, "link") == 0)
                {
                    if (stricmp(pAttribute, "href") == 0)
                        gpxm->metadata.author.link.href = sContent;
                }
            }
            break;

        case PARSING_TRK:
            break;

        case PARSING_TRK_LINK:
            if (stricmp(pTag, "link") == 0)
            {
                if (stricmp(pAttribute, "href") == 0)
                    gpxm->trk.back().metadata.links.back().href = sContent;
            }
            break;

        case PARSING_TRKSEG:
            break;

        case PARSING_TRKPT:
            if (stricmp(pTag, "trkpt") == 0)
            {
                if (stricmp(pAttribute, "lat") == 0)
                    gpxm->trk.back().trkseg.back().trkpt.back().latitude = atof(pContent);
                else if (stricmp(pAttribute, "lon") == 0)
                    gpxm->trk.back().trkseg.back().trkpt.back().longitude = atof(pContent);
            }
            break;

        case PARSING_TRKPT_LINK:
            if (stricmp(pTag, "link") == 0)
            {
                if (stricmp(pAttribute, "href") == 0)
                    gpxm->trk.back().trkseg.back().trkpt.back().links.back().href = sContent;
            }
            break;

        case PARSING_FILE_EXTENSIONS:
        case PARSING_METADATA_EXTENSIONS:
        case PARSING_TRK_EXTENSIONS:
        case PARSING_TRKSEG_EXTENSIONS:
        case PARSING_TRKPT_EXTENSIONS:
        case PARSING_TRKPT_EXTENSIONS_GPXTPX:
            gExtensionStr.resize(gExtensionStr.size() - 1); // remove '>'
            gExtensionStr += " ";
            gExtensionStr += pAttribute;
            gExtensionStr += "=\"";
            gExtensionStr += pContent;
            gExtensionStr += "\">";
            break;
    }
}

static void closeTag(void* pXml, char* pTag)
{
    T_uXml* xml = (T_uXml*)pXml;
    GPX_model *gpxm = (GPX_model*)xml->pObject;

    switch (xml->state)
    {
    case PARSING_NONE:
        break;

    case PARSING_GPX:
        if (stricmp(pTag, "gpx") == 0)
            xml->state = PARSING_NONE;
        break;

    case PARSING_METADATA:
        if (stricmp(pTag, "metadata") == 0)
            xml->state = PARSING_GPX;
        break;

    case PARSING_METADATA_LINK:
        if (stricmp(pTag, "link") == 0)
            xml->state = PARSING_METADATA;
        break;

    case PARSING_METADATA_AUTHOR:
        if (stricmp(pTag, "author") == 0)
            xml->state = PARSING_METADATA;
        break;

    case PARSING_TRK:
        if (stricmp(pTag, "trk") == 0)
        {
            // update track
            GPX_model *gpxm = (GPX_model*)xml->pObject;
            gpxm->updateTrack(gpxm->trk.back());
            xml->state = PARSING_GPX;
        }
        break;

    case PARSING_TRK_LINK:
        if (stricmp(pTag, "link") == 0)
            xml->state = PARSING_TRK;
        break;

    case PARSING_TRKSEG:
        if (stricmp(pTag, "trkseg") == 0)
            xml->state = PARSING_TRK;
        break;

    case PARSING_TRKPT:
        if (stricmp(pTag, "trkpt") == 0)
            xml->state = PARSING_TRKSEG;
        break;

    case PARSING_TRKPT_LINK:
        if (stricmp(pTag, "link") == 0)
            xml->state = PARSING_TRKPT;
        break;

    case PARSING_TRKPT_EXTENSIONS_GPXTPX:
        if (stricmp(pTag, "gpxtpx:TrackPointExtension") == 0)
        {
            gExtensionVector = &gpxm->trk.back().trkseg.back().trkpt.back().extensions.extension;
            gExtensionStr = "";
            gExtensionLevelDepth--;
            gExtensionDoubleOpen = false;
            xml->state = PARSING_TRKPT_EXTENSIONS;
            break;
        }
        else if (stricmp(pTag, "gpxtpx:hr") == 0)
        {
            break;
        }
        case PARSING_FILE_EXTENSIONS:
        case PARSING_METADATA_EXTENSIONS:
        case PARSING_TRK_EXTENSIONS:
        case PARSING_TRKSEG_EXTENSIONS:
        case PARSING_TRKPT_EXTENSIONS:
            if (stricmp(pTag, "extensions") == 0)
            {
                gExtensionVector = NULL;
                xml->state = gExtensionPrevState;
            }
            else
            {
                if (gExtensionStr == "")
                    strInitExtensionStr(xml->recursionDepth - gExtensionLevelDepth);

                if (gExtensionHasContent)
                {
                    gExtensionStr += "</";
                    gExtensionStr += pTag;
                    gExtensionStr += ">";
                }
                else
                {
                    gExtensionStr.resize(gExtensionStr.size() - 1); // remove '>'
                    gExtensionStr += "/>";
                    gExtensionHasContent = true; // for closing nested tags <bla1><bla2/><bla1>
                }

                if (gExtensionVector)
                    gExtensionVector->push_back(gExtensionStr);
                gExtensionStr = "";
                gExtensionDoubleOpen = false;
            }
            break;
    }
}

GPX_model::retCode_e GPXFile::load(ifstream* fp, GPX_model* gpxm, bool overwriteMetadata)
{
    int ret;
    char *tz;
    T_uXml uXML;

    UXML_init(&uXML);
    uXML.fp = fp;
    uXML.getChar = getChar;
    uXML.pContent = gBuffer;
    uXML.maxContent = BUFFER_SIZE;
    uXML.openTag = openTag;
    uXML.closeTag = closeTag;
    uXML.setContent = tagContent;
    uXML.setAttribute = tagAttribute;
    uXML.state = PARSING_NONE;
    uXML.pObject = gpxm;

    gVersion = 0;
    gOverwriteMetadata = overwriteMetadata;
    gExtensionVector = NULL;

    // set timezone temporary to UTC
    tz = getenv("TZ");
    SYS_setenv("TZ", "UTC");
    tzset();

    // parse file
    ret = UXML_parseFile(&uXML);

    // change back timezone
    if (tz)
        SYS_setenv("TZ", tz);
    else
        SYS_unsetenv("TZ");
    tzset();

    if (ret != 0)
        return GPX_model::GPXM_ERR_FAILED;
    return GPX_model::GPXM_OK;
}

////////////////////////////////////////////////////////////////////////////////

static void writeLineIndent(ofstream *fp, int depth)
{
    depth *= DEPTH_INDENT;
    while (depth--)
        fp->put(' ');
}

static void writeStr(ofstream *fp, const char *str, bool endOfLine = true)
{
    fp->write(str, strlen(str));
    if (endOfLine)
        fp->put('\n');
}

static void writeSimpleTag(ofstream *fp, int depth, const char *tag, const char *content)
{
    writeLineIndent(fp, depth);
    fp->put('<');
    fp->write(tag, strlen(tag));
    fp->put('>');
    fp->write(content, strlen(content));
    fp->put('<');
    fp->put('/');
    fp->write(tag, strlen(tag));
    fp->put('>');
    fp->put('\n');
}

static void writeCopyright(ofstream *fp, int depth, const GPX_copyrightType *c)
{
    if (!c->author.empty())
    {
        writeLineIndent(fp, depth);
        writeStr(fp, "<copyright author=\"", false);
        writeStr(fp, c->author.c_str(), false);
        writeStr(fp, "\">");
        if (!c->year.empty())
            writeSimpleTag(fp, depth + 1, "year", c->year.c_str());
        if (!c->license.empty())
            writeSimpleTag(fp, depth + 1, "license", c->license.c_str());
        writeLineIndent(fp, depth);
        writeStr(fp, "</copyright>");
    }
}

static void writeLink(ofstream *fp, int depth, const GPX_linkType *l)
{
    if (!l->href.empty())
    {
        writeLineIndent(fp, depth);
        writeStr(fp, "<link href=\"", false);
        writeStr(fp, l->href.c_str(), false);
        writeStr(fp, "\">");
        if (!l->text.empty())
            writeSimpleTag(fp, depth + 1, "text", l->text.c_str());
        if (!l->type.empty())
            writeSimpleTag(fp, depth + 1, "type", l->type.c_str());
        writeLineIndent(fp, depth);
        writeStr(fp, "</link>");
    }
}

static void writeEmail(ofstream *fp, int depth, const GPX_emailType *e)
{
    if (!e->id.empty())
    {
        writeLineIndent(fp, depth);
        writeStr(fp, "<email id=\"", false);
        writeStr(fp, e->id.c_str(), false);
        writeStr(fp, "\" domain=\"", false);
        writeStr(fp, e->domain.c_str(), false);
        writeStr(fp, "\"/>");
    }
}

static void writeAuthor(ofstream *fp, int depth, const GPX_personType *a)
{
    if (!a->name.empty() || !a->email.id.empty() || !a->link.href.empty())
    {
        writeLineIndent(fp, depth);
        writeStr(fp, "<author>");
        if (!a->name.empty())
            writeSimpleTag(fp, depth + 1, "name", a->name.c_str());
        writeEmail(fp, depth + 1, &a->email);
        writeLink(fp, depth + 1, &a->link);
        writeLineIndent(fp, depth);
        writeStr(fp, "</author>");
    }
}

static void writeBounds(ofstream *fp, int depth, const GPX_boundsType *b)
{
    writeLineIndent(fp, depth);
    writeStr(fp, "<bounds minlat=\"", false);
    sprintf(gBuffer, "%.9f", b->minlat);
    writeStr(fp, gBuffer, false);
    writeStr(fp, "\" minlon=\"", false);
    sprintf(gBuffer, "%.9f", b->minlon);
    writeStr(fp, gBuffer, false);
    writeStr(fp, "\" maxlat=\"", false);
    sprintf(gBuffer, "%.9f", b->maxlat);
    writeStr(fp, gBuffer, false);
    writeStr(fp, "\" maxlon=\"", false);
    sprintf(gBuffer, "%.9f", b->maxlon);
    writeStr(fp, gBuffer, false);
    writeStr(fp, "\"/>");
}

static void writeExtensions(ofstream *fp, int depth, const GPX_extType *e)
{
    if (!e->extension.empty())
    {
        writeLineIndent(fp, depth);
        writeStr(fp, "<extensions>");
        for (vector<string>::const_iterator str = e->extension.begin(); str != e->extension.end(); ++str)
        {
            writeLineIndent(fp, depth + 1);
            writeStr(fp, str->c_str());
        }
        writeLineIndent(fp, depth);
        writeStr(fp, "</extensions>");
    }
}

static void writePointExtensions(ofstream *fp, int depth, const GPX_wptType *p)
{
    if (!p->extensions.extension.empty() || !p->extensionsGarmin.other.empty() || p->extensionsGarmin.heartrate > 0)
    {
        vector<string>::const_iterator itExt;

        writeLineIndent(fp, depth);
        writeStr(fp, "<extensions>");

        // Garmin extensions
        if (!p->extensionsGarmin.other.empty() || p->extensionsGarmin.heartrate > 0)
        {
            writeLineIndent(fp, depth + 1);
            writeStr(fp, "<gpxtpx:TrackPointExtension>");
            if (p->extensionsGarmin.heartrate > 0)
            {
                writeLineIndent(fp, depth + 2);
                writeStr(fp, "<gpxtpx:hr>", false);
                sprintf(gBuffer, "%d", p->extensionsGarmin.heartrate);
                writeStr(fp, gBuffer, false);
                writeStr(fp, "</gpxtpx:hr>");
            }
            if (!p->extensionsGarmin.other.empty())
            {
                for (itExt = p->extensionsGarmin.other.begin(); itExt != p->extensionsGarmin.other.end(); ++itExt)
                {
                    writeLineIndent(fp, depth + 2);
                    writeStr(fp, itExt->c_str());
                }
            }
            writeLineIndent(fp, depth + 1);
            writeStr(fp, "</gpxtpx:TrackPointExtension>");
        }

        // other (unknown) extensions
        for (itExt = p->extensions.extension.begin(); itExt != p->extensions.extension.end(); ++itExt)
        {
            writeLineIndent(fp, depth + 1);
            writeStr(fp, itExt->c_str());
        }

        writeLineIndent(fp, depth);
        writeStr(fp, "</extensions>");
    }
}

static void writeTime(ofstream *fp, int depth, time_t t, int milli, bool local = false)
{
    if (milli > 999)
        milli = 0;
    writeLineIndent(fp, depth);
    writeStr(fp, "<time>", false);
    if (milli == 0)
    {
        strftime(gBuffer, BUFFER_SIZE, "%Y-%m-%dT%H:%M:%SZ", local ? localtime(&t) : gmtime(&t));
    }
    else
    {
        char sMilli[4];
        size_t milliPos = strftime(gBuffer, BUFFER_SIZE, "%Y-%m-%dT%H:%M:%S.000Z", local ? localtime(&t) : gmtime(&t)) - 4;
        sprintf(sMilli, "%d", milli);
        if (milli < 10)
        {
            gBuffer[milliPos + 2] = sMilli[0];
        }
        else if (milli < 100)
        {
            gBuffer[milliPos + 1] = sMilli[0];
            gBuffer[milliPos + 2] = sMilli[1];
        }
        else
        {
            gBuffer[milliPos + 0] = sMilli[0];
            gBuffer[milliPos + 1] = sMilli[1];
            gBuffer[milliPos + 2] = sMilli[2];
        }

    }
    writeStr(fp, gBuffer, false);
    writeStr(fp, "</time>");
}

static void writeMetadata(ofstream* fp, int depth, const GPX_metadataType* m)
{
    writeLineIndent(fp, depth);
    writeStr(fp, "<metadata>");
    if (!m->name.empty())
        writeSimpleTag(fp, depth + 1, "name", m->name.c_str());
    if (!m->desc.empty())
        writeSimpleTag(fp, depth + 1, "desc", m->desc.c_str());
    writeAuthor(fp, depth + 1, &m->author);
    writeCopyright(fp, depth + 1, &m->copyright);
    if (!m->links.empty())
        for (vector<GPX_linkType>::const_iterator link = m->links.begin(); link != m->links.end(); ++link)
            writeLink(fp, depth + 1, &*link);
    writeTime(fp, depth + 1, m->timestamp, m->millisecond);
    if (!m->keywords.empty())
        writeSimpleTag(fp, depth + 1, "keywords", m->keywords.c_str());
    writeBounds(fp, depth + 1, &m->bounds);
    writeExtensions(fp, depth + 1, &m->extensions);
    writeLineIndent(fp, depth);
    writeStr(fp, "</metadata>");
}

GPX_model::retCode_e GPXFile::save(ofstream* fp, const GPX_model* gpxm)
{
    writeStr(fp, "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>");
    writeStr(fp, "<gpx xmlns=\"http://www.topografix.com/GPX/1/1\" version=\"1.1\" creator=\"", false);
    writeStr(fp, gpxm->creator.c_str(), false);
    writeStr(fp, "\"");
    writeStr(fp, "     xmlns:gpxtpx=\"http://www.garmin.com/xmlschemas/TrackPointExtension/v1\"");
    writeStr(fp, "     xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"");
    writeStr(fp, "     xsi:schemaLocation=\"http://www.topografix.com/GPX/1/1 http://www.topografix.com/GPX/1/1/gpx.xsd\">");
    writeMetadata(fp, 1, &gpxm->metadata);
    if (!gpxm->trk.empty())
    {
        for (vector<GPX_trkType>::const_iterator trk = gpxm->trk.begin(); trk != gpxm->trk.end(); ++trk)
        {
            writeLineIndent(fp, 1);
            writeStr(fp, "<trk>");
            if (!trk->metadata.name.empty())
                writeSimpleTag(fp, 2, "name", trk->metadata.name.c_str());
            if (!trk->metadata.cmt.empty())
                writeSimpleTag(fp, 2, "cmt", trk->metadata.cmt.c_str());
            if (!trk->metadata.desc.empty())
                writeSimpleTag(fp, 2, "desc", trk->metadata.desc.c_str());
            if (!trk->metadata.src.empty())
                writeSimpleTag(fp, 2, "src", trk->metadata.src.c_str());
            if (!trk->metadata.links.empty())
                for (vector<GPX_linkType>::const_iterator link = trk->metadata.links.begin(); link != trk->metadata.links.end(); ++link)
                    writeLink(fp, 2, &*link);
            writeLineIndent(fp, 2);
            writeStr(fp, "<number>", false);
            sprintf(gBuffer, "%zu", trk->metadata.number);
            writeStr(fp, gBuffer, false);
            writeStr(fp, "</number>");
            if (!trk->metadata.type.empty())
                writeSimpleTag(fp, 2, "type", trk->metadata.type.c_str());
            writeExtensions(fp, 2, &trk->extensions);
            if (!trk->trkseg.empty())
            {
                for (vector<GPX_trksegType>::const_iterator trkseg = trk->trkseg.begin(); trkseg != trk->trkseg.end(); ++trkseg)
                {
                    writeLineIndent(fp, 2);
                    writeStr(fp, "<trkseg>");
                    if (!trkseg->trkpt.empty())
                    {
                        for (vector<GPX_wptType>::const_iterator trkpt = trkseg->trkpt.begin(); trkpt != trkseg->trkpt.end(); ++trkpt)
                        {
                            writeLineIndent(fp, 3);
                            writeStr(fp, "<trkpt lat=\"", false);
                            sprintf(gBuffer, "%.9f", trkpt->latitude);
                            writeStr(fp, gBuffer, false);
                            writeStr(fp, "\" lon=\"", false);
                            sprintf(gBuffer, "%.9f", trkpt->longitude);
                            writeStr(fp, gBuffer, false);
                            writeStr(fp, "\">");
                            sprintf(gBuffer, "%.6f", trkpt->altitude);
                            writeSimpleTag(fp, 4, "ele", gBuffer);
                            if (trkpt->timestamp > 0)
                                writeTime(fp, 4, trkpt->timestamp, trkpt->millisecond);
                            if (trkpt->magvar != 0.0f)
                            {
                                sprintf(gBuffer, "%.6f", trkpt->magvar);
                                writeSimpleTag(fp, 4, "magvar", gBuffer);
                            }
                            if (trkpt->geoidheight != 0.0f)
                            {
                                sprintf(gBuffer, "%.1f", trkpt->geoidheight);
                                writeSimpleTag(fp, 4, "geoidheight", gBuffer);
                            }
                            if (!trkpt->name.empty())
                                writeSimpleTag(fp, 4, "name", trkpt->name.c_str());
                            if (!trkpt->cmt.empty())
                                writeSimpleTag(fp, 4, "cmt", trkpt->cmt.c_str());
                            if (!trkpt->desc.empty())
                                writeSimpleTag(fp, 4, "desc", trkpt->desc.c_str());
                            if (!trkpt->src.empty())
                                writeSimpleTag(fp, 4, "src", trkpt->src.c_str());
                            if (!trkpt->links.empty())
                                for (vector<GPX_linkType>::const_iterator link = trkpt->links.begin(); link != trkpt->links.end(); ++link)
                                    writeLink(fp, 4, &*link);
                            if (!trkpt->sym.empty())
                                writeSimpleTag(fp, 4, "sym", trkpt->sym.c_str());
                            if (!trkpt->type.empty())
                                writeSimpleTag(fp, 4, "type", trkpt->type.c_str());
                            if (!trkpt->fix.empty())
                                writeSimpleTag(fp, 4, "fix", trkpt->fix.c_str());
                            if (trkpt->sat != 0)
                            {
                                sprintf(gBuffer, "%d", trkpt->sat);
                                writeSimpleTag(fp, 4, "sat", gBuffer);
                            }
                            if (trkpt->hdop != 0.0f)
                            {
                                sprintf(gBuffer, "%.6f", trkpt->hdop);
                                writeSimpleTag(fp, 4, "hdop", gBuffer);
                            }
                            if (trkpt->vdop != 0.0f)
                            {
                                sprintf(gBuffer, "%.6f", trkpt->vdop);
                                writeSimpleTag(fp, 4, "vdop", gBuffer);
                            }
                            if (trkpt->pdop != 0.0f)
                            {
                                sprintf(gBuffer, "%.6f", trkpt->pdop);
                                writeSimpleTag(fp, 4, "pdop", gBuffer);
                            }
                            if (trkpt->ageofdgpsdata != 0.0f)
                            {
                                sprintf(gBuffer, "%.6f", trkpt->ageofdgpsdata);
                                writeSimpleTag(fp, 4, "ageofdgpsdata", gBuffer);
                            }
                            if (trkpt->dgpsid != 0)
                            {
                                sprintf(gBuffer, "%d", trkpt->dgpsid);
                                writeSimpleTag(fp, 4, "dgpsid", gBuffer);
                            }
                            writePointExtensions(fp, 4, &*trkpt);
                            writeLineIndent(fp, 3);
                            writeStr(fp, "</trkpt>");
                        }
                    }
                    writeExtensions(fp, 3, &trkseg->extensions);
                    writeLineIndent(fp, 2);
                    writeStr(fp, "</trkseg>");
                }
            }
            writeLineIndent(fp, 1);
            writeStr(fp, "</trk>");
        }
    }
    writeExtensions(fp, 1, &gpxm->extensions);
    writeStr(fp, "</gpx>");
    return GPX_model::GPXM_OK;
}
