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

#ifndef _QDIAGRAMWIDGET_H_
#define _QDIAGRAMWIDGET_H_

#include "qcustomplotext.h"

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
 * @version 1.0
 * @date 7 Nov 2014
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
     * @brief Sets new diagram values
     * @param time Time values
     * @param altitude Altitude values
     * @param speed Speed values
     */
    void setData(const QVector<double>& time, const QVector<double>& altitude, const QVector<double>& speed);

    /**
     * @brief Clears the diagram
     */
    void clear();
};

#endif // _QDIAGRAMWIDGET_H_
