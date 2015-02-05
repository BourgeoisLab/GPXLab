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

#ifndef _GPX_MODEL_H_
#define _GPX_MODEL_H_

#include <string>
#include <vector>

using namespace std;

/**
 * @defgroup GPX_model GPX model
 * @brief Model representation of a GPX file
 *
 * This model can be used to represent a GPX file (GPS exchange format).
 * The structure is based on the GPX version 1.1.
 *
 * The model is divided in tracks (GPX_trkType), track segments
 * (GPX_trksegType) and track points (GPX_wptType).
 *
 * The model is represented by a tree like this:
 * <PRE>
 * GPX_model
 *   creator
 *   GPX_metadataType
 *   GPX_extensionsType [0...*]
 *   GPX_statsType [1]
 *   GPX_trkType [0...*]
 *     GPX_trkMetadataType [1]
 *     GPX_extensionsType [0...*]
 *     GPX_statsType [1]
 *     GPX_trksegType [0...*]
 *       GPX_extensionsType [0...*]
 *       GPX_statsType [1]
 *       GPX_wptType [0...*]
 * </PRE>
 *
 * @see http://www.topografix.com/gpx.asp
 *
 * @{
 */

class GPX_trkType;
class GPX_trksegType;
class GPX_wptType;

/**
 * @brief Information about the copyright holder and any license governing use of the GPX file
 */
struct GPX_copyrightType
{
    string                  author;             /**< Copyright holder */
    string                  year;               /**< Year of copyright */
    string                  license;            /**< Link to external file containing license text */
};

/**
 * @brief Representation of a link to an external resource
 */
struct GPX_linkType
{
    string                  href;               /**< URL of the link */
    string                  text;               /**< Text of the link */
    string                  type;               /**< Mime type of content (text/html) */
};

/**
 * @brief Representation of an email address
 */
struct GPX_emailType
{
    string                  id;                 /**< ID half of email address (bourgeoislab) */
    string                  domain;             /**< Domain half of email address (gmail.com) */
};

/**
 * @brief Representation of a person or organization
 */
struct GPX_personType
{
    string                  name;               /**< Name of person or organization */
    GPX_emailType           email;              /**< Email address */
    GPX_linkType            link;               /**< Link to Web site or other external information */
};

/**
 * @brief Bound given by minimal and maximal latitude / longitude
 */
struct GPX_boundsType
{
    double                  minlat;             /**< Minimum latitude [deg] */
    double                  minlon;             /**< Minimum longitude [deg] */
    double                  maxlat;             /**< Maximum latitude [deg] */
    double                  maxlon;             /**< Maximum longitude [deg] */
};

/**
 * @brief Any extension to the standard GPX format
 */
struct GPX_extType
{
    vector <string>         extension;          /**< Any extension element */
};

/**
* @brief Garmin's TrackPointExtension GPX extension
*/
struct GPX_extTPExtType
{
    unsigned int            heartrate;          /**< Heart rate [bpm] */
    vector <string>         other;              /**< Any other extension element */
};

/**
 * @brief Information about the GPX file
 */
struct GPX_metadataType
{
    string                  name;               /**< Name of the GPX file */
    string                  desc;               /**< Description of the contents of the GPX file */
    GPX_personType          author;             /**< Person or organization who created the GPX file */
    GPX_copyrightType       copyright;          /**< Copyright and license information governing use of the file */
    vector <GPX_linkType>   links;              /**< Links associated with the location described in the file */
    time_t                  timestamp;          /**< GMT creation date of the file [s] */
    int                     millisecond;        /**< GMT creation date of the file (0-999) [ms] */
    string                  keywords;           /**< Keywords associated with the file */
    GPX_boundsType          bounds;             /**< Minimum and maximum coordinates */
    GPX_extType          extensions;         /**< Extensions */
};

/**
 * @brief Information about a track
 */
struct GPX_trkMetadataType
{
    string                  name;               /**< Name of the track */
    string                  cmt;                /**< Comment for track */
    string                  desc;               /**< Description of track */
    string                  src;                /**< Source of data */
    vector <GPX_linkType>   links;              /**< Links to external information about track */
    size_t                  number;             /**< Track number */
    string                  type;               /**< Type (classification) of the track */
};

/**
 * @brief Statistic information about a list of track points
 */
