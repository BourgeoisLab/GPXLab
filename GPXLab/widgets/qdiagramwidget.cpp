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

#include "gpxlab.h"
#include "qdiagramwidget.h"

QDiagramWidget::QDiagramWidget(QWidget *parent) :
    QCustomPlotExt(parent)
{
    xAxis->setTickLabelType(QCPAxis::ltDateTime);
    xAxis->setDateTimeFormat("H:mm");
    xAxis->setDateTimeSpec(Qt::UTC);
    xAxis->setRange(0.0, 1.0);
    yAxis->setRange(0.0, 1.0);
    yAxis2->setRange(0.0, 1.0);
    yAxis2->setVisible(true);

    addGraph(xAxis, yAxis2);
    graph(0)->setPen(QPen(Qt::lightGray));

    addGraph(xAxis, yAxis);
    graph(1)->setPen(QPen(Qt::blue));
    graph(1)->setBrush(QBrush(QColor(0, 0, 255, 20)));

    QPen pen;
    pen.setColor(GPXLab::appColor);
    pen.setStyle(Qt::DashLine);
    addMarker(pen);
    connect(this, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(newSelection(QMouseEvent*)));
}

void QDiagramWidget::init(const GPX_wrapper *gpxmw)
{
    const GPX_wrapper::TrackPointProperty choices[] = {
        GPX_wrapper::none,
        GPX_wrapper::distance,
        GPX_wrapper::leglength,
        GPX_wrapper::speed,
        GPX_wrapper::altitude,
        GPX_wrapper::latitude,
        GPX_wrapper::longitude,
        GPX_wrapper::heading,
        GPX_wrapper::sat,
        GPX_wrapper::magvar,
        GPX_wrapper::hdop,
        GPX_wrapper::vdop,
        GPX_wrapper::pdop,
        GPX_wrapper::ageofdgpsdata,
        GPX_wrapper::dgpsid,
        GPX_wrapper::geoidheight,
        GPX_wrapper::heartrate
    };

    this->gpxmw = gpxmw;

    curveMain = GPX_wrapper::altitude;
    curveSecondary = GPX_wrapper::distance;

    QAction *actCurveMain = new QAction("Main curve", this);
    QAction *actCurveSecondary = new QAction("Secondary curve", this);
    QMenu *submenuCurveMain = new QMenu;
    QMenu *submenuCurveSecondary = new QMenu;
    actCurveMain->setMenu(submenuCurveMain);
    actCurveSecondary->setMenu(submenuCurveSecondary);
    addAction(actCurveMain);
    addAction(actCurveSecondary);
    for (size_t i = 0; i < sizeof(choices)/sizeof(GPX_wrapper::TrackPointProperty); ++i)
    {
        QAction *actCurveMain = new QAction(gpxmw->getTrackPointPropertyLabel(choices[i]), this);
        actCurveMain->setData(choices[i]);
        connect(actCurveMain, SIGNAL(triggered()), this, SLOT(on_actionCurveMain_triggered()));
        submenuCurveMain->addAction(actCurveMain);

        QAction *actCurveSecondary = new QAction(gpxmw->getTrackPointPropertyLabel(choices[i]), this);
        actCurveSecondary->setData(choices[i]);
        connect(actCurveSecondary, SIGNAL(triggered()), this, SLOT(on_actionCurveSecondary_triggered()));
        submenuCurveSecondary->addAction(actCurveSecondary);
    }

    build();
}

void QDiagramWidget::build()
{
    double xMin, xMax, CurveMainMin, CurveMainMax, CurveSecondaryMin, CurveSecondaryMax;

    // clear old values
    clear();

    // get start time of track
    const GPX_wptType *trkpt = gpxmw->getPoint(gpxmw->getSelectedTrackNumber(), 0, 0);
    if (trkpt)
        startTimestamp = gpxmw->getTrackPointPropertyAsDouble(trkpt, GPX_wrapper::timestamp);
    else
        startTimestamp = 0;

    // generate new values
    gpxmw->generateDiagramValues(gpxmw->getSelectedTrackNumber(), gpxmw->getSelectedTrackSegmentNumber(), curveMain, curveSecondary);

    // get range
    getMinMax(gpxmw->getTimeValues(), xMin, xMax);
    getMinMax(gpxmw->getCurveMainValues(), CurveMainMin, CurveMainMax);
    getMinMax(gpxmw->getCurveSecondaryValues(), CurveSecondaryMin, CurveSecondaryMax);

    // set new values
    xAxis->setRange(xMin, xMax);
    yAxis->setLabel(gpxmw->getTrackPointPropertyLabel(curveMain));
    yAxis->setRange(CurveMainMin, CurveMainMax);
    yAxis2->setLabel(gpxmw->getTrackPointPropertyLabel(curveSecondary));
    yAxis2->setRange(CurveSecondaryMin, CurveSecondaryMax);
    graph(1)->setData(gpxmw->getTimeValues(), gpxmw->getCurveMainValues());
    graph(0)->setData(gpxmw->getTimeValues(), gpxmw->getCurveSecondaryValues());

    yAxis->setVisible((curveMain == GPX_wrapper::none) ? false : true);
    yAxis2->setVisible((curveSecondary == GPX_wrapper::none) ? false : true);

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

void QDiagramWidget::select(time_t timestamp)
{
    setMarkerValue((double)(timestamp - startTimestamp));
}

void QDiagramWidget::newSelection(QMouseEvent* event)
{
    Q_UNUSED(event);
    emit selectionChanged((time_t)getMarkerValue() + startTimestamp);
}

void QDiagramWidget::on_actionCurveMain_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        curveMain = (GPX_wrapper::TrackPointProperty)action->data().toInt();
        build();
    }
}

void QDiagramWidget::on_actionCurveSecondary_triggered()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if (action)
    {
        curveSecondary = (GPX_wrapper::TrackPointProperty)action->data().toInt();
        build();
    }
}
GPX_wrapper::TrackPointProperty QDiagramWidget::getCurveMain() const
{
    return curveMain;
}

GPX_wrapper::TrackPointProperty QDiagramWidget::getCurveSecondary() const
{
    return curveSecondary;
}
