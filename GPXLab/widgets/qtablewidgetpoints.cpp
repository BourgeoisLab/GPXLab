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
 
#include <QAction>
#include <QHeaderView>
#include <QDateTime>
#include "qtablewidgetpoints.h"
#include "QUtils.h"

////////////////////////////////////////////////////////////////////////////////

// Build full table of only shown columns
#define TABLE_BUILD_FULL            0

// Number of columns
#define TABLE_NUM_COLS              23

// Column number (unique)
#define TABLE_COL_DATETIME          0
#define TABLE_COL_ELAPSED           1
#define TABLE_COL_DISTANCE          2
#define TABLE_COL_LEGLENGTH         3
#define TABLE_COL_SPEED             4
#define TABLE_COL_ELEVATION         5
#define TABLE_COL_COORDINATES       6
#define TABLE_COL_HEADING           7
#define TABLE_COL_FIX               8
#define TABLE_COL_SATELLITES        9
#define TABLE_COL_MAGVAR            10
#define TABLE_COL_HDOP              11
#define TABLE_COL_VDOP              12
#define TABLE_COL_PDOP              13
#define TABLE_COL_AGEOFDATA         14
#define TABLE_COL_DGPSID            15
#define TABLE_COL_GEOIDHEIGHT       16
#define TABLE_COL_NAME              17
#define TABLE_COL_COMMENT           18
#define TABLE_COL_DESCRIPTION       19
#define TABLE_COL_SOURCE            20
#define TABLE_COL_SYMBOL            21
#define TABLE_COL_TYPE              22

// Column heading text
#define TABLE_COL_LABEL_DATETIME    "Date & Time"
#define TABLE_COL_LABEL_ELAPSED     "Elapsed time\n[s]"
#define TABLE_COL_LABEL_DISTANCE    "Distance\n[km]"
#define TABLE_COL_LABEL_LEGLENGTH   "Leg length\n[m]"
#define TABLE_COL_LABEL_SPEED       "Speed\n[km/h]"
#define TABLE_COL_LABEL_ELEVATION   "Elevation\n[m]"
#define TABLE_COL_LABEL_COORDINATES "Coodinates\n[deg]"
#define TABLE_COL_LABEL_HEADING     "Heading"
#define TABLE_COL_LABEL_FIX         "Fix"
#define TABLE_COL_LABEL_SATELLITES  "Satellites"
#define TABLE_COL_LABEL_MAGVAR      "Magnetic\nvariation"
#define TABLE_COL_LABEL_HDOP        "HDOP"
#define TABLE_COL_LABEL_VDOP        "VDOP"
#define TABLE_COL_LABEL_PDOP        "PDOP"
#define TABLE_COL_LABEL_AGEOFDATA   "Age of\nDGPS"
#define TABLE_COL_LABEL_DGPSID      "DGPS\nID"
#define TABLE_COL_LABEL_GEOIDHEIGHT "Geoid height\n[m]"
#define TABLE_COL_LABEL_NAME        "Name"
#define TABLE_COL_LABEL_COMMENT     "Comment"
#define TABLE_COL_LABEL_DESCRIPTION "Description"
#define TABLE_COL_LABEL_SOURCE      "Source"
#define TABLE_COL_LABEL_SYMBOL      "Symbol"
#define TABLE_COL_LABEL_TYPE        "Type"

////////////////////////////////////////////////////////////////////////////////

PointTableModel::PointTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    numberRows(0)
{
}

int PointTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return numberRows;
}

int PointTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return TABLE_NUM_COLS;
}

