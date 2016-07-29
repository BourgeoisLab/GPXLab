QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): cache()

ORGANISATION = BourgeoisLab
TARGET = GPXLab
VERSION = 0.4.0.0
TEMPLATE = app

DEFINES += ORGANISATION=\\\"$$ORGANISATION\\\" TARGET=\\\"$$TARGET\\\" VERSION=\\\"$$VERSION\\\"

win32:RC_ICONS += gpxlab.ico

OBJECTS_DIR = tmp
MOC_DIR = tmp
UI_DIR = tmp
DESTDIR = ../bin

win32 {
  LIBS += -L../bin/ -lqmapcontrol0
}
else {
  LIBS += -L../bin/ -lqmapcontrol
}

INCLUDEPATH += ../QMapControl/ gpx_model/ commands/ functions/ dialogs/ widgets/

SOURCES += main.cpp\
    gpxlab.cpp \
    gpxlab_actions.cpp \
    qutils.cpp \
    gpx_wrapper.cpp \
    settings.cpp \
    gpx_model/gpx_model.cpp \
    gpx_model/gpxfile.cpp \
    gpx_model/uxmlpars.c \
    gpx_model/nmeafile.cpp \
    gpx_model/actfile.cpp \
    functions/srtm.cpp \
    widgets/qcustomplot.cpp \
    widgets/qcustomplotext.cpp \
    widgets/qdiagramwidget.cpp \
    widgets/qtablewidgetpoints.cpp \
    widgets/qtreewidgettracks.cpp \
    widgets/qmapwidget.cpp \
    widgets/qmaptrack.cpp \
    widgets/qcalendarwidgetext.cpp \
    dialogs/dialog_modelproperties.cpp \
    dialogs/dialog_trackproperties.cpp \
    dialogs/dialog_srtm.cpp \
    dialogs/dialog_about.cpp \
    dialogs/dialog_settings.cpp \
    dialogs/dialog_datetime.cpp \
    commands/selectcommand.cpp \
    commands/editfilepropertiescommand.cpp \
    commands/edittrackpropertiescommand.cpp \
    commands/setaltitudecommand.cpp \
    commands/movetrackdowncommand.cpp \
    commands/movetrackupcommand.cpp \
    commands/removetrackcommand.cpp \
    commands/appendtrackcommand.cpp \
    commands/splittrackcommand.cpp \
    commands/combinetrackcommand.cpp \
    commands/pointdeletecommand.cpp \
    commands/pointinsertcommand.cpp \
    commands/tracktimeshiftcommand.cpp \
    commands/pointeditcommand.cpp

HEADERS  += \
    gpxlab.h \
    qutils.h \
    gpx_wrapper.h \
    settings.h \
    gpx_model/gpx_model.h \
    gpx_model/uxmlpars.h \
    gpx_model/gpxfile.h \
    gpx_model/nmeafile.h \
    gpx_model/actfile.h \
    functions/srtm.h \
    widgets/qcustomplot.h \
    widgets/qcustomplotext.h \
    widgets/qdiagramwidget.h \
    widgets/qtablewidgetpoints.h \
    widgets/qtreewidgettracks.h \
    widgets/qmapwidget.h \
    widgets/qmaptrack.h \
    widgets/qcalendarwidgetext.h \
    dialogs/dialog_modelproperties.h \
    dialogs/dialog_trackproperties.h \
    dialogs/dialog_srtm.h \
    dialogs/dialog_about.h \
    dialogs/dialog_settings.h \
    dialogs/dialog_datetime.h \
    commands/selectcommand.h \
    commands/editfilepropertiescommand.h \
    commands/edittrackpropertiescommand.h \
    commands/setaltitudecommand.h \
    commands/movetrackdowncommand.h \
    commands/movetrackupcommand.h \
    commands/removetrackcommand.h \
    commands/appendtrackcommand.h \
    commands/splittrackcommand.h \
    commands/combinetrackcommand.h \
    commands/pointdeletecommand.h \
    commands/pointinsertcommand.h \
    commands/tracktimeshiftcommand.h \
    commands/pointeditcommand.h

FORMS    += \
    gpxlab.ui \
    dialogs/dialog_modelproperties.ui \
    dialogs/dialog_trackproperties.ui \
    dialogs/dialog_srtm.ui \
    dialogs/dialog_about.ui \
    dialogs/dialog_settings.ui \
    dialogs/dialog_datetime.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
