#ifndef GPXFILE_H
#define GPXFILE_H

#include <fstream>
#include "gpx_model.h"

using namespace std;

/**
 * @ingroup GPX_model
 * @{
 */

/**
 * @namespace GPXFile
 *
 * @brief Functions to load and save GPX files.
 *
 * This file provides two functions to read and write a GPX file.
 * While reading the GPX file a GPX_model structure is filled.
 * In the other way a GPX file is written from a GPX_model structure.
 *
 * @see http://www.topografix.com/gpx.asp
 */
namespace GPXFile
{
    /**
     * @brief Parses a GPX file and fills the GPX model
     * @param fp File handler to the opened GPX file
     * @param gpxm GPX_model
     * @param overwriteMetadata If true the metadata of GPX_model is overwritten
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e load(ifstream* fp, GPX_model* gpxm, bool overwriteMetadata = false);

    /**
     * @brief Saves a GPX file representing the GPX model
     * @param fp File handler to the opened GPX file
     * @param gpxm GPX model to write
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e save(ofstream* fp, const GPX_model* gpxm);
}

/** @} GPX_model */

#endif // GPXFILE_H
