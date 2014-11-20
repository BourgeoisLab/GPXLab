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

#ifndef _GPX_WRAPPER_H_
#define _GPX_WRAPPER_H_

#include <QString>
#include <QVector>
#include "gpx_model.h"

/**
 * @class GPX_wrapper
 *
 * @brief Qt wrapper class for GPX_model.
 *
 * Qt wrapper class for GPX_model.
 *
 * All interactions to the GPX_model are performed over this class.
 *
 * @see GPX_Model
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 31 Oct 2014
 */
class GPX_wrapper
{
public:

    /**
     * @brief Constructs a GPX_Model wrapper.
     * @param creator Creator of the model
     */
    GPX_wrapper(const QString& creator);
    ~GPX_wrapper();

    /**
     * @brief Parses a file and add its content to the model
     * @param fileName File to parse
     * @param overwriteMetadata If true the model metadata is overwritten with the metadata of the new file
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e load(const QString &fileName, bool overwriteMetadata);

    /**
     * @brief Saves the content of the model to a file
     * @param fileName File name
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e save(const QString &fileName);

    /**
     * @brief Gets the file name from a previous load() or save() call
     * @return File name
     */
    const QString &getFileName() const;

    /**
     * @brief Returns true if the GPX_model was modified
     * @note The internal flag is set to false after a save() call
     * @return True if GPX_model was modified
     */
    bool isModified() const;

    /**
     * @brief Gets statistics about the model
     * @return Statistics
     */
    const GPX_statsType* getModelStats() const;

    /**
     * @brief Gets the metadata of the model
     * @return Metadata
     */
    const GPX_metadataType* getModelMetadata() const;

    /**
     * @brief Sest the metadata of the model
     * @param metadata New metadata
     */
    void setModelMetadata(const GPX_metadataType &metadata);

    /**
     * @brief Gets the number of tracks
     * @return Number of tracks
     */
    int getNumTracks() const;

    /**
     * @brief Gets statistics about a track
     * @param trackNumber Track number
     * @return Statistics
     */
    const GPX_statsType* getTrackStats(int trackNumber) const;

    /**
     * @brief Gets the metadata of a track
     * @param trackNumber Track number
     * @return Metadata
     */
    const GPX_trkMetadataType* getTrackMetadata(int trackNumber) const;

    /**
     * @brief Sets the metadata of a track
     * @param trackNumber Track number
     * @param metadata New metadata
     */
    void setTrackMetadata(int trackNumber, const GPX_trkMetadataType &metadata);

    /**
     * @brief Gets statistics about an item (track or track segment)
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number. If -1 return track statistics
     * @return Item statistics
     */
    const GPX_statsType* getItemStats(int trackNumber, int trackSegmentNumber = -1) const;

    /**
     * @brief Gets name of an item (track or track segment)
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number. If -1 return track name
     * @return Item name
     */
    const QString getItemName(int trackNumber, int trackSegmentNumber = -1) const;

    /**
     * @brief Gets a track point
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number. If -1 for whole track
     * @param pointNumber Point number starting from the given track segment
     * or first segment if trackSegmentNumber = -1
     * @return Track point, NULL if invalid
     */
    const GPX_wptType *getPoint(int trackNumber, int trackSegmentNumber, int pointNumber) const;

    /**
     * @brief Gets number of points in the track or track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number. If -1 for whole track
     * @return Number of points
     */
    int getNumPoints(int trackNumber, int trackSegmentNumber) const;

    /**
     * @brief Removes a track
     * @param trackNumber Track number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e removeTrack(int trackNumber);

    /**
     * @brief Moves a track upward
     * @param trackNumber Track number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e moveTrackUp(int trackNumber);

    /**
     * @brief Moves a track downward
     * @param trackNumber Track number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e moveTrackDown(int trackNumber);

    /**
     * @brief Internally selects a track or track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number. If -1 select track
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedTrack(int trackNumber, int trackSegmentNumber = -1);

    /**
     * @brief Gets selected track number
     * @return Track number, -1 if no track selected
     */
    int getSelectedTrackNumber() const;

