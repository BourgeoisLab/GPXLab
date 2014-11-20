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

#include <QDateTime>
#include "dialog_trackproperties.h"
#include "ui_dialog_trackproperties.h"
#include "qutils.h"

const QString Dialog_trackProperties::dlgName = "Track Properties";

Dialog_trackProperties::Dialog_trackProperties(GPX_trkMetadataType &metadata, const GPX_statsType &stats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_trackProperties),
    metadata(metadata)
{
    ui->setupUi(this);

    // Tab: Information
    ui->lineEditName->setText(QString::fromStdString(metadata.name));
    ui->lineEditCmt->setText(QString::fromStdString(metadata.cmt));
    ui->lineEditDesc->setText(QString::fromStdString(metadata.desc));
    ui->lineEditSrc->setText(QString::fromStdString(metadata.src));
    ui->lineEditType->setText(QString::fromStdString(metadata.type));
    ui->labelNumber->setText(QString::number(metadata.number));

    // Tab: Link
    ui->tableLinks->setColumnWidth(0, 234);
    ui->tableLinks->setColumnWidth(1, 140);
    ui->tableLinks->setColumnWidth(2, 80);
    for (vector<GPX_linkType>::iterator link = metadata.links.begin(); link != metadata.links.end(); ++link)
    {
        int row = ui->tableLinks->rowCount();
        ui->tableLinks->insertRow(row);
        ui->tableLinks->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(link->href)));
        ui->tableLinks->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(link->text)));
        ui->tableLinks->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(link->type)));
    }

    // Tab: Statistics
    QDateTime timestamp;
    ui->labelLatitudeMin->setText(QString::number(stats.bounds.minlat, 'f', 6));
    ui->labelLatitudeMax->setText(QString::number(stats.bounds.maxlat, 'f', 6));
    ui->labelLongitudeMin->setText(QString::number(stats.bounds.minlon, 'f', 6));
    ui->labelLongitudeMax->setText(QString::number(stats.bounds.maxlon, 'f', 6));
    ui->labelNumPoints->setText(QString::number(stats.points));
    timestamp.setTime_t(stats.startTime);
    ui->labelStartTime->setText(timestamp.toString("dd.MM.yyyy H:mm:ss"));
    timestamp.setTime_t(stats.endTime);
    ui->labelEndTime->setText(timestamp.toString("dd.MM.yyyy H:mm:ss"));
    ui->labelDuration->setText(QUtils::seconds_to_DHMS(stats.duration));
    ui->labelDistance->setText(QString::number(stats.distance, 'f', 1) + " km");
    ui->labelSpeed->setText(QString::number(stats.speed, 'f', 1) + " km/h");
    ui->labelHeightMin->setText(QString::number(stats.minhei, 'f', 0) + " m");
    ui->labelHeightMax->setText(QString::number(stats.maxhei, 'f', 0) + " m");
    ui->labelHeightIntUp->setText(QString::number(stats.heightIntUp, 'f', 0) + " m");
    ui->labelHeightIntDown->setText(QString::number(stats.heightIntDown, 'f', 0) + " m");

    // set modified flag after setText()
    setModified(false);
}

Dialog_trackProperties::~Dialog_trackProperties()
{
    delete ui;
}

void Dialog_trackProperties::on_Dialog_trackProperties_accepted()
{
    if (modified)
    {
        setModified(false);

        // Tab: Information
        metadata.name = ui->lineEditName->text().toStdString();
        metadata.cmt = ui->lineEditCmt->text().toStdString();
        metadata.desc = ui->lineEditDesc->text().toStdString();
        metadata.src = ui->lineEditSrc->text().toStdString();
        metadata.type = ui->lineEditType->text().toStdString();

        // Tab: Link
        metadata.links.clear();
        QTableWidgetItem *item;
        for (int i = 0; i < ui->tableLinks->rowCount(); ++i )
        {
            item = ui->tableLinks->item(i, 0);
            if (item && !item->text().isEmpty())
            {
                GPX_linkType link;
                link.href = item->text().toStdString();
                link.text = ui->tableLinks->item(i, 1)->text().toStdString();
                link.type = ui->tableLinks->item(i, 2)->text().toStdString();
                metadata.links.push_back(link);
            }
        }
    }
    else
    {
        setResult(QDialog::Rejected);
    }
}

void Dialog_trackProperties::setModified(bool modified)
{
    if (modified)
        setWindowTitle("*" + QString::fromStdString(metadata.name) + " - " + dlgName);
    else
        setWindowTitle(QString::fromStdString(metadata.name) + " - " + dlgName);
    this->modified = modified;
}

void Dialog_trackProperties::on_pushButtonTableAdd_clicked()
{
    int row = ui->tableLinks->rowCount();
    ui->tableLinks->insertRow(row);
    ui->tableLinks->setItem(row, 0, new QTableWidgetItem(""));
    ui->tableLinks->setItem(row, 1, new QTableWidgetItem(""));
    ui->tableLinks->setItem(row, 2, new QTableWidgetItem(""));
    setModified(true);
}

void Dialog_trackProperties::on_pushButtonTableDelete_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableLinks->selectedItems();
    if (items.count() > 0)
    {
        ui->tableLinks->removeRow(items[0]->row());
        setModified(true);
    }
}

void Dialog_trackProperties::on_pushButtonUndoName_clicked()
{
    ui->lineEditName->setText(QString::fromStdString(metadata.name));
}

void Dialog_trackProperties::on_pushButtonUndoCmt_clicked()
{
    ui->lineEditCmt->setText(QString::fromStdString(metadata.cmt));
}

void Dialog_trackProperties::on_pushButtonUndoDesc_clicked()
{
    ui->lineEditDesc->setText(QString::fromStdString(metadata.desc));
}

void Dialog_trackProperties::on_pushButtonUndoSrc_clicked()
{
    ui->lineEditSrc->setText(QString::fromStdString(metadata.src));
}

void Dialog_trackProperties::on_pushButtonUndoType_clicked()
{
    ui->lineEditType->setText(QString::fromStdString(metadata.type));
}

void Dialog_trackProperties::on_lineEditName_editingFinished()
{
    setModified(true);
}

void Dialog_trackProperties::on_lineEditCmt_editingFinished()
{
    setModified(true);
}

void Dialog_trackProperties::on_lineEditDesc_editingFinished()
{
    setModified(true);
}

void Dialog_trackProperties::on_lineEditSrc_editingFinished()
{
    setModified(true);
}

void Dialog_trackProperties::on_lineEditType_editingFinished()
{
    setModified(true);
}
