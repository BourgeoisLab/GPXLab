#ifndef GPX_WRAPPER_H
#define GPX_WRAPPER_H

#include <QObject>
#include <QString>
#include <QVector>
#include "gpx_model.h"

/**
 * @defgroup GPX_wrapper GPX model wrapper
 * @brief Qt wrapper for the GPX model
 * @{
 */

/**
 * @class GPX_wrapper
 *
 * @brief Qt wrapper class for GPX_model
 *
 * Qt wrapper class for GPX_model.
 *
 * All interactions to the GPX_model are performed over this class.
 *
 * @see GPX_Model
 */
class GPX_wrapper : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Enumeration of track point properties
     */
    enum TrackPointProperty
    {
        none = 0x00000000,
        fix = 0x00000001,
        sat = 0x00000002,
        timestamp = 0x00000004,
        magvar = 0x00000008,
        altitude = 0x00000010,
        hdop = 0x00000020,
        vdop = 0x00000040,
        pdop = 0x00000080,
        ageofdgpsdata = 0x00000100,
        dgpsid = 0x00000200,
        geoidheight = 0x00000400,
        latitude = 0x00000800,
        longitude = 0x00001000,
        name = 0x00002000,
        cmt = 0x00004000,
        desc = 0x00008000,
        src = 0x00010000,
        links = 0x00020000,
        sym = 0x00040000,
        type = 0x00080000,
        heartrate = 0x00100000,
        speed = 0x00200000,
        heading = 0x00400000,
        distance = 0x00800000,
        leglength = 0x01000000,
        elapsedTime = 0x02000000
    };

