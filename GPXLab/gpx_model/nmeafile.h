#ifndef NMEAFILE_H
#define NMEAFILE_H

#include <string>
#include <fstream>
#include "gpx_model.h"

using namespace std;

/**
 * @ingroup GPX_model
 * @{
 */

/**
 * @namespace NMEAFile
 *
 * @brief Functions to load NMEA files.
 *
 * This file provides a function to read a NMEA file.
 * While reading the NMEA file a GPX_model structure is filled.
 *
 * @see http://www.gpsinformation.org/dale/nmea.htm
 */
namespace NMEAFile
{
    /**
     * @brief Parses a NMEA file and fills the GPX model structure
     * @param fp File handler to the opened ACT file
     * @param gpxm GPX_model
     * @param name Name of the NMEA file without extension
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e load(ifstream* fp, GPX_model* gpxm, const string& name = "");
}

/** @} GPX_model */

#endif // NMEAFILE_H
