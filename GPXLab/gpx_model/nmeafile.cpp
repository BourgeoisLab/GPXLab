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
#include <time.h>
#include "nmeafile.h"
#include "utils.h"

// Maximal length of a NMEA sentence
#define NMEA_MAXLENGTH                  82

static bool hasDate = false;
static int tm_mday = 0, tm_mon = 0, tm_year = 0;

static int parse_hex(char c)
{
    if (c < '0')
        return 0;
    if (c <= '9')
        return c - '0';
    if (c < 'A')
        return 0;
    if (c <= 'F')
        return (c - 'A') + 10;
    if (c < 'a')
        return 0;
    if (c <= 'f')
        return (c - 'a') + 10;
    return 0;
}

static void copy_value(char *dst, const char *src, int maxsize)
{
    int n = 0;
    while (*src)
    {
        if (*src == ',')
            break;
        *dst++ = *src++;
        if (++n >= maxsize - 1)
            break;
    }
    *dst = '\0';
}

static double parse_position(const char *value)
{
    int degrees = atoi(value) / 100;
    double decimal = (UTILS_atof(value) - (degrees * 100)) / 60.0f;
    return degrees + decimal;
}

static int parseNMEA(GPX_wptType *pWp, const char *pNmea)
{
    static char buffer[12];

    const char *p = pNmea;
    size_t len = strlen(pNmea);

    int numCommas = 0;

    // do checksum check
    if (pNmea[len - 3] == '*')
    {
        size_t i;
        int sum = parse_hex(pNmea[len - 2]) * 16 + parse_hex(pNmea[len - 1]);
        for (i = 1; i < (len - 3); i++)
            sum ^= pNmea[i];
        if (sum != 0)
            return 0; // bad checksum
    }
    else
    {
        return 0; // sentence incomplete
    }

    // count number of commas
    do
    {
        if (*p == ',')
            numCommas++;
    } while (*p++);
    p = pNmea;

    // GGA sentence
    if (strncmp(pNmea, "$GPGGA", 6) == 0)
    {
        int itime, fix;
        struct tm timeinfo;

        if (numCommas != 14)
            return 0; // bad format

        // first set quality to bad
        pWp->fix = "";

        // time
        p = strchr(p, ',') + 1;
        itime = atoi(p);
        timeinfo.tm_hour = itime / 10000;
        timeinfo.tm_min = (itime % 10000) / 100;
        timeinfo.tm_sec = (itime % 100);
        if (!hasDate)
        {
            // TODO: check if it's a new day and increase tm_mday
        }
        timeinfo.tm_mday = tm_mday;
        timeinfo.tm_mon = tm_mon;
        timeinfo.tm_year = tm_year;
        timeinfo.tm_isdst = 0;
        pWp->timestamp = mktime(&timeinfo);
        p = strchr(p, '.') + 1;
        pWp->millisecond = atoi(p);

        // latitude
        p = strchr(p, ',') + 1;
        copy_value(buffer, p, 12);
        pWp->latitude = parse_position(buffer);
        p = strchr(p, ',') + 1;
        if (*p == 'S')
            pWp->latitude *= -1;

        // longitude
        p = strchr(p, ',') + 1;
        copy_value(buffer, p, 12);
        pWp->longitude = parse_position(buffer);
        p = strchr(p, ',') + 1;
        if (*p == 'W')
            pWp->longitude *= -1;

        // fix quality
        p = strchr(p, ',') + 1;
        fix = atoi(p);

        // number of satellites
        p = strchr(p, ',') + 1;
        pWp->sat = atoi(p);

        switch (fix)
        {
        case 0:
            pWp->fix = "none";
            break;
        case 1:
            pWp->fix = (pWp->sat > 3) ? "3d" : "2d";
            break;
        case 2:
            pWp->fix = "dgps";
            break;
        case 3:
            pWp->fix = "pps";
            break;
        }

        // horizontal dilution of position
        p = strchr(p, ',') + 1;
        pWp->hdop = (float)UTILS_atof(p);

        // altitude
        p = strchr(p, ',') + 1;
        pWp->altitude = UTILS_atof(p) ;
        p = strchr(p, ',') + 1;

        // height of geoid
        p = strchr(p, ',') + 1;
        pWp->geoidheight = (float)UTILS_atof(p);
        p = strchr(p, ',') + 1;

        return 1;
    }

    // RMC sentence
    if (strncmp(pNmea, "$GPRMC", 6) == 0)
    {
        int itime;
        struct tm timeinfo;

        if (numCommas != 12)
            return 0; // bad format

        // time
        p = strchr(p, ',') + 1;
        itime = atoi(p);
        timeinfo.tm_hour = itime / 10000;
        timeinfo.tm_min = (itime % 10000) / 100;
        timeinfo.tm_sec = (itime % 100);
        timeinfo.tm_isdst = 0;
        p = strchr(p, '.') + 1;
        pWp->millisecond = atoi(p);

        // status
        p = strchr(p, ',') + 1;
        if (*p != 'A')
        {
            timeinfo.tm_mday = tm_mday;
            timeinfo.tm_mon = tm_mon;
            timeinfo.tm_year = tm_year;
            pWp->timestamp = mktime(&timeinfo);
            return 0;
        }

        // latitude
        p = strchr(p, ',') + 1;
        copy_value(buffer, p, 12);
        pWp->latitude = parse_position(buffer);
        p = strchr(p, ',') + 1;
        if (*p == 'S')
            pWp->latitude *= -1;

        // longitude
        p = strchr(p, ',') + 1;
        copy_value(buffer, p, 12);
        pWp->longitude = parse_position(buffer);
        p = strchr(p, ',') + 1;
        if (*p == 'W')
            pWp->longitude *= -1;

        // speed
        p = strchr(p, ',') + 1;
        pWp->speed = (float)UTILS_atof(p) * 1.852f; // knots -> km/h

        // angle
        p = strchr(p, ',') + 1;
        pWp->heading = (float)UTILS_atof(p);

        // date
        hasDate = true;
        p = strchr(p, ',') + 1;
        itime = atol(p);
        timeinfo.tm_mday = itime / 10000;
        timeinfo.tm_mon = (itime % 10000) / 100 - 1;
        timeinfo.tm_year = 100 + (itime % 100);
        pWp->timestamp = mktime(&timeinfo);
        tm_mday = timeinfo.tm_mday;
        tm_mon = timeinfo.tm_mon;
        tm_year = timeinfo.tm_year;

        return 1;
    }

    // unknown sentence
    return 0;
}

