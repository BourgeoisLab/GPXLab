#ifndef ACTFILE_H
#define ACTFILE_H

#include <fstream>
#include "gpx_model.h"

using namespace std;

/**
 * @ingroup GPX_model
 * @{
 */

/**
 * @namespace ACTFile
 *
 * @brief Functions to load and save ACT files.
 *
 * This file provides one function to read a ACT file.
 * While reading the ACT file a GPX_model structure is filled.
 *
 * @see http://www.a-rival.de
 */
namespace ACTFile
{
    /**
     * @brief Parses a ACT file and fills the GPX model
     * @param fp File handler to the opened GPX file
     * @param gpxm GPX_model
     * @return Return code, GPXM_OK on success
     */
    GPX_model::retCode_e load(ifstream* fp, GPX_model* gpxm);
}

/** @} GPX_model */

#endif // ACTFILE_H
