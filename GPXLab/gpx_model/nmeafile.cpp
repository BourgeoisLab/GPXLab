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

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "nmeafile.h"

// Maximal length of a NMEA sentence
#define NMEA_MAXLENGTH                  82

static bool correctDate = false;
static int tm_mday = -1, tm_mon = -1, tm_year = -1;

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
    double decimal = (atof(value) - (degrees * 100)) / 60.0f;
    return degrees + decimal;
}

static int parseNMEA(GPX_wptType *pWp, const char *pNmea)
{
    static char buffer[12];

    const char *p = pNmea;
    int len = strlen(pNmea);

    int numCommas = 0;

    // do checksum check
    if (pNmea[len - 3] == '*')
    {
        int i;
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
        if (tm_mday != -1)
        {
            timeinfo.tm_hour = itime / 10000;
            timeinfo.tm_min = (itime % 10000) / 100;
            timeinfo.tm_sec = (itime % 100);
            timeinfo.tm_mday = tm_mday;
            timeinfo.tm_mon = tm_mon;
            timeinfo.tm_year = tm_year;
            timeinfo.tm_isdst = 0;
            pWp->timestamp = mktime(&timeinfo) - timezone;
        }
        else
        {
            pWp->timestamp = itime;
        }
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
        pWp->hdop = (float)atof(p);

        // altitude
        p = strchr(p, ',') + 1;
        pWp->altitude = atof(p) ;
        p = strchr(p, ',') + 1;

        // height of geoid
        p = strchr(p, ',') + 1;
        pWp->geoidheight = (float)atof(p);
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
            if (tm_mday != -1)
            {
                timeinfo.tm_mday = tm_mday;
                timeinfo.tm_mon = tm_mon;
                timeinfo.tm_year = tm_year;
                pWp->timestamp = mktime(&timeinfo) - timezone;
            }
            else
            {
                pWp->timestamp = itime;
            }
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
        pWp->speed = (float)atof(p) * 1.852f; // knots -> km/h

        // angle
        p = strchr(p, ',') + 1;
        pWp->heading = (float)atof(p);

        // date
        p = strchr(p, ',') + 1;
        itime = atol(p);
        timeinfo.tm_mday = itime / 10000;
        timeinfo.tm_mon = (itime % 10000) / 100 - 1;
        timeinfo.tm_year = 100 + (itime % 100);
        pWp->timestamp = mktime(&timeinfo) - timezone;
        if (tm_mday == -1)
            correctDate = true;
        tm_mday = timeinfo.tm_mday;
        tm_mon = timeinfo.tm_mon;
        tm_year = timeinfo.tm_year;

        return 1;
    }

    // unknown sentence
    return 0;
}

GPX_model::retCode_e NMEAFile::load(ifstream* fp, GPX_model* gpxm, const string& name)
{
    GPX_trkType trk(gpxm->trk.size());
    GPX_trksegType trkseg;
    GPX_wptType trkpt, trkpt_last;
    vector<GPX_wptType>::iterator itrkpt;
    char buffer[NMEA_MAXLENGTH];

    correctDate = false;
    tm_mday = -1;
    tm_mon = -1;
    tm_year = -1;

    while (fp->getline(buffer, NMEA_MAXLENGTH))
    {
        if (parseNMEA(&trkpt, buffer))
        {
            if (correctDate)
            {
                // add date to all previous points
                struct tm timeinfo;
                timeinfo.tm_hour = 0;
                timeinfo.tm_min = 0;
                timeinfo.tm_sec = 0;
                timeinfo.tm_mday = tm_mday;
                timeinfo.tm_mon = tm_mon;
                timeinfo.tm_year = tm_year;
                for (itrkpt = trkseg.trkpt.begin(); itrkpt != trkseg.trkpt.end(); itrkpt++)
                {
                    timeinfo.tm_hour = (int)(itrkpt->timestamp / 10000);
                    timeinfo.tm_min = (itrkpt->timestamp % 10000) / 100;
                    timeinfo.tm_sec = (itrkpt->timestamp % 100);
                    timeinfo.tm_isdst = 0;
                    itrkpt->timestamp = mktime(&timeinfo) - timezone;

                }
                timeinfo.tm_hour = (int)(trkpt_last.timestamp / 10000);
                timeinfo.tm_min = (trkpt_last.timestamp % 10000) / 100;
                timeinfo.tm_sec = (trkpt_last.timestamp % 100);
                timeinfo.tm_isdst = 0;
                trkpt_last.timestamp = mktime(&timeinfo) - timezone;
                correctDate = false;
            }
            if (!trkpt.isSameTime(trkpt_last))
            {
                if (trkpt_last.timestamp != 0)
                {
                    // add track point to the track segment
                    trkseg.trkpt.push_back(trkpt_last);
                }
                trkpt_last = trkpt;
                trkpt.timestamp = 0;
                trkpt.millisecond = 0;
            }
        }
    }

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