public:

    /**
     * @brief Constructs a GPX_model wrapper
     * @param creator Creator of the model
     */
    GPX_wrapper(const QString& creator);
    ~GPX_wrapper();

    /**
     * @brief Initializes the GPX_model
     * @param creator Creator of the model
     */
    void init(const QString& creator);

    /**
     * @brief Clears the underlying GPX_model
     * @note Call init() to re-initialize a GPX_model
     */
    void clear();

    /**
     * @brief Parses a file and add its content to the model
     * @param fileName File to parse
     * @param fileType File type, GPXM_FILE_AUTOMATIC for automatic detection
     * @param overwriteMetadata If true the model metadata is overwritten with the metadata of the new file
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e load(const QString &fileName, GPX_model::fileType_e fileType, bool overwriteMetadata);

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
     * @brief Sets the metadata of the model
     * @param metadata New metadata
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setModelMetadata(const GPX_metadataType &metadata);

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
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setTrackMetadata(int trackNumber, const GPX_trkMetadataType &metadata);

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
     * @brief Gets a track (read-only)
     * @param trackNumber Track number
     * @return Track, nullptr if invalid
     */
    const GPX_trkType *getTrack(int trackNumber) const;

    /**
     * @brief Gets a track segment (read-only)
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @return Track segment, nullptr if invalid
     */
    const GPX_trksegType *getTrackSegment(int trackNumber, int trackSegmentNumber) const;

    /**
     * @brief Gets a track point (read-only)
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 for whole track
     * @param pointNumber Point number starting from the given track segment
     * or first segment if trackSegmentNumber = -1
     * @return Track point, nullptr if invalid
     */
    const GPX_wptType *getPoint(int trackNumber, int trackSegmentNumber, int pointNumber) const;

    /**
     * @brief Gets the label of a track point property
     * @param property Property
     * @param newLines Set true if label should have multiple lines
     * @return Property label
     */
    QString getTrackPointPropertyLabel(TrackPointProperty property, bool newLines = false) const;

    /**
     * @brief Gets a track point property as double value
     * @note Not all properties can be represented as double value
     * @param trkpt Track point
     * @param property Property
     * @return Property as double value
     */
    double getTrackPointPropertyAsDouble(const GPX_wptType *trkpt, TrackPointProperty property) const;

    /**
     * @brief Gets a track point property as string value
     * @param trkpt Track point
     * @param property Property
     * @return Property as string value
     */
    QString getTrackPointPropertyAsString(const GPX_wptType *trkpt, TrackPointProperty property) const;

    /**
     * @brief Sets a track point property
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param numProperties Number of properties to set
     * @param property Properties to set
     * @param value Value as string
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setTrackPointProperty(int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, TrackPointProperty property[], QString value[]);

    /**
     * @brief Sets a track point property
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param numProperties Number of properties to set
     * @param property Properties to set
     * @param value Value as double
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setTrackPointProperty(int trackNumber, int trackSegmentNumber, int pointNumber, size_t numProperties, TrackPointProperty property[], double value[]);

    /**
     * @brief Gets the number of points in a track or track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 for whole track
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
     * @brief Inserts a track
     * @param trackNumber Insertion place
     * @param trk Track to add
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e insertTrack(int trackNumber, const GPX_trkType &trk);

    /**
     * @brief Moves a track upward
     * @param trackNumber Track number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e moveTrackUp(int &trackNumber);

    /**
     * @brief Moves a track downward
     * @param trackNumber Track number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e moveTrackDown(int &trackNumber);

    /**
     * @brief Splits a track in two track segments
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 for whole track
     * @param pointNumber Point number at which the track is split
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e splitTrack(int trackNumber, int &trackSegmentNumber, int &pointNumber);

    /**
     * @brief Combines a track from two track segments
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, if -1 for whole track
     * @param pointNumber Point number at which the track is combined
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e combineTrack(int trackNumber, int &trackSegmentNumber, int &pointNumber);

    /**
     * @brief Deletes a point from a track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e deletePoint(int trackNumber, int &trackSegmentNumber, int pointNumber);

    /**
     * @brief Inserts a point into a track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param wpt Point to insert
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e insertPoint(int trackNumber, int &trackSegmentNumber, int pointNumber, const GPX_wptType &wpt);

    /**
     * @brief Time shifts a track
     * @param trackNumber Track number
     * @param seconds Amount of seconds to shift
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e timeShiftTrack(int trackNumber, long seconds);

    /**
     * @brief Change the selection
     * @param trackNumber Selected track number
     * @param trackSegmentNumber Selected track segment number, -1 for only track selection
     * @param pointNumber Selected point number, -1 for only track selection
     * @param forceSelect Force to select, even if the selection is the same
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e select(int trackNumber, int trackSegmentNumber = -1, int pointNumber = -1, bool forceSelect = false);

    /**
     * @brief Change the selection
     * @param trackNumber Selected track number
     * @param trackSegmentNumber Selected track segment number, -1 for only track selection
     * @param timestamp Select point with this timestamp
     * @param forceSelect Force to select, even if the selection is the same
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e select(int trackNumber, int trackSegmentNumber, time_t timestamp, bool forceSelect = false);

    /**
     * @brief Change the selection
     * @param trackNumber Selected track number
     * @param trackSegmentNumber Selected track segment number, -1 for only track selection
     * @param lat Select point with this latitude value
     * @param lon Select point with this longitude value
     * @param forceSelect Force to select, even if the selection is the same
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e select(int trackNumber, int trackSegmentNumber, double lat, double lon, bool forceSelect = false);

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
     * @brief Gets the selected track point item number
     * @return Track point item number, -1 if no point selected
     */
    int getSelectedPointNumber() const;

    /**
     * @brief Gets the selected track point
     * @return Track point, nullptr if no point selected
     */
    const GPX_wptType *getSelectedPoint() const;

    /**
     * @brief Generates the values for a track or track segment used for the diagram
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number, -1 for the track
     * @param curveMain Property for the main curve
     * @param curveSecondary Property for the secondary curve
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e generateDiagramValues(int trackNumber, int trackSegmentNumber, TrackPointProperty curveMain, TrackPointProperty curveSecondary) const;

    /**
     * @brief Gets the time values generated with generateDiagramValues()
     * @return Time values
     */
    const QVector<double> &getTimeValues() const;

    /**
     * @brief Gets the altitude values generated with generateDiagramValues()
     * @return Altitude values
     */
    const QVector<double> &getAltitudeValues() const;

    /**
     * @brief Gets the speed values generated with generateDiagramValues()
     * @return Speed values
     */
    const QVector<double> &getCurveMainValues() const;

    /**
     * @brief Gets the distance values generated with generateDiagramValues()
     * @return Distance values
     */
    const QVector<double> &getCurveSecondaryValues() const;

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
     * @return Next track, nullptr if iteration finished
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
     * @return Next track segment, nullptr if iteration finished
     */
    const GPX_trksegType *getNextTrkseg() const;

    /**
     * @brief Initializes a track point iteration
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @return Number of track points, GPXM_ERR_FAILED if failed
     */
    int initTrkptIteration(int trackNumber, int trackSegmentNumber) const;

    /**
     * @brief Gets the next track point from the iteration
     * @return Next track point, nullptr if iteration finished
     */
    const GPX_wptType* getNextTrkpt() const;

