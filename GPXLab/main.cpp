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

#include "gpxlab.h"
#include <QApplication>
#include <QLibraryInfo>
#include <QTranslator>
#include <QStandardPaths>

#define TRANSLATIONS_DIR QStandardPaths::locate(QStandardPaths::AppDataLocation, "translations", QStandardPaths::LocateDirectory)

int main(int argc, char *argv[])
{
    GPXLab *gxplab;
    QApplication app(argc, argv);
    app.setOrganizationName(GPXLab::organisationName);
    app.setApplicationName(GPXLab::appName);
    app.setApplicationVersion(GPXLab::appVersion);

    QTranslator gpxlab;
    gpxlab.load(QLocale::system(), "gpxlab", "_", TRANSLATIONS_DIR);
    app.installTranslator(&gpxlab);

    QTranslator qt;
#if defined(Q_OS_WIN32) || defined(Q_OS_MAC)
    qt.load(QLocale::system(), "qt", "_", TRANSLATIONS_DIR);
#else
    qt.load(QLocale::system(), "qt", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath));
#endif
    app.installTranslator(&qt);

    if (argc > 1)
        gxplab = new GPXLab(QString(argv[1]));
    else
        gxplab = new GPXLab();
    gxplab->show();
    return app.exec();
}