    /**
     * @brief Gets selected track segment number
     * @return Track segment number, -1 if no track segment selected
     */
    int getSelectedTrackSegmentNumber() const;

    /**
     * @brief Internally selects a track point from the selected track or track segment
     * given its item number
     * @param number Item number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedPointByNumber(int number);

    /**
     * @brief Internally selects a track point from the selected track or track segment
     * given a latitude / longitude coordinate
     * @param lat Latitude in decimal degree
     * @param lon Longitude in decimal degree
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedPointByCoord(double lat, double lon);

    /**
     * @brief Internally selects a track point from the selected track or track segment
     * given a timestamp
     * @param timestamp Timestamp
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedPointByTimestamp(time_t timestamp);

    /**
     * @brief Gets the selected track point
     * @return Track point, NULL if no point selected
     */
    const GPX_wptType *getSelectedPoint() const;

    /**
     * @brief Gets the selected track point item number
     * @return Track point item number, -1 if no point selected
     */
    int getSelectedPointNumber() const;

    /**
     * @brief Generates the values for a track or track segment used for the diagram
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, -1 for the track
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e generateDiagramValues(int trackNumber, int trackSegmentNumber = -1);

    /**
     * @brief Gets the time values generated with generateDiagramValues()
     * @return Time values
     */
    const QVector<double> &getDiagramTimeValues() const;

    /**
     * @brief Gets the altitude values generated with generateDiagramValues()
     * @return Altitude values
     */
    const QVector<double> &getDiagramAltitudeValues() const;

    /**
     * @brief Gets the speed values generated with generateDiagramValues()
     * @return Speed values
     */
    const QVector<double> &getDiagramSpeedValues() const;

    /**
     * @brief Sets new altitude values for a track or track segment
     * @param values Altitude values
     * @param trackNumber Track number
     * @param trackSegmentNumber  Track segment number, -1 for the track
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setAltitudeValues(const QVector<double> &values, int trackNumber, int trackSegmentNumber = -1);

    /**
     * @brief Initializes a track iteration
     * @return Number of tracks, GPXM_ERR_FAILED if failed
     */
    int initTrkIteration() const;

    /**
     * @brief Gets the next track from the iteration
     * @return Next track, NULL if iteration finished
     */
    const GPX_trkType* getNextTrk() const;

    /**
     * @brief Initializes a track segment iteration
     * @param trackNumber Track number
     * @return Number of track segments, GPXM_ERR_FAILED if failed
     */
    int initTrksegIteration(int trackNumber) const;

    /**
     * @brief Gets the next track segment from the iteration
     * @param trackNumber Track number
     * @return Next track segment, NULL if iteration finished
     */
    const GPX_trksegType *getNextTrkseg(int trackNumber) const;

    /**
     * @brief Initializes a track point iteration
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @return Number of track points, GPXM_ERR_FAILED if failed
     */
    int initWptIteration(int trackNumber, int trackSegmentNumber) const;

    /**
     * @brief Gets the next track point from the iteration
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @return Next track point, NULL if iteration finished
     */
    const GPX_wptType* getNextWpt(int trackNumber, int trackSegmentNumber) const;

private:
    GPX_model* gpxm;
    bool modified;
    QString fileName;
    int selectedTrackNumber;
    int selectedTrackSegmentNumber;
    int selectedPointNumber;
    GPX_wptType *selectedPoint;
    mutable vector<GPX_trkType>::const_iterator iterator_trk;
    mutable vector<GPX_trksegType>::const_iterator iterator_trkseg;
    mutable vector<GPX_wptType>::const_iterator iterator_wpt;
    QVector<double> timeValues;
    QVector<double> speedValues;
    QVector<double> altitudeValues;
};

#endif // _GPX_WRAPPER_H_