struct GPX_statsType
{
    int                     points;             /**< Number of track points within the list */
    time_t                  startTime;          /**< GMT timestamp of the first point [s] */
    time_t                  endTime;            /**< GMT timestamp of the last point [s] */
    unsigned int            duration;           /**< Time elapsed between first and last point [s] */
    double                  distance;           /**< Distance covered [km] */
    float                   speed;              /**< Average speed [km/h] */
    float                   minhei;             /**< Minimal height [m] */
    float                   maxhei;             /**< Maximal height [m] */
    float                   heightIntUp;        /**< Height integral uphill */
    float                   heightIntDown;      /**< Height integral downhill */
    GPX_boundsType          bounds;             /**< Minimum and maximum coordinates */
};

/**
 * @class GPX_model
 *
 * @brief Class representing the content of a GPX file
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 10 Jan 2015
 */
class GPX_model
{
public:

    /**
     * @brief Return code
     */
    enum retCode_e
    {
        GPXM_OK = 0,                            /**< Success */
        GPXM_ERR_INVALID_ARG = -1,              /**< Invalid argument */
        GPXM_ERR_FILEOPEN = -2,                 /**< Failed to open file (read or write) */
        GPXM_ERR_FAILED = -3,                   /**< Operation failed */
        GPXM_ERR_OUTOFMEM = -4,                 /**< Out of memory */
        GPXM_WARN_NOTHING_CHANGED = 1           /**< Function call had no effect */
    };

    /**
     * @brief File Type
     */
    enum fileType_e
    {
        GPXM_FILE_AUTOMATIC,                    /**< Automatic detection */
        GPXM_FILE_NOT_SUPPORTED,                /**< Not supported */
        GPXM_FILE_GPX,                          /**< GPX file */
        GPXM_FILE_NMEA,                         /**< NMEA file */
        GPXM_FILE_ACT                           /**< ACT file */
    };

    const string            creator;            /**< Creator of the GPX file */
    GPX_metadataType        metadata;           /**< Information about the GPX file */
    GPX_extType             extensions;         /**< Extensions */
    GPX_statsType           stats;              /**< Statistic information */
    vector <GPX_trkType>    trk;                /**< A list of tracks */

    /**
     * @brief Gets the type of a file
     * @param fileName File
     * @return File type
     */
    static fileType_e getFileType(const string& fileName);

    /**
     * @brief Construcst a GPX model representing the content of a GPX file
     * @param creator Creator of the model
     */
    GPX_model(const string& creator);

    /**
     * @brief Constructs a GPX model representing the content of a GPX file
     * @param creator Creator of the model
     * @param fileName File to parse and add to the model
     */
    GPX_model(const string& creator, const string& fileName);

    /**
     * @brief Parses a file and add its content to the model
     * @param fileName File to parse
     * @param fileType Specify type of file, GPXM_FILE_AUTOMATIC for automatic detection
     * @param overwriteMetadata If true the model metadata is overwritten with the metadata of the new file
     * @return Return code, GPXM_OK on success
     */
    retCode_e load(const string& fileName, fileType_e fileType = GPXM_FILE_AUTOMATIC, bool overwriteMetadata = false);

    /**
     * @brief Saves the content of the model to a file
     * @param fileName File name
     * @return Return code, GPXM_OK on success
     */
    retCode_e save(const string& fileName);

    /**
     * @brief Updates the properties of the model
     * @note Should be called when the model was changed in any way
     * @param propagate If true also update the properties of
     * the tracks, track segments and track points
     */
    void update(bool propagate = true);

    /**
     * @brief Updates the properties of a given track
     * @note Should be called if the track was modified in any way
     * @param track Track to update
     * @param propagate If true also update the properties of
     * the track segments and track points
     */
    void updateTrack(GPX_trkType &track, bool propagate = true);

private:

    /**
     * @brief Clears the metadata of the model
     */
    void clearMetadata();
};

/**
 * @class GPX_trkType
 *
 * @brief Track class holding a list of track segments
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 10 Jan 2015
 */
class GPX_trkType
{
public:
    friend class GPX_model;

    GPX_trkMetadataType     metadata;           /**< Information about the track */
    GPX_extType             extensions;         /**< Extensions */
    GPX_statsType           stats;              /**< Statistic information */
    vector <GPX_trksegType> trkseg;             /**< List of track segments */

    /**
     * @brief Constructs a new track
     * @param number Track number
     */
    GPX_trkType(size_t number);

private:

    /**
     * @brief Updates the properties of the track
     * @param propagate If true also update the properties of
     * the track segments and track points
     */
    void update(bool propagate);
};

