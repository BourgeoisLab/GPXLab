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

#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @ingroup GPXLab
 * @{
 */

/**
 * @class Settings
 *
 * @brief Manage the settings
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 4 Dec 2014
 */
class Settings
{
public:

    /**
     * @brief Constructor
     */
    Settings();

    /**
     * @brief Saves the settings
     */
    void save();

    /**
     * @brief Saves the settings
     */
    void load();

    /**
     * @brief Clears the cache
     */
    void clearCache();

    /**
     * @brief Returns the default cache path
     * @return Default cache path
     */
    QString defaultCachePath();

    /**
     * @brief Do persistent caching of map tiles
     */
    bool doPersistentCaching;

    /**
     * @brief Path to the tiles cache directory
     */
    QString cachePath;
};

/** @} GPXLab */

#endif // SETTINGS_H