static void copyValues(GPX_wptType &pDst, const GPX_wptType &pSrc)
{
    if (!pSrc.fix.empty())
        pDst.fix = pSrc.fix;
    if (pSrc.sat != 0)
        pDst.sat = pSrc.sat;
    if (pSrc.timestamp != 0)
        pDst.timestamp = pSrc.timestamp;
    if (pSrc.millisecond != 0)
        pDst.millisecond = pSrc.millisecond;
    if (pSrc.magvar != 0.0f)
        pDst.magvar = pSrc.magvar;
    if (pSrc.altitude != 0.0)
        pDst.altitude = pSrc.altitude;
    if (pSrc.hdop != 0.0f)
        pDst.hdop = pSrc.hdop;
    if (pSrc.vdop != 0.0f)
        pDst.vdop = pSrc.vdop;
    if (pSrc.pdop != 0.0f)
        pDst.pdop = pSrc.pdop;
    if (pSrc.ageofdgpsdata != 0.0f)
        pDst.ageofdgpsdata = pSrc.ageofdgpsdata;
    if (pSrc.dgpsid != 0)
        pDst.dgpsid = pSrc.dgpsid;
    if (pSrc.geoidheight != 0.0f)
        pDst.geoidheight = pSrc.geoidheight;
    if (pSrc.latitude != 0.0)
        pDst.latitude = pSrc.latitude;
    if (pSrc.longitude != 0.0)
        pDst.longitude = pSrc.longitude;
    if (!pSrc.name.empty())
        pDst.name = pSrc.name;
    if (!pSrc.cmt.empty())
        pDst.cmt = pSrc.cmt;
    if (!pSrc.desc.empty())
        pDst.desc = pSrc.desc;
    if (!pSrc.src.empty())
        pDst.src = pSrc.src;
    if (!pSrc.links.empty())
        pDst.links = pSrc.links;
    if (!pSrc.sym.empty())
        pDst.sym = pSrc.sym;
    if (!pSrc.type.empty())
        pDst.type = pSrc.type;
    if (!pSrc.extensions.extension.empty())
        pDst.extensions.extension = pSrc.extensions.extension;
    if (pSrc.speed != 0.0f)
        pDst.speed = pSrc.speed;
    if (pSrc.heading != 0.0f)
        pDst.heading = pSrc.heading;
    if (pSrc.leglength != 0.0)
        pDst.leglength = pSrc.leglength;
    if (pSrc.distanceTot != 0.0)
        pDst.distanceTot = pSrc.distanceTot;
    if (pSrc.elapsedTime != 0)
        pDst.elapsedTime = pSrc.elapsedTime;
}

