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
#include <QAction>
#include <QHeaderView>
#include <QDateTime>
#include "qtablewidgetpoints.h"
#include "QUtils.h"
#include "pointeditcommand.h"

////////////////////////////////////////////////////////////////////////////////

const PointTableModel::column_t PointTableModel::columns[] = {
    {GPX_wrapper::timestamp, true, false, 130},
    {GPX_wrapper::elapsedTime, true, false, 80},
    {GPX_wrapper::distance, true, false, 60},
    {GPX_wrapper::leglength, true, false, 80},
    {GPX_wrapper::speed, true, false, 60},
    {GPX_wrapper::altitude, true, true, 60},
    {GPX_wrapper::latitude, true, true, 80},
    {GPX_wrapper::longitude, true, true, 80},
    {GPX_wrapper::heading, false, false, 60},
    {GPX_wrapper::fix, false, true, 60},
    {GPX_wrapper::sat, false, true, 60},
    {GPX_wrapper::magvar, false, true, 60},
    {GPX_wrapper::hdop, false, true, 60},
    {GPX_wrapper::vdop, false, true, 60},
    {GPX_wrapper::pdop, false, true, 60},
    {GPX_wrapper::ageofdgpsdata, false, true, 60},
    {GPX_wrapper::dgpsid, false, true, 60},
    {GPX_wrapper::geoidheight, false, true, 80},
    {GPX_wrapper::name, false, true, 130},
    {GPX_wrapper::cmt, false, true, 130},
    {GPX_wrapper::desc, false, true, 130},
    {GPX_wrapper::src, false,  true, 130},
    {GPX_wrapper::sym, false, true, 130},
    {GPX_wrapper::type, false, true, 130},
    {GPX_wrapper::heartrate, false, true, 80}
};

PointTableModel::PointTableModel(QObject *parent) :
    QAbstractTableModel(parent),
    numberRows(0),
    numberColumns(0)
{           
    numberColumns = sizeof(columns)/sizeof(column_t);
}

int PointTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return numberRows;
}

int PointTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return numberColumns;
}

QVariant PointTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (orientation == Qt::Horizontal)
        {
            return gpxmw->getTrackPointPropertyLabel(columns[section].property, true);
        }
        else if (orientation == Qt::Vertical)
        {
            return QString::number(section);
        }
        break;

    case Qt::UserRole:
        return columns[section].showByDefault;

    case Qt::UserRole+1:
        return columns[section].size;
    }
    return QVariant();
}

QVariant PointTableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        if (gpxmw)
        {
            const GPX_wptType *trkpt = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), index.row());
            if (trkpt)
            {
                return gpxmw->getTrackPointPropertyAsString(trkpt, columns[index.column()].property);
            }
        }
    }
    else if (role == Qt::TextAlignmentRole)
    {
        return Qt::AlignCenter;
    }
    return QVariant();
}

bool PointTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        GPX_wrapper::TrackPointProperty properties[1] = {columns[index.column()].property};
        QString values[1] = {value.toString()};
        undoStack->push(new PointEditCommand(gpxmw, gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), index.row(), 1, properties, values));
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

Qt::ItemFlags PointTableModel::flags (const QModelIndex &index) const
{
    if (columns[index.column()].editable)
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    else
        return QAbstractItemModel::flags(index);
}

void PointTableModel::init(GPX_wrapper *gpxmw, QUndoStack *undoStack)
{
    this->gpxmw = gpxmw;
    this->undoStack = undoStack;
}

void PointTableModel::build()
{
    beginResetModel();
    numberRows = gpxmw->getNumPoints(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber());
    endResetModel();
}

void PointTableModel::clear()
{
    beginResetModel();
    numberRows = 0;
    endResetModel();
}

int PointTableModel::getNumberRows() const
{
    return numberRows;
}

int PointTableModel::getNumberColumns() const
{
    return numberColumns;
}