QVariant PointTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            switch (section)
            {
            case TABLE_COL_DATETIME:
                return QString(TABLE_COL_LABEL_DATETIME);
            case TABLE_COL_ELAPSED:
                return QString(TABLE_COL_LABEL_ELAPSED);
            case TABLE_COL_DISTANCE:
                return QString(TABLE_COL_LABEL_DISTANCE);
            case TABLE_COL_LEGLENGTH:
                return QString(TABLE_COL_LABEL_LEGLENGTH);
            case TABLE_COL_SPEED:
                return QString(TABLE_COL_LABEL_SPEED);
            case TABLE_COL_ELEVATION:
                return QString(TABLE_COL_LABEL_ELEVATION);
            case TABLE_COL_COORDINATES:
                return QString(TABLE_COL_LABEL_COORDINATES);
            case TABLE_COL_HEADING:
                return QString(TABLE_COL_LABEL_HEADING);
            case TABLE_COL_FIX:
                return QString(TABLE_COL_LABEL_FIX);
            case TABLE_COL_SATELLITES:
                return QString(TABLE_COL_LABEL_SATELLITES);
            case TABLE_COL_MAGVAR:
                return QString(TABLE_COL_LABEL_MAGVAR);
            case TABLE_COL_HDOP:
                return QString(TABLE_COL_LABEL_HDOP);
            case TABLE_COL_VDOP:
                return QString(TABLE_COL_LABEL_VDOP);
            case TABLE_COL_PDOP:
                return QString(TABLE_COL_LABEL_PDOP);
            case TABLE_COL_AGEOFDATA:
                return QString(TABLE_COL_LABEL_AGEOFDATA);
            case TABLE_COL_DGPSID:
                return QString(TABLE_COL_LABEL_DGPSID);
            case TABLE_COL_GEOIDHEIGHT:
                return QString(TABLE_COL_LABEL_GEOIDHEIGHT);
            case TABLE_COL_NAME:
                return QString(TABLE_COL_LABEL_NAME);
            case TABLE_COL_COMMENT:
                return QString(TABLE_COL_LABEL_COMMENT);
            case TABLE_COL_DESCRIPTION:
                return QString(TABLE_COL_LABEL_DESCRIPTION);
            case TABLE_COL_SOURCE:
                return QString(TABLE_COL_LABEL_SOURCE);
            case TABLE_COL_SYMBOL:
                return QString(TABLE_COL_LABEL_SYMBOL);
            case TABLE_COL_TYPE:
                return QString(TABLE_COL_LABEL_TYPE);
            }
        }
        else if (orientation == Qt::Vertical)
        {
            return QString::number(section);
        }
    }
    return QVariant();
}

QVariant PointTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        if (gpxmw)
        {
            const GPX_wptType *wpt = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), index.row());
            if (wpt)
            {
                QDateTime timestamp;
                QString sTmp;
                switch(index.column())
                {
                case TABLE_COL_DATETIME:
                    timestamp.setTime_t(wpt->timestamp);
                    sTmp = timestamp.toString("dd.MM.yyyy H:mm:ss");
                    if (wpt->millisecond)
                        sTmp += "." + QString("%1").arg(wpt->millisecond, 3, 10, QChar('0'));
                    return sTmp;
                case TABLE_COL_ELAPSED:
                    return QUtils::seconds_to_DHMS(wpt->elapsedTime);
                case TABLE_COL_DISTANCE:
                    return QString::number(wpt->distanceTot, 'f', 1);
                case TABLE_COL_LEGLENGTH:
                    return QString::number(wpt->distance*1000, 'f', 1);
                case TABLE_COL_SPEED:
                    return QString::number(wpt->speed, 'f', 1);
                case TABLE_COL_ELEVATION:
                    return QString::number(wpt->altitude, 'f', 0);
                case TABLE_COL_COORDINATES:
                    sTmp = QString::number(wpt->latitude, 'f', 6) + " / " + QString::number(wpt->longitude, 'f', 6);
                    return sTmp;
                case TABLE_COL_HEADING:
                    return QString::fromStdString(wpt->cardinal());
                case TABLE_COL_FIX:
                    return QString::fromStdString(wpt->fix);
                case TABLE_COL_SATELLITES:
                    return QString::number(wpt->sat);
                case TABLE_COL_MAGVAR:
                    return QString::number(wpt->magvar, 'f', 1);
                case TABLE_COL_HDOP:
                    return QString::number(wpt->hdop, 'f', 1);
                case TABLE_COL_VDOP:
                    return QString::number(wpt->vdop, 'f', 1);
                case TABLE_COL_PDOP:
                    return QString::number(wpt->pdop, 'f', 1);
                case TABLE_COL_AGEOFDATA:
                    return QString::number(wpt->ageofdgpsdata, 'f', 1);
                case TABLE_COL_DGPSID:
                    return QString::number(wpt->dgpsid);
                case TABLE_COL_GEOIDHEIGHT:
                    return QString::number(wpt->geoidheight, 'f', 1);
                case TABLE_COL_NAME:
                    return QString::fromStdString(wpt->name);
                case TABLE_COL_COMMENT:
                    return QString::fromStdString(wpt->cmt);
                case TABLE_COL_DESCRIPTION:
                    return QString::fromStdString(wpt->desc);
                case TABLE_COL_SOURCE:
                    return QString::fromStdString(wpt->src);
                case TABLE_COL_SYMBOL:
                    return QString::fromStdString(wpt->sym);
                case TABLE_COL_TYPE:
                    return QString::fromStdString(wpt->type);
                }
            }
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    return QVariant();
}

