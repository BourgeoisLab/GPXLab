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

#ifndef _SRTM_H_
#define _SRTM_H_

#include <string>

using namespace std;

/**
 * @addtogroup Functions Functions
 * @brief Specific functions
 * @{
 */

/**
 * @class SRTM
 *
 * @brief Lookup functions for altitude values given latitude/longitude
 *
 * This file provides a lookup function for altitude values given
 * latitude and longitude coordinates. The lookup table is given by
 * SRTM (Shuttle Radar Topography Mission).
 *
 * The value is interpolated from the four corner points.
 *
 * From http://www2.jpl.nasa.gov/srtm/faq.html:
 *
 * The SRTM data files have names like "N34W119.hgt". What do the letters
 * and numbers refer to, and what is ".hgt" format?
 *
 * Each data file covers a one-degree-of-latitude by one-degree-of-longitude
 * block of Earth's surface. The first seven characters indicate the
 * south west corner of the block, with N, S, E, and W referring to north,
 * south, east, and west. Thus, the "N34W119.hgt" file covers latitudes 34
 * to 35 North and longitudes 118-119 West (this file includes down town Los
 * Angeles, California). The file name extension ".hgt" simply stands for the
 * word "height", meaning elevation. It is NOT a format type. These files
 * are in "raw" format (no headers and not compressed), 16-bit signed
 * integers, elevation measured in meters above sea level, in a "geographic"
 * (latitude and longitude array) projection, with data voids indicated by
 * -32768. International 3-arc-second files have 1201 columns and 1201 rows
 * of data, with a total file size of 2,884,802 bytes ( = 1201 x 1201 x 2).
 * United States 1-arc-second files have 3601 columns and 3601 rows of data,
 * with a total file size of 25,934,402 bytes ( = 3601 x 3601 x 2). For more
 * information read the text file "SRTM_Topo.txt" at
 * http://edcftp.cr.usgs.gov/pub/data/srtm/Readme.html
 *
 * The bytes are in Motorola "big-endian" order with the most significant
 * byte first.
 *
 * A single HGT file is cached to improve performance. If another HGT file
 * is needed the old cache is clear and replaced.
 *
 * @see http://www2.jpl.nasa.gov/srtm/
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 28 Oct 2014
 */
class SRTM
{
public:

    /**
     * @brief Model of HGT files
     */
    enum SRTMModel
    {
        OneArcSecond = 3601,    /**< One arc-second lat/long intervals, 3601 lines, 3601 samples */
        ThreeArcSecond = 1201   /**< three arc-second lat/long intervals, 1201 lines, 1201 samples */
    };

    /** Value for invalid altitude data */
    static const short INVALID_DATA = -32768;

    /** URL to download HGT files (http://dds.cr.usgs.gov/srtm/version2_1/SRTM3/) */
    static const string URL;

    /** Subfolders of HGT files (Eurasia, North_America, Australia, Islands, South_America, Africa */
    static const string folders[];

    /**
     * @brief Constructs the SRTM class
     * @param model The model defines the precision of the HGT files
     */
    SRTM(SRTMModel model = ThreeArcSecond);
    ~SRTM();

    /**
     * @brief Gets an (interpolated) altitude value for the given coordinates
     * @param latitude Latitude coordinate in decimal degree
     * @param longitude Longitude coordinate in decimal degree
     * @param altitude Altitude from lookup table
     * @return True on success
     */
    bool getAltitude(double latitude, double longitude, short &altitude);

    /**
     * @brief Returns the HGT file name of the last getAltitude() call
     * @return HGT file name
     */
    string getFileName() const;

    /**
     * @brief Returns the URL to download the HGT file of the last getAltitude() call
     * @return HGT download URL
     */
    string getFileURL() const;

private:
    const int tileSize;
    int    iLatitude;
    int    iLongitude;
    bool   failed;
    short* lookup;

    /**
     * @brief Parses a HGT file and store content in the lookup table
     * @return True on success
     */
    bool parseHGT();

    /**
     * @brief Checks endianess of platform
     * @return True if platform is little endian
     */
    bool isLittleEndian();

    /**
     * @brief Performs a bilinear interpolation on the given altitude array
     * @param inAltitudes Array of four altitude values on corners of square (bl, br, tl, tr)
     * @param inX x coordinate
     * @param inY y coordinate
     * @return  interpolated altitude
     */
    short bilinearInterpolate(short *inAltitudes, double inX, double inY);

    /**
     * @brief Calculates the average of the non-void altitudes in the given array
     * @param inAltitudes Array of altitudes with one or more invalid values
     * @return Average of non-void altitudes
     */
    short averageNonVoid(short *inAltitudes);

    /**
     * @brief Fixes a single void in the given array by replacing it with the average of the others
     * @param inAltitudes Array of altitudes containing one void
     * @return  fixed array without voids
     */
    short *fixVoid(short *inAltitudes);
};

/** @} Functions */

#endif // _SRTM_H_
