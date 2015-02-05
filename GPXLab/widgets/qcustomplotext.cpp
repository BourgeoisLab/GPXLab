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
 
#include "qcustomplotext.h"

QCustomPlotExt::QCustomPlotExt(QWidget *parent) :
    QCustomPlot(parent),
    marker(NULL)
{
    clearExt();
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
    axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    connect(this, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel(QWheelEvent*)));
    connect(xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisRangeChanged(QCPRange)));
    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisRangeChanged(QCPRange)));
}

void QCustomPlotExt::addMarker()
{
    QPen pen;
    pen.setColor(Qt::darkRed);
    pen.setStyle(Qt::DashLine);
    addMarker(pen);
}

void QCustomPlotExt::addMarker(const QPen &pen)
{
    const QVector<double> markerValues = QVector<double>() << 0 << 0;
    marker = new QCPCurve(xAxis, yAxis);
    marker->setPen(pen);
    marker->setData(markerValues, markerValues);
    addPlottable(marker);
    connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress(QMouseEvent*)));
    markerValue = 0.0;
}

void QCustomPlotExt::updateMarker()
{
    if (marker)
    {
        QCPCurveDataMap *dataMap = marker->data();
        QMap<double, QCPCurveData>::iterator i = dataMap->begin();
        i.value().key = markerValue;
        i.value().value = yAxis->range().lower;
        ++i;
        i.value().key = markerValue;
        i.value().value = yAxis->range().upper;
    }
}

void QCustomPlotExt::setMarkerValue(double value)
{
    if (marker)
    {
        markerValue = value;
        updateMarker();
        replot();
    }
}

double QCustomPlotExt::getMarkerValue() const
{
    return markerValue;
}

void QCustomPlotExt::clearExt()
{
    xAxisRangeOrg.lower = QCPRange::minRange;
    xAxisRangeOrg.upper = QCPRange::maxRange;
    plotYOffset = 0.0;
    plotYGain = 1.0;
    markerValue = 0.0;
}

void QCustomPlotExt::updateExt()
{
    xAxisRangeOrg = xAxis->range();

    // set gain and offset to adapt yAxis2 range to yAxis range keeping ratio
    plotYOffset = yAxis->range().lower - yAxis2->range().lower;
    plotYGain = (yAxis2->range().upper - yAxis2->range().lower) / (yAxis->range().upper - yAxis->range().lower);
}

void QCustomPlotExt::mouseWheel(QWheelEvent *event)
{
    if (event->modifiers()& Qt::ShiftModifier)
    {
        axisRect()->setRangeZoom(Qt::Vertical);
    }
    else if (event->delta() < 0 && xAxisRangeOrg == xAxis->range())
    {
        axisRect()->setRangeZoom(0);
    }
    else
    {
        if (event->modifiers()& Qt::ControlModifier)
            axisRect()->setRangeZoom(Qt::Horizontal);
        else
            axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    }
}

void QCustomPlotExt::mousePress(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (marker)
        {
            if(axisRect()->rect().contains(event->pos()))
                setMarkerValue(xAxis->pixelToCoord(event->x()));
        }
    }
}

void QCustomPlotExt::yAxisRangeChanged(const QCPRange& range)
{
    yAxis2->setRange((range - plotYOffset) * plotYGain);
    updateMarker();
}

void QCustomPlotExt::xAxisRangeChanged(const QCPRange& range)
{
    double delta = range.upper - range.lower;
    if (range.lower < xAxisRangeOrg.lower)
    {
        if (xAxisRangeOrg.lower + delta < xAxisRangeOrg.upper)
            xAxis->setRange(QCPRange(xAxisRangeOrg.lower, xAxisRangeOrg.lower + delta));
        else
            xAxis->setRange(xAxisRangeOrg);
    }
    else if (range.upper > xAxisRangeOrg.upper)
    {
        if (xAxisRangeOrg.upper - delta > xAxisRangeOrg.lower)
            xAxis->setRange(QCPRange(xAxisRangeOrg.upper - delta, xAxisRangeOrg.upper));
        else
            xAxis->setRange(xAxisRangeOrg);
    }
    updateMarker();
}

void QCustomPlotExt::getMinMax(const QVector<double>& vec, double& min, double& max)
{
    min = std::numeric_limits<double>::max();
    max = std::numeric_limits<double>::min();
    foreach (double val, vec)
    {
        min = qMin(min, val);
        max = qMax(max, val);
    }
}
