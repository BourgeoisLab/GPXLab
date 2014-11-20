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
 
#include "qdiagramwidget.h"

QDiagramWidget::QDiagramWidget(QWidget *parent) :
    QCustomPlotExt(parent)
{
    xAxis->setTickLabelType(QCPAxis::ltDateTime);
    xAxis->setDateTimeFormat("d.M.yyyy\nH:mm:ss");
    xAxis->setRange(0.0, 1.0);

    yAxis->setLabel(tr("Altitude [m]"));
    yAxis->setRange(0.0, 1.0);

    yAxis2->setLabel(tr("Speed [km/h]"));
    yAxis2->setRange(0.0, 1.0);
    yAxis2->setVisible(true);

    addGraph(xAxis, yAxis2);
    graph(0)->setPen(QPen(Qt::lightGray));
    graph(0)->setName(tr("Speed"));

    addGraph(xAxis, yAxis);
    graph(1)->setPen(QPen(Qt::blue));
    graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    graph(1)->setName(tr("Altitude"));

    addMarker();
}

void QDiagramWidget::setData(const QVector<double>& time, const QVector<double>& altitude, const QVector<double>& speed)
{
    double timeMin, timeMax, altitudeMin, altitudeMax, speedMin, speedMax;

    // clear old values
    clear();

    // get range
    getMinMax(time, timeMin, timeMax);
    getMinMax(altitude, altitudeMin, altitudeMax);
    getMinMax(speed, speedMin, speedMax);

    // set new values
    xAxis->setRange(timeMin, timeMax);
    yAxis->setRange(altitudeMin, altitudeMax);
    yAxis2->setRange(speedMin, speedMax);
    graph(1)->setData(time, altitude);
    graph(0)->setData(time, speed);

    // update extensions
    updateExt();

    // replot
    replot();
}

void QDiagramWidget::clear()
{
    // clear old values
    graph(0)->clearData();
    graph(1)->clearData();

    // clear extensions
    clearExt();

    // replot
    replot();
}
