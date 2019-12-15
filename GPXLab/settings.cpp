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

#include <QSettings>
#include <QDir>
#include <QStandardPaths>
#include "settings.h"

Settings::Settings(QMainWindow *parent) :
    undoLimit(50),
    maxRecentFiles(5),
    parent(parent)
{
    // save default window layout
    defaultState = parent->saveState();
    defaultGeometry = parent->saveGeometry();
}

void Settings::load()
{
    QSettings qsettings;
    parent->restoreGeometry(qsettings.value("geometry").toByteArray());
    parent->restoreState(qsettings.value("windowState").toByteArray());
    recentFiles = qsettings.value("recentFileList").toStringList();
    doPersistentCaching = qsettings.value("doPersistentCaching", true).toBool();
    cachePath = qsettings.value("cachePath", "").toString();
    tilesURL = qsettings.value("tilesURL", "").toString();
    if (cachePath.isEmpty())
        cachePath = defaultCachePath();
    autoLoadLastFile = qsettings.value("autoLoadLastFile", true).toBool();
    checkUpdate = qsettings.value("checkUpdate", true).toBool();
    checkUpdateUrl = qsettings.value("checkUpdateUrl", "https://api.github.com/repos/bourgeoislab/gpxlab/releases/latest").toString();
    checkUpdateLastDate = qsettings.value("checkUpdateLastDate", QDate()).toDate();
    if (tilesURL.isEmpty())
        tilesURL = defaultTilesURL();
    emit settingsChanged(true);
}

void Settings::save()
{
    QSettings qsettings;
    qsettings.setValue("geometry", parent->saveGeometry());
    qsettings.setValue("windowState", parent->saveState());
    qsettings.setValue("recentFileList", recentFiles);
    qsettings.setValue("doPersistentCaching", doPersistentCaching);
    qsettings.setValue("cachePath", cachePath);
    qsettings.setValue("autoLoadLastFile", autoLoadLastFile);
    qsettings.setValue("checkUpdate", checkUpdate);
    qsettings.setValue("checkUpdateLastDate", checkUpdateLastDate);
    qsettings.setValue("tilesURL", tilesURL);
    emit settingsChanged(false);
}

void Settings::restoreLayout()
{
    // restore default window layout
    parent->restoreGeometry(defaultGeometry);
    parent->restoreState(defaultState);
}

void Settings::setValue(const QString &key, const QVariant &value)
{
    QSettings qsettings;
    qsettings.setValue(key, value);
    emit settingsChanged(true);
}

QVariant Settings::getValue(const QString &key)
{
    QSettings qsettings;
    return qsettings.value(key);
}

//////////////////////////////////////////////////////////////////////////////

void Settings::addToRecentFile(const QString &fileName)
{
    recentFiles.removeAll(fileName);
    recentFiles.prepend(fileName);
    while (recentFiles.size() > maxRecentFiles)
        recentFiles.removeLast();
}

void Settings::removeFromRecentFile(const QString &fileName)
{
    recentFiles.removeAll(fileName);
}

//////////////////////////////////////////////////////////////////////////////

void Settings::clearCache()
{
    QDir cache(cachePath);

    cache.setFilter( QDir::NoDotAndDotDot | QDir::Files );
    foreach(QString dirItem, cache.entryList())
        cache.remove(dirItem);

    cache.setFilter( QDir::NoDotAndDotDot | QDir::Dirs );
    foreach( QString dirItem, cache.entryList())
    {
        QDir subDir( cache.absoluteFilePath( dirItem ) );
        subDir.removeRecursively();
    }
}

QString Settings::defaultCachePath()
{
    return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
}

QString Settings::defaultTilesURL()
{
    return QString("http://tile.openstreetmap.org/%1/%2/%3.png");
}