void PointTableModel::init(const GPX_wrapper *gpxmw)
{
    beginResetModel();
    this->gpxmw = gpxmw;
    if (gpxmw)
        numberRows = gpxmw->getNumPoints(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
    else
        numberRows = 0;
    endResetModel();
}

////////////////////////////////////////////////////////////////////////////////

QTableWidgetPoints::QTableWidgetPoints(QWidget *parent) :
    QTableView(parent)
{
    QAction *menuActions[TABLE_NUM_COLS];

    // build label array
    pLabels = new const char*[TABLE_NUM_COLS];
    pLabels[TABLE_COL_DATETIME] = TABLE_COL_LABEL_DATETIME;
    pLabels[TABLE_COL_ELAPSED] = TABLE_COL_LABEL_ELAPSED;
    pLabels[TABLE_COL_DISTANCE] = TABLE_COL_LABEL_DISTANCE;
    pLabels[TABLE_COL_LEGLENGTH] = TABLE_COL_LABEL_LEGLENGTH;
    pLabels[TABLE_COL_SPEED] = TABLE_COL_LABEL_SPEED;
    pLabels[TABLE_COL_ELEVATION] = TABLE_COL_LABEL_ELEVATION;
    pLabels[TABLE_COL_COORDINATES] = TABLE_COL_LABEL_COORDINATES;
    pLabels[TABLE_COL_HEADING] = TABLE_COL_LABEL_HEADING;
    pLabels[TABLE_COL_FIX] = TABLE_COL_LABEL_FIX;
    pLabels[TABLE_COL_SATELLITES] = TABLE_COL_LABEL_SATELLITES;
    pLabels[TABLE_COL_MAGVAR] = TABLE_COL_LABEL_MAGVAR;
    pLabels[TABLE_COL_HDOP] = TABLE_COL_LABEL_HDOP;
    pLabels[TABLE_COL_VDOP] = TABLE_COL_LABEL_VDOP;
    pLabels[TABLE_COL_PDOP] = TABLE_COL_LABEL_PDOP;
    pLabels[TABLE_COL_AGEOFDATA] = TABLE_COL_LABEL_AGEOFDATA;
    pLabels[TABLE_COL_DGPSID] = TABLE_COL_LABEL_DGPSID;
    pLabels[TABLE_COL_GEOIDHEIGHT] = TABLE_COL_LABEL_GEOIDHEIGHT;
    pLabels[TABLE_COL_NAME] = TABLE_COL_LABEL_NAME;
    pLabels[TABLE_COL_COMMENT] = TABLE_COL_LABEL_COMMENT;
    pLabels[TABLE_COL_DESCRIPTION] = TABLE_COL_LABEL_DESCRIPTION;
    pLabels[TABLE_COL_SOURCE] = TABLE_COL_LABEL_SOURCE;
    pLabels[TABLE_COL_SYMBOL] = TABLE_COL_LABEL_SYMBOL;
    pLabels[TABLE_COL_TYPE] = TABLE_COL_LABEL_TYPE;

    // insert columns and create action for context menu
    for (int i = 0; i < TABLE_NUM_COLS; ++i)
    {
        menuActions[i] = new QAction(pLabels[i], this);
        menuActions[i]->setCheckable(true);
        horizontalHeader()->addAction(menuActions[i]);
    }

    // activate context menu for heading
    horizontalHeader()->setContextMenuPolicy(Qt::ActionsContextMenu);

    // set context menu action signals
    connect(menuActions[TABLE_COL_DATETIME], SIGNAL(toggled(bool)), this, SLOT(on_actionDateTime_toggled(bool)));
    connect(menuActions[TABLE_COL_ELAPSED], SIGNAL(toggled(bool)), this, SLOT(on_actionTimeElapsed_toggled(bool)));
    connect(menuActions[TABLE_COL_DISTANCE], SIGNAL(toggled(bool)), this, SLOT(on_actionDistance_toggled(bool)));
    connect(menuActions[TABLE_COL_LEGLENGTH], SIGNAL(toggled(bool)), this, SLOT(on_actionLegLength_toggled(bool)));
    connect(menuActions[TABLE_COL_SPEED], SIGNAL(toggled(bool)), this, SLOT(on_actionSpeed_toggled(bool)));
    connect(menuActions[TABLE_COL_ELEVATION], SIGNAL(toggled(bool)), this, SLOT(on_actionElevation_toggled(bool)));
    connect(menuActions[TABLE_COL_COORDINATES], SIGNAL(toggled(bool)), this, SLOT(on_actionCoordinates_toggled(bool)));
    connect(menuActions[TABLE_COL_HEADING], SIGNAL(toggled(bool)), this, SLOT(on_actionHeading_toggled(bool)));
    connect(menuActions[TABLE_COL_FIX], SIGNAL(toggled(bool)), this, SLOT(on_actionFix_toggled(bool)));
    connect(menuActions[TABLE_COL_SATELLITES], SIGNAL(toggled(bool)), this, SLOT(on_actionSatellites_toggled(bool)));
    connect(menuActions[TABLE_COL_MAGVAR], SIGNAL(toggled(bool)), this, SLOT(on_actionMagVar_toggled(bool)));
    connect(menuActions[TABLE_COL_HDOP], SIGNAL(toggled(bool)), this, SLOT(on_actionHDOP_toggled(bool)));
    connect(menuActions[TABLE_COL_VDOP], SIGNAL(toggled(bool)), this, SLOT(on_actionVDOP_toggled(bool)));
    connect(menuActions[TABLE_COL_PDOP], SIGNAL(toggled(bool)), this, SLOT(on_actionPDOP_toggled(bool)));
    connect(menuActions[TABLE_COL_AGEOFDATA], SIGNAL(toggled(bool)), this, SLOT(on_actionAgeOfData_toggled(bool)));
    connect(menuActions[TABLE_COL_DGPSID], SIGNAL(toggled(bool)), this, SLOT(on_actionDGPSID_toggled(bool)));
    connect(menuActions[TABLE_COL_GEOIDHEIGHT], SIGNAL(toggled(bool)), this, SLOT(on_actionGeoidHeight_toggled(bool)));
    connect(menuActions[TABLE_COL_NAME], SIGNAL(toggled(bool)), this, SLOT(on_actionName_toggled(bool)));
    connect(menuActions[TABLE_COL_COMMENT], SIGNAL(toggled(bool)), this, SLOT(on_actionComment_toggled(bool)));
    connect(menuActions[TABLE_COL_DESCRIPTION], SIGNAL(toggled(bool)), this, SLOT(on_actionDescription_toggled(bool)));
    connect(menuActions[TABLE_COL_SOURCE], SIGNAL(toggled(bool)), this, SLOT(on_actionSource_toggled(bool)));
    connect(menuActions[TABLE_COL_SYMBOL], SIGNAL(toggled(bool)), this, SLOT(on_actionSymbol_toggled(bool)));
    connect(menuActions[TABLE_COL_TYPE], SIGNAL(toggled(bool)), this, SLOT(on_actionType_toggled(bool)));

    // set default column state (show/hidden)
    menuActions[TABLE_COL_DATETIME]->setChecked(true);
    menuActions[TABLE_COL_ELAPSED]->setChecked(true);
    menuActions[TABLE_COL_DISTANCE]->setChecked(true);
    menuActions[TABLE_COL_LEGLENGTH]->setChecked(true);
    menuActions[TABLE_COL_SPEED]->setChecked(true);
    menuActions[TABLE_COL_ELEVATION]->setChecked(true);
    menuActions[TABLE_COL_COORDINATES]->setChecked(true);
    menuActions[TABLE_COL_HEADING]->setChecked(true);
    menuActions[TABLE_COL_FIX]->setChecked(false);
    menuActions[TABLE_COL_SATELLITES]->setChecked(false);
    menuActions[TABLE_COL_MAGVAR]->setChecked(false);
    menuActions[TABLE_COL_HDOP]->setChecked(false);
    menuActions[TABLE_COL_VDOP]->setChecked(false);
    menuActions[TABLE_COL_PDOP]->setChecked(false);
    menuActions[TABLE_COL_AGEOFDATA]->setChecked(false);
    menuActions[TABLE_COL_DGPSID]->setChecked(false);
    menuActions[TABLE_COL_GEOIDHEIGHT]->setChecked(false);
    menuActions[TABLE_COL_NAME]->setChecked(false);
    menuActions[TABLE_COL_COMMENT]->setChecked(false);
    menuActions[TABLE_COL_DESCRIPTION]->setChecked(false);
    menuActions[TABLE_COL_SOURCE]->setChecked(false);
    menuActions[TABLE_COL_SYMBOL]->setChecked(false);
    menuActions[TABLE_COL_TYPE]->setChecked(false);

    // create model
    setModel(new PointTableModel(this));

    // set column width
    setColumnWidth(TABLE_COL_DATETIME, 130);
    setColumnWidth(TABLE_COL_ELAPSED, 80);
    setColumnWidth(TABLE_COL_DISTANCE, 60);
    setColumnWidth(TABLE_COL_LEGLENGTH, 80);
    setColumnWidth(TABLE_COL_SPEED, 60);
    setColumnWidth(TABLE_COL_ELEVATION, 60);
    setColumnWidth(TABLE_COL_COORDINATES, 130);
    setColumnWidth(TABLE_COL_HEADING, 60);
    setColumnWidth(TABLE_COL_FIX, 60);
    setColumnWidth(TABLE_COL_SATELLITES, 60);
    setColumnWidth(TABLE_COL_MAGVAR, 60);
    setColumnWidth(TABLE_COL_HDOP, 60);
    setColumnWidth(TABLE_COL_VDOP, 60);
    setColumnWidth(TABLE_COL_PDOP, 60);
    setColumnWidth(TABLE_COL_AGEOFDATA, 60);
    setColumnWidth(TABLE_COL_DGPSID, 60);
    setColumnWidth(TABLE_COL_GEOIDHEIGHT, 80);
    setColumnWidth(TABLE_COL_NAME, 130);
    setColumnWidth(TABLE_COL_COMMENT, 130);
    setColumnWidth(TABLE_COL_DESCRIPTION, 130);
    setColumnWidth(TABLE_COL_SOURCE, 130);
    setColumnWidth(TABLE_COL_SYMBOL, 130);
    setColumnWidth(TABLE_COL_TYPE, 130);

    // hide columns
    for (int i = 0; i < TABLE_NUM_COLS; ++i)
    {
        if (horizontalHeader()->actions().at(i)->isChecked() == false)
            hideColumn(i);
    }

    connect(selectionModel(),SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),this,SLOT(selectionChangedSlot(const QItemSelection&,const QItemSelection&)));

}