signals:

    /**
     * @brief Signal when a file is successfully loaded
     * @see load()
     */
    void fileLoaded();

    /**
     * @brief Signal when a file is successfully saved
     * @see save()
     */
    void fileSaved();

    /**
     * @brief Signal when the GPX_model is cleared
     * @see clear()
     */
    void modelCleared();

    /**
     * @brief Signal when properties of the GPX_model changed
     * @see setAltitudeValues()
     */
    void modelPropertiesChanged();

    /**
     * @brief Signal when the metadata of the GPX_model changed
     * @see setModelMetadata()
     */
    void modelMetadataChanged();

    /**
     * @brief Signal when the metadata of a track changed
     * @param trackNumber Track number
     * @see setTrackMetadata()
     */
    void trackMetadataChanged(int trackNumber);

    /**
     * @brief Signal when a track was inserted
     * @param trackNumber Track Number
     * @param track Track
     * @see insertTrack()
     */
    void trackInserted(int trackNumber, const GPX_trkType &track);

    /**
     * @brief Signal when a track was deleted
     * @param trackNumber Track number
     * @see removeTrack()
     */
    void trackDeleted(int trackNumber);

    /**
     * @brief Signal when a track was moved up
     * @param trackNumber Track number
     * @see moveTrackUp()
     */
    void trackMovedUp(int trackNumber);

    /**
     * @brief Signal when a track was moved down
     * @param trackNumber Track number
     * @see moveTrackDown()
     */
    void trackMovedDown(int trackNumber);

    /**
     * @brief Signal when a track was splited
     * @param trackNumber Track number
     * @see splitTrack()
     */
    void trackSplited(int trackNumber);

    /**
     * @brief Signal when two track segments were combined
     * @param trackNumber Track number
     * @see combineTrack()
     */
    void trackCombined(int trackNumber);

    /**
     * @brief Signal when a track was time shifted
     * @param trackNumber Track number
     * @param seconds Amounts of seconds shifted
     * @see timeShiftTrack()
     */
    void trackTimeShifted(int trackNumber, long seconds);

    /**
     * @brief Signal when a new track was selected
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number, -1 for no point selected
     * @param wpt Point
     * @see select()
     */
    void trackSelectionChanged(int trackNumber, int trackSegmentNumber, int pointNumber, const GPX_wptType *wpt);

    /**
     * @brief Signal when a point was edited
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param property Property that changed
     * @see setTrackPointProperty()
     */
    void pointEdited(int trackNumber, int trackSegmentNumber, int pointNumber, GPX_wrapper::TrackPointProperty property);

    /**
     * @brief Signal when a point was inserted
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @param wpt Point
     * @see insertPoint()
     */
    void pointInserted(int trackNumber, int trackSegmentNumber, int pointNumber, const GPX_wptType& wpt);

    /**
     * @brief Signal when a point was deleted
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @param pointNumber Point number
     * @see deletePoint()
     */
    void pointDeleted(int trackNumber, int trackSegmentNumber, int pointNumber);

    /**
     * @brief Signal when a new point was selected
     * @param pointNumber Point number
     * @param wpt Point
     * @see select()
     */
    void pointSelectionChanged(int pointNumber, const GPX_wptType *wpt);

private:

    /**
     * @brief Updates all track numbers
     */
    void updateTrackNumbers();

    /**
     * @brief Internally selects a track or track segment
     * @param trackNumber Track number
     * @param trackSegmentNumber Track segment number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedTrack(int trackNumber, int trackSegmentNumber, bool forceSelect);

    /**
     * @brief Internally selects a track point from the selected track or track segment
     * given its item number
     * @param pointNumber Item number
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedPointByNumber(int pointNumber, bool forceSelect);

    /**
     * @brief Internally selects a track point from the selected track or track segment
     * given a latitude / longitude coordinate
     * @param lat Latitude in decimal degree
     * @param lon Longitude in decimal degree
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedPointByCoord(double lat, double lon, bool forceSelect);

    /**
     * @brief Internally selects a track point from the selected track or track segment
     * given a timestamp
     * @param timestamp Timestamp
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e setSelectedPointByTimestamp(time_t timestamp, bool forceSelect);

private:

    GPX_model* gpxm;
    QString fileName;
    int selectedTrackNumber;
    int selectedTrackSegmentNumber;
    int selectedPointNumber;
    const GPX_wptType *selectedPoint;
    mutable QVector<double> timeValues;
    mutable QVector<double> altitudeValues;
    mutable QVector<double> curveMainValues;
    mutable QVector<double> curveSecondaryValues;
    mutable vector<GPX_trkType>::const_iterator itTrk;
    mutable vector<GPX_trkType>::const_iterator itTrkEnd;
    mutable vector<GPX_trksegType>::const_iterator itTrkseg;
    mutable vector<GPX_trksegType>::const_iterator itTrksegEnd;
    mutable vector<GPX_wptType>::const_iterator itTrkpt;
    mutable vector<GPX_wptType>::const_iterator itTrkptEnd;
};

/** @} GPX_wrapper */

#endif // GPX_WRAPPER_H
