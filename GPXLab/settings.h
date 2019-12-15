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

#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @ingroup GPXLab
 * @{
 */

#include <QObject>
#include <QMainWindow>
#include <QString>
#include <QDate>

/**
 * @class Settings
 *
 * @brief Manage the settings
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.3
 * @date 22 Aug 2016
 */
class Settings : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     */
    Settings(QMainWindow *parent);

    /**
     * @brief Saves the settings
     */
    void save();

    /**
     * @brief Saves the settings
     */
    void load();

    /**
     * @brief Restores the window layout
     */
    void restoreLayout();

    /**
     * @brief Sets a settings
     * @param key Key
     * @param value Value
     */
    void setValue(const QString &key, const QVariant &value);

    /**
     * @brief Gets a settings
     * @param key Key
     * @return Value
     */
    QVariant getValue(const QString &key);

    /**
     * @brief Adds a file to the recent file list
     * @param fileName File name
     */
    void addToRecentFile(const QString &fileName);

    /**
     * @brief Removes a file from the recent file list
     * @param fileName File name
     */
    void removeFromRecentFile(const QString &fileName);

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
     * @brief Returns the default tiles URL
     * @return Default tiles URL
     */
    QString defaultTilesURL();

    /**
     * @brief Maximal number of undo commands stored
     */
    const int undoLimit;

    /**
     * @brief Maximal number of recent files shown in the file menu
     */
    const int maxRecentFiles;

    /**
     * @brief List of the recent files shown in the file menu
     */
    QStringList recentFiles;

    /**
     * @brief Do persistent caching of map tiles
     */
    bool doPersistentCaching;

    /**
     * @brief Path to the tiles cache directory
     */
    QString cachePath;

    /**
     * @brief Loads the last opened file on startup
     */
    bool autoLoadLastFile;

    /**
     * @brief Checks for update at the start of the program
     */
    bool checkUpdate;

    /**
     * @brief URL to check for updates
     */
    QString checkUpdateUrl;

    /**
     * @brief Last date of the update check
     */
    QDate checkUpdateLastDate;

    /**
     * @brief Tiles URL
     */
    QString tilesURL;

signals:

    /**
     * @brief Signal when settings are loaded or saved
     */
    void settingsChanged(bool loaded);

private:

    QMainWindow *parent;
    QByteArray  defaultState;
    QByteArray  defaultGeometry;
};

/** @} GPXLab */

#endif // SETTINGS_H
