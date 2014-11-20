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

#include "dialog_srtm.h"
#include "ui_dialog_srtm.h"
#include "qutils.h"

const QString Dialog_srtm::dlgName = "Get Altitude From SRTM";

Dialog_srtm::Dialog_srtm(const GPX_wrapper *gpxmw, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_srtm),
    gpxmw(gpxmw)
{
    // create new SRTM class
     srtm = new SRTM();

    // setup UI
    ui->setupUi(this);
    ui->labelDownload->setTextFormat(Qt::RichText);
    ui->labelDownload->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelDownload->setOpenExternalLinks(true);
    ui->labelKernelSize->setText(QString::number(ui->horizontalSliderKernelSize->value()));
    ui->labelNumPasses->setText(QString::number(ui->horizontalSliderNumPasses->value()));

    // add original curve
    ui->widgetPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    ui->widgetPlot->xAxis->setDateTimeFormat("d.M.yyyy\nH:mm:ss");
    ui->widgetPlot->yAxis->setLabel("Altitude [m]");
    ui->widgetPlot->addGraph();
    ui->widgetPlot->addGraph();
    ui->widgetPlot->graph(0)->setPen(QPen(Qt::blue));
    ui->widgetPlot->graph(0)->setBrush(QBrush(QColor(0, 0, 255, 20)));
    ui->widgetPlot->graph(0)->setData(gpxmw->getDiagramTimeValues(), gpxmw->getDiagramAltitudeValues());
    ui->widgetPlot->graph(0)->rescaleAxes();
    ui->widgetPlot->updateExt();
    ui->widgetPlot->replot();

    setModified(false);
}

Dialog_srtm::~Dialog_srtm()
{
    delete ui;
    delete srtm;
}

void Dialog_srtm::on_Dialog_srtm_accepted()
{
    if (modified)
    {
        setModified(false);
    }
    else
    {
        setResult(QDialog::Rejected);
    }
}

const QVector<double> &Dialog_srtm::getValues() const
{
    return values;
}

void Dialog_srtm::setModified(bool modified)
{
    if (modified)
        setWindowTitle("*" + dlgName);
    else
        setWindowTitle(dlgName);
    this->modified = modified;
}

bool Dialog_srtm::generateSRTMAltitudeValues(int option)
{
    values.clear();
    if (gpxmw->initTrksegIteration(gpxmw->getSelectedTrackNumber()) > 0)
    {
        const GPX_trksegType* trkseg;
        const GPX_wptType* wpt;
        double altitude = 0.0;
        short altitudeSRTM = 0;
        int TrackSegmentNumber = 0;
        while ((trkseg = gpxmw->getNextTrkseg(gpxmw->getSelectedTrackNumber())))
        {
            if (gpxmw->initWptIteration(gpxmw->getSelectedTrackNumber(), TrackSegmentNumber) > 0)
            {
                while ((wpt = gpxmw->getNextWpt(gpxmw->getSelectedTrackNumber(), TrackSegmentNumber)))
                {
                    switch (option)
                    {
                    case 0:
                        if (srtm->getAltitude(wpt->latitude, wpt->longitude, altitudeSRTM) == false)
                            return false;
                        altitude = altitudeSRTM;
                        break;

                    case 1:
                        if (wpt->altitude == 0.0)
                        {
                            if (srtm->getAltitude(wpt->latitude, wpt->longitude, altitudeSRTM) == false)
                                return false;
                            altitude = altitudeSRTM;
                        }
                        else
                        {
                            altitude = wpt->altitude;
                        }
                        break;

                    case 2:
                        if (srtm->getAltitude(wpt->latitude, wpt->longitude, altitudeSRTM) == false)
                            return false;
                        altitude = wpt->altitude / altitudeSRTM;
                        if (altitude < 0.9 || altitude > 1.1)
                            altitude = altitudeSRTM;
                        else
                            altitude = wpt->altitude;
                        break;
                    }
                    values.append((double)altitude);
                }
            }
            TrackSegmentNumber++;
        }
        setModified(true);
        return true;
    }
    return false;
}

void Dialog_srtm::on_pushButtonFetchData_clicked()
{
    int option = 0;
    if (ui->radioButton_1->isChecked())
        option = 0;
    else if (ui->radioButton_2->isChecked())
        option = 1;
    else if (ui->radioButton_3->isChecked())
        option = 2;

    // generate altitude values from SRTM database
    if (generateSRTMAltitudeValues(option))
    {
        ui->labelNotFound->setText("");
        ui->labelDownload->setText("");

        // average data
        for (int n = 0; n < ui->horizontalSliderNumPasses->value(); ++n)
            QUtils::movingAverage(values, ui->horizontalSliderKernelSize->value());

        // SRTM curve
        ui->widgetPlot->graph(1)->clearData();
        ui->widgetPlot->graph(1)->setPen(QPen(Qt::red));
        ui->widgetPlot->graph(1)->setData(gpxmw->getDiagramTimeValues(), values);
        ui->widgetPlot->graph(1)->rescaleAxes();
        ui->widgetPlot->replot();
    }
    else
    {
        QString url = QString::fromStdString(srtm->getFileURL());
        ui->labelNotFound->setText("Height file not found: " + QString::fromStdString(srtm->getFileName()));
        ui->labelDownload->setText("Donwload file here: <a href=\"" + url + "\">" + url + "</a>");
    }
}

void Dialog_srtm::on_horizontalSliderKernelSize_valueChanged(int value)
{
    ui->labelKernelSize->setText(QString::number(value));
}

void Dialog_srtm::on_horizontalSliderNumPasses_valueChanged(int value)
{
    ui->labelNumPasses->setText(QString::number(value));
}
