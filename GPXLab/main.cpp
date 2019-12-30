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
