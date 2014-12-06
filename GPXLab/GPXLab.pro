QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): cache()

TARGET = GPXLab
VERSION = 0.2.0.0
TEMPLATE = app

win32:RC_ICONS += gpxlab.ico

MOC_DIR = tmp
OBJECTS_DIR = obj
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
    gpx_model/gpx_model.cpp \
    gpx_model/gpxfile.cpp \
    gpx_model/uxmlpars.c \
    gpx_model/nmeafile.cpp \
    functions/srtm.cpp \
    widgets/qcustomplot.cpp \
    widgets/qcustomplotext.cpp \
    widgets/qdiagramwidget.cpp \
    widgets/qtablewidgetpoints.cpp \
    widgets/qtreewidgettracks.cpp \
    widgets/qmapwidget.cpp \
    widgets/linestringext.cpp \
    dialogs/dialog_modelproperties.cpp \
    dialogs/dialog_trackproperties.cpp \
    dialogs/dialog_srtm.cpp \
    dialogs/dialog_about.cpp \
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
    dialogs/dialog_settings.cpp \
    settings.cpp

HEADERS  += \
    gpxlab.h \
    qutils.h \
    gpx_wrapper.h \
    gpx_model/gpx_model.h \
    gpx_model/uxmlpars.h \
    gpx_model/gpxfile.h \
    gpx_model/nmeafile.h \
    functions/srtm.h \
    widgets/qcustomplot.h \
    widgets/qcustomplotext.h \
    widgets/qdiagramwidget.h \
    widgets/qtablewidgetpoints.h \
    widgets/qtreewidgettracks.h \
    widgets/qmapwidget.h \
    widgets/linestringext.h \
    dialogs/dialog_modelproperties.h \
    dialogs/dialog_trackproperties.h \
    dialogs/dialog_srtm.h \
    dialogs/dialog_about.h \
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
    dialogs/dialog_settings.h \
    settings.h

FORMS    += \
    gpxlab.ui \
    dialogs/dialog_modelproperties.ui \
    dialogs/dialog_trackproperties.ui \
    dialogs/dialog_srtm.ui \
    dialogs/dialog_about.ui \
    dialogs/dialog_settings.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