GPX_model::retCode_e NMEAFile::load(ifstream* fp, GPX_model* gpxm, const string& name)
{
    GPX_trkType trk(gpxm->trk.size());
    GPX_trksegType trkseg;
    GPX_wptType trkpt, trkpt_last;
    char buffer[NMEA_MAXLENGTH];
    bool hasComputedMissingDate = false;

    // set timezone temporary to UTC
    char *tz = getenv("TZ");
    UTILS_setenv("TZ", "UTC");
    tzset();

    hasDate = false;
    tm_mday = 1;
    tm_mon = 0;
    tm_year = 100;

    while (fp->getline(buffer, NMEA_MAXLENGTH))
    {
        if (parseNMEA(&trkpt, buffer))
        {
            if (hasDate && !hasComputedMissingDate)
            {
                struct tm *timeinfo;

                // add date to all previous points
                vector<GPX_wptType>::iterator itrkpt;
                for (itrkpt = trkseg.trkpt.begin(); itrkpt != trkseg.trkpt.end(); itrkpt++)
                {
                    timeinfo = gmtime(&itrkpt->timestamp);
                    timeinfo->tm_mday = tm_mday;
                    timeinfo->tm_mon = tm_mon;
                    timeinfo->tm_year = tm_year;
                    timeinfo->tm_isdst = 0;
                    itrkpt->timestamp = mktime(timeinfo);
                }

                // add date to last point
                if (trkpt_last.timestamp != 0)
                {
                    timeinfo = gmtime(&trkpt_last.timestamp);
                    timeinfo->tm_mday = tm_mday;
                    timeinfo->tm_mon = tm_mon;
                    timeinfo->tm_year = tm_year;
                    timeinfo->tm_isdst = 0;
                    trkpt_last.timestamp = mktime(timeinfo);
                }

                hasComputedMissingDate = true;
            }

            if (!trkpt.isSameTime(trkpt_last) && trkpt_last.timestamp != 0)
            {
                // add track point to the track segment
                trkseg.trkpt.push_back(trkpt_last);
                trkpt_last.clear();
            }

            // copy parsed values
            copyValues(trkpt_last, trkpt);
            trkpt.clear();
        }
    }

    // change back timezone
    if (tz)
        UTILS_setenv("TZ", tz);
    else
        UTILS_unsetenv("TZ");
    tzset();

    // add last track point
    trkseg.trkpt.push_back(trkpt_last);

    // add segment to the track
    trk.trkseg.push_back(trkseg);

    // set track name
    trk.metadata.name = name;

    // update track
    gpxm->updateTrack(trk);

    // add track
    gpxm->trk.push_back(trk);

    return GPX_model::GPXM_OK;
}