void QTableWidgetPoints::build(const GPX_wrapper *gpxmw)
{
    PointTableModel *m = dynamic_cast<PointTableModel*>(model());
    if (m)
        m->init(gpxmw);

    // scroll to top
    scrollToTop();
}

void QTableWidgetPoints::clear()
{
    build(NULL);
}

void QTableWidgetPoints::selectionChangedSlot(const QItemSelection  &selected, const QItemSelection  &deselected )
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    emit(selectionChanged(getSelectedRow()));
}

void QTableWidgetPoints::selectRow(int rowNumber)
{
    QTableView::selectRow(rowNumber);
}

int QTableWidgetPoints::getSelectedRow()
{
    return selectionModel()->selectedRows().at(0).row();
}

void QTableWidgetPoints::on_actionDateTime_toggled(bool show)
{
    show ? showColumn(TABLE_COL_DATETIME) : hideColumn(TABLE_COL_DATETIME);
}

void QTableWidgetPoints::on_actionTimeElapsed_toggled(bool show)
{
    show ? showColumn(TABLE_COL_ELAPSED) : hideColumn(TABLE_COL_ELAPSED);
}

void QTableWidgetPoints::on_actionDistance_toggled(bool show)
{
    show ? showColumn(TABLE_COL_DISTANCE) : hideColumn(TABLE_COL_DISTANCE);
}

