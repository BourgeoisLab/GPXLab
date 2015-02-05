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

#ifndef QCALENDARWIDGETEXT_H
#define QCALENDARWIDGETEXT_H

#include <QCalendarWidget>
#include <QTextCharFormat>
#include "gpx_wrapper.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QCalendarWidgetExt
 *
 * @brief QCalendarWidget subclass
 *
 * Extends the QCalendarWidget class.
 *
 * @see QCalendarWidget
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.0
 * @date 11 Jan 2015
 */
class QCalendarWidgetExt : public QCalendarWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    QCalendarWidgetExt(QWidget *parent = 0);
    ~QCalendarWidgetExt();

    /**
     * @brief Initializes the widget
     * @note Call this function prior to any other function
     * @param gpxmw GPX_model wrapper
     */
    void init(const GPX_wrapper *gpxmw);

    /**
     * @brief Builds the widget
     */
    void build();

    /**
     * @brief Clears the widget
     */
    void clear();

    /**
     * @brief Selects a date corresponding to the track
     * @param trackNumber Track number
     */
    void select(int trackNumber);

signals:

    /**
     * @brief Signal when a new date was selected with a track on this day
     * @see selectionChangedExt()
     * @param trackNumber Track number
     */
    void selectionChanged(int trackNumber);

private slots:

    void selectionChangedExt();

private:

    const GPX_wrapper *gpxmw;
    QTextCharFormat format;
    bool selecting;
};

/** @} Widgets */

#endif // QCALENDARWIDGETEXT_H
