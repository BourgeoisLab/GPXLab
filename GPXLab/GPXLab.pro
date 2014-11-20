QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
greaterThan(QT_MAJOR_VERSION, 4): cache()

TARGET = GPXLab
VERSION = 0.1.0.0
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

INCLUDEPATH += ../QMapControl/ gpx_model/ functions/ dialogs/ widgets/

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
    dialogs/dialog_about.cpp

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
    dialogs/dialog_about.h

FORMS    += \
    gpxlab.ui \
    dialogs/dialog_modelproperties.ui \
    dialogs/dialog_trackproperties.ui \
    dialogs/dialog_srtm.ui \
    dialogs/dialog_about.ui

OTHER_FILES +=

RESOURCES += \
    resources.qrc
