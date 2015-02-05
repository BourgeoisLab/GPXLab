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

#ifndef _QDIAGRAMWIDGET_H_
#define _QDIAGRAMWIDGET_H_

#include "qcustomplotext.h"
#include "gpx_wrapper.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QDiagramWidget
 *
 * @brief QCustomPlotExt subclass
 *
 * Extends the QCustomPlotExt class by adding a setData() function.
 *
 * @see QCustomPlotExt
 *
 * @author Frederic Bourgeois <bourgeoislab@gmail.com>
 * @version 1.2
 * @date 4 Jan 2015
 */
class QDiagramWidget : public QCustomPlotExt
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    explicit QDiagramWidget(QWidget *parent = 0);

    /**
     * @brief init
     * @param gpxmw GPX_model wrapper
     */
    void init(const GPX_wrapper *gpxmw);

    /**
     * @brief Sets new diagram values
     */
    void build();

    /**
     * @brief Clears the diagram
     */
    void clear();

    /**
     * @brief Gets the property of the main curve
     * @return Property
     */
    GPX_wrapper::TrackPointProperty getCurveMain() const;

    /**
     * @brief Gets the property of the secondary curve
     * @return Property
     */
    GPX_wrapper::TrackPointProperty getCurveSecondary() const;

private slots:

    void on_actionCurveMain_triggered();
    void on_actionCurveSecondary_triggered();

private:

    const GPX_wrapper *gpxmw;
    GPX_wrapper::TrackPointProperty curveMain;
    GPX_wrapper::TrackPointProperty curveSecondary;
};

/** @} Widgets */

#endif // _QDIAGRAMWIDGET_H_
