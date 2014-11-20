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

#ifndef _QTABLEWIDGETPOINTS_H_
#define _QTABLEWIDGETPOINTS_H_

#include <QTableView>
#include <QAbstractTableModel>
#include "gpx_wrapper.h"

/**
 * @class PointTableModel
 *
 * @brief Model for point table
 *
 * Implements the QAbstractTableModel abstract class.
 *
 * The data for the table is taken directly from the GPX_model.
 *
 * @see QAbstractTableModel
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 14 Nov 2014
 */
class PointTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    PointTableModel(QObject *parent = 0);

    /**
     * @brief Returns the number of rows ( = number of track points)
     * @param parent Parent
     * @return Number of rows
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;

    /**
     * @brief Returns the number of columns
     * @param parent Parent
     * @return Number of columns
     */
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * @brief Returns the data stored under the given role for the item referred to by the index.
     * @param index Index
     * @param role Role
     * @return Data
     */
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    /**
     * @brief Returns the data for the given role and section in the header with the specified orientation.
     * @param section Section
     * @param orientation Orientation
     * @param role Role
     * @return Data
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    /**
     * @brief Initializes the model
     * @param gpxmw GPX_model wrapper
     */
    void init(const GPX_wrapper *gpxmw);
private:
    const GPX_wrapper *gpxmw;
    int numberRows;
};

/**
 * @class QTableWidgetPoints
 *
 * @brief QTableWidget subclass
 *
 * Extends the QTableWidget class.
 *
 * @see QTableWidget
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 14 Nov 2014
 */
class QTableWidgetPoints : public QTableView
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    explicit QTableWidgetPoints(QWidget *parent = 0);

    /**
     * @brief Generates table
     * @param gpxmw GPX_model wrapper
     */
    void build(const GPX_wrapper *gpxmw);

    /**
     * @brief Clears the table
     */
    void clear();

    /**
     * @brief Selects the row
     * @param rowNumber Row number
     */
    void selectRow(int rowNumber);

    /**
     * @brief Gets selected row
     * @return Selected row number
     */
    int getSelectedRow();

signals:

    /**
     * @brief Signal when the selection changed
     * @param rowNumber New selected row
     */
    void selectionChanged(int rowNumber);

private slots:
    void selectionChangedSlot(const QItemSelection  &selected, const QItemSelection  &deselected );
    void on_actionDateTime_toggled(bool show);
    void on_actionTimeElapsed_toggled(bool show);
    void on_actionDistance_toggled(bool show);
    void on_actionLegLength_toggled(bool show);
    void on_actionSpeed_toggled(bool show);
    void on_actionElevation_toggled(bool show);
    void on_actionCoordinates_toggled(bool show);
    void on_actionHeading_toggled(bool show);
    void on_actionFix_toggled(bool show);
    void on_actionSatellites_toggled(bool show);
    void on_actionMagVar_toggled(bool show);
    void on_actionHDOP_toggled(bool show);
    void on_actionVDOP_toggled(bool show);
    void on_actionPDOP_toggled(bool show);
    void on_actionAgeOfData_toggled(bool show);
    void on_actionDGPSID_toggled(bool show);
    void on_actionGeoidHeight_toggled(bool show);
    void on_actionName_toggled(bool show);
    void on_actionComment_toggled(bool show);
    void on_actionDescription_toggled(bool show);
    void on_actionSource_toggled(bool show);
    void on_actionSymbol_toggled(bool show);
    void on_actionType_toggled(bool show);

private:
    const char **pLabels;
};

#endif // QTABLEWIDGETPOINTS_H