////////////////////////////////////////////////////////////////////////////////

QTableWidgetPoints::QTableWidgetPoints(QWidget *parent) :
    QTableView(parent)
{
}

void QTableWidgetPoints::init(GPX_wrapper *gpxmw, QUndoStack *undoStack)
{
    // create model
    PointTableModel *m = new PointTableModel(this);
    setModel(m);
    m->init(gpxmw, undoStack);
    m->build();

    // insert columns and create action for context menu
    QAction *menuAction;
    for (int i = 0; i < m->getNumberColumns(); ++i)
    {
        menuAction = new QAction(m->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString(), this);
        menuAction->setCheckable(true);
        menuAction->setChecked(m->headerData(i, Qt::Horizontal, Qt::UserRole).toBool());
        menuAction->setData(i);
        horizontalHeader()->addAction(menuAction);
        connect(menuAction, SIGNAL(toggled(bool)), this, SLOT(on_actionMenu_toggled(bool)));
        setColumnWidth(i, m->headerData(i, Qt::Horizontal, Qt::UserRole+1).toInt());
        if (horizontalHeader()->actions().at(i)->isChecked() == false)
            hideColumn(i);
    }

    // add action to restore default settings
    QAction *menuActionRestore = new QAction("Restore Default", this);
    QAction *separator = new QAction(this);
    separator->setSeparator(true);
    horizontalHeader()->addAction(separator);
    horizontalHeader()->addAction(menuActionRestore);
    connect(menuActionRestore, SIGNAL(triggered()), this, SLOT(on_actionRestore_triggered()));

    // activate context menu for header
    horizontalHeader()->setContextMenuPolicy(Qt::ActionsContextMenu);

    // connect signal for selection
    connect(selectionModel(),SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)),this,SLOT(selectionChangedExt(const QItemSelection&,const QItemSelection&)));

    // store default state
    defaultState = saveState();

    // load settings
    settingsChanged(true);
}

bool QTableWidgetPoints::restoreState(const QByteArray & state)
{
    if (horizontalHeader()->restoreState(state))
    {
        PointTableModel *m = dynamic_cast<PointTableModel*>(model());
        if (m)
        {
            for (int i = 0; i < m->getNumberColumns(); ++i)
                horizontalHeader()->actions().at(i)->setChecked(!isColumnHidden(i));
        }
        return true;
    }
    return false;
}

QByteArray QTableWidgetPoints::saveState() const
{
    return horizontalHeader()->saveState();
}

void QTableWidgetPoints::build(int selectedRow)
{
    PointTableModel *m = dynamic_cast<PointTableModel*>(model());
    if (m)
    {
        m->build();
        if (selectedRow > 0)
            selectRow(selectedRow);
        else
            scrollToTop();
    }
}

void QTableWidgetPoints::clear()
{
    PointTableModel *m = dynamic_cast<PointTableModel*>(model());
    if (m)
    {
        m->clear();
        scrollToTop();
    }
}

void QTableWidgetPoints::update()
{
    emit model()->dataChanged(QModelIndex(), QModelIndex());
}

int QTableWidgetPoints::getSelectedRow()
{
    return selectionModel()->selectedRows().at(0).row();
}

void QTableWidgetPoints::settingsChanged(bool loaded)
{
    QSettings qsettings;
    if (loaded)
        restoreState(qsettings.value("tableState").toByteArray());
    else
        qsettings.setValue("tableState", saveState());
}

void QTableWidgetPoints::selectionChangedExt(const QItemSelection  &selected, const QItemSelection  &deselected )
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    emit(selectionChanged(getSelectedRow()));
}

void QTableWidgetPoints::on_actionRestore_triggered()
{
    restoreState(defaultState);
}

void QTableWidgetPoints::on_actionMenu_toggled(bool show)
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        show ? showColumn(action->data().toInt()) : hideColumn(action->data().toInt());
    }
}