void QTableWidgetPoints::on_actionLegLength_toggled(bool show)
{
    show ? showColumn(TABLE_COL_LEGLENGTH) : hideColumn(TABLE_COL_LEGLENGTH);
}

void QTableWidgetPoints::on_actionSpeed_toggled(bool show)
{
    show ? showColumn(TABLE_COL_SPEED) : hideColumn(TABLE_COL_SPEED);
}

void QTableWidgetPoints::on_actionElevation_toggled(bool show)
{
    show ? showColumn(TABLE_COL_ELEVATION) : hideColumn(TABLE_COL_ELEVATION);
}

void QTableWidgetPoints::on_actionCoordinates_toggled(bool show)
{
    show ? showColumn(TABLE_COL_COORDINATES) : hideColumn(TABLE_COL_COORDINATES);
}

void QTableWidgetPoints::on_actionHeading_toggled(bool show)
{
    show ? showColumn(TABLE_COL_HEADING) : hideColumn(TABLE_COL_HEADING);
}

void QTableWidgetPoints::on_actionFix_toggled(bool show)
{
    show ? showColumn(TABLE_COL_FIX) : hideColumn(TABLE_COL_FIX);
}

void QTableWidgetPoints::on_actionSatellites_toggled(bool show)
{
    show ? showColumn(TABLE_COL_SATELLITES) : hideColumn(TABLE_COL_SATELLITES);
}

