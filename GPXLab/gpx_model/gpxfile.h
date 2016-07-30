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

#ifndef _GPXFILE_H_
#define _GPXFILE_H_

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
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.4
 * @date 30 Jul 2016
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

#endif // _GPXFILE_H_
