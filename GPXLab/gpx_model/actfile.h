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

#ifndef _ACTFILE_H_
#define _ACTFILE_H_

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
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 10 Jan 2015
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

#endif // _ACTFILE_H_