void QTableWidgetPoints::on_actionMagVar_toggled(bool show)
{
    show ? showColumn(TABLE_COL_MAGVAR) : hideColumn(TABLE_COL_MAGVAR);
}

void QTableWidgetPoints::on_actionHDOP_toggled(bool show)
{
    show ? showColumn(TABLE_COL_HDOP) : hideColumn(TABLE_COL_HDOP);
}

void QTableWidgetPoints::on_actionVDOP_toggled(bool show)
{
    show ? showColumn(TABLE_COL_VDOP) : hideColumn(TABLE_COL_VDOP);
}

void QTableWidgetPoints::on_actionPDOP_toggled(bool show)
{
    show ? showColumn(TABLE_COL_PDOP) : hideColumn(TABLE_COL_PDOP);
}

void QTableWidgetPoints::on_actionAgeOfData_toggled(bool show)
{
    show ? showColumn(TABLE_COL_AGEOFDATA) : hideColumn(TABLE_COL_AGEOFDATA);
}

void QTableWidgetPoints::on_actionDGPSID_toggled(bool show)
{
    show ? showColumn(TABLE_COL_DGPSID) : hideColumn(TABLE_COL_DGPSID);
}

void QTableWidgetPoints::on_actionGeoidHeight_toggled(bool show)
{
    show ? showColumn(TABLE_COL_GEOIDHEIGHT) : hideColumn(TABLE_COL_GEOIDHEIGHT);
}

void QTableWidgetPoints::on_actionName_toggled(bool show)
{
    show ? showColumn(TABLE_COL_NAME) : hideColumn(TABLE_COL_NAME);
}

void QTableWidgetPoints::on_actionComment_toggled(bool show)
{
    show ? showColumn(TABLE_COL_COMMENT) : hideColumn(TABLE_COL_COMMENT);
}

void QTableWidgetPoints::on_actionDescription_toggled(bool show)
{
    show ? showColumn(TABLE_COL_DESCRIPTION) : hideColumn(TABLE_COL_DESCRIPTION);
}

void QTableWidgetPoints::on_actionSource_toggled(bool show)
{
    show ? showColumn(TABLE_COL_SOURCE) : hideColumn(TABLE_COL_SOURCE);
}

void QTableWidgetPoints::on_actionSymbol_toggled(bool show)
{
    show ? showColumn(TABLE_COL_SYMBOL) : hideColumn(TABLE_COL_SYMBOL);
}

void QTableWidgetPoints::on_actionType_toggled(bool show)
{
    show ? showColumn(TABLE_COL_TYPE) : hideColumn(TABLE_COL_TYPE);
}
