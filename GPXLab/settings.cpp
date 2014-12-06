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

#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include "settings.h"

Settings::Settings()
{
}

void Settings::load()
{
    QSettings qsettings;
    doPersistentCaching = qsettings.value("doPersistentCaching", true).toBool();
    cachePath = qsettings.value("cachePath", "").toString();
    if (cachePath.isEmpty())
        cachePath = defaultCachePath();
}

void Settings::save()
{
    QSettings qsettings;
    qsettings.setValue("doPersistentCaching", doPersistentCaching);
    qsettings.setValue("cachePath", cachePath);
}

void Settings::clearCache()
{
    QDir cache(cachePath);
    cache.setFilter(QDir::Files);
    foreach(QString cacheFile, cache.entryList())
        cache.remove(cacheFile);
}

QString Settings::defaultCachePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}