/**
 * @class GPX_trksegType
 *
 * @brief Track segment class holding a list of points which are logically connected in order
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 10 Jan 2015
 */
class GPX_trksegType
{
public:
    friend class GPX_trkType;

    GPX_extType             extensions;         /**< Extensions */
    GPX_statsType           stats;              /**< Statistic information */
    vector <GPX_wptType>    trkpt;              /**< List of track points */

    /**
     * @brief Constructs a new track segment
     */
    GPX_trksegType();

private:

    /**
     * @brief Updates the properties of the track segment
     * @param propagate If true also update the properties of the track points
     * @param trackStartTime Timestamp of the beginning of the track (used for elapsedTime)
     * @param prevWpt Pointer to last track point of the previous track segment
     */
    void update(bool propagate, time_t trackStartTime, const GPX_wptType* prevWpt);
};

/**
 * @class GPX_wptType
 *
 * @brief Track point class representing a single GPS localization point
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.1
 * @date 10 Jan 2015
 */
class GPX_wptType
{
public:
    friend class GPX_trksegType;

    string                  fix;                /**< Type of fix ("none", "2d", "3d", "dgps" or "pps") */
    int                     sat;                /**< Number of satellites used to calculate the fix */
    time_t                  timestamp;          /**< GMT timestamp [s] */
    int                     millisecond;        /**< GMT timestamp milliseconds (0-999) [ms] */
    float                   magvar;             /**< Magnetic variation at the point [deg] */
    double                  altitude;           /**< Elevation of the point [m] */
    float                   hdop;               /**< Horizontal dilution of precision */
    float                   vdop;               /**< Vertical dilution of precision */
    float                   pdop;               /**< Position dilution of precision */
    float                   ageofdgpsdata;      /**< Number of seconds since last DGPS update */
    int                     dgpsid;             /**< ID of DGPS station used in differential correction */
    float                   geoidheight;        /**< Height of geoid above WGS84 earth ellipsoid [m] */
    double                  latitude;           /**< Latitude of the point [deg] */
    double                  longitude;          /**< Longitude of the point [deg] */
    string                  name;               /**< Name of the track point */
    string                  cmt;                /**< Comment about the point */
    string                  desc;               /**< Description of the point */
    string                  src;                /**< Source of data */
    vector <GPX_linkType>   links;              /**< Links to additional information about the point */
    string                  sym;                /**< Symbol name */
    string                  type;               /**< Type (classification) of the point */
    GPX_extType             extensions;         /**< Extensions */
    GPX_extTPExtType        extensionsGarmin;   /**< Garmin extensions */
    float                   speed;              /**< Ground speed [km/h] */
    float                   heading;            /**< Heading angle [deg] */
    double                  leglength;          /**< Distance from the previous point [m] */
    double                  distanceTot;        /**< Distance from first point [km] */
    unsigned int            elapsedTime;        /**< Time elapsed since start [s] */

    /**
     * @brief Constructs a new track point
     */
    GPX_wptType();

    /**
     * @brief Clears all properties
     */
    void clear();

    /**
     * @brief Gets the timestamp
     * @return Timestamp
     */
    double getTime() const;

    /**
     * @brief Checks if the reference track point has the same time
     * @param wptRef Reference point
     * @return True if both have the same time
     */
    bool isSameTime(const GPX_wptType &wptRef) const;

    /**
     * @brief Returns a human readable string representing the heading
     * @return Cardinal string
     */
    const string& cardinal() const;

private:

    /**
     * @brief Updates the properties of the track point
     * @param trackStartTime Timestamp of the beginning of the track (used for elapsedTime)
     * @param prevWpt Pointer to previous track point (used for distance and speed)
     * @param nextWpt Pointer to next track point (used for heading)
     */
    void update(time_t trackStartTime, const GPX_wptType *prevWpt, const GPX_wptType *nextWpt);

    /**
     * @brief Calculates the distance from a given point and set the member distance.
     * @param latitudeFrom Latitude of the point
     * @param longitudeFrom Longitude of the point
     * @see http://www.movable-type.co.uk/scripts/latlong.html
     */
    void setDistance(double latitudeFrom, double longitudeFrom);

    /**
     * @brief Calculates the heading to a given point and set the member heading.
     * @param latitudeTo Latitude of the point
     * @param longitudeTo Longitude of the point
     * @see http://www.movable-type.co.uk/scripts/latlong.html
     */
    void setHeading(double latitudeTo, double longitudeTo);
};

/** @} GPX_model */

#endif // _GPX_MODEL_H_
