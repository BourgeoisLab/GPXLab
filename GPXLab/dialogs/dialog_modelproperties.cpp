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

#include <QDateTime>
#include "dialog_modelproperties.h"
#include "ui_dialog_modelproperties.h"
#include "qutils.h"

Dialog_modelProperties::Dialog_modelProperties(GPX_metadataType &metadata, const GPX_statsType &stats, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_modelProperties),
    metadata(metadata)
{
    ui->setupUi(this);

    // Tab: Information
    ui->lineEditName->setText(QString::fromStdString(metadata.name));
    ui->lineEditDesc->setText(QString::fromStdString(metadata.desc));
    ui->textEditKeywords->setText(QString::fromStdString(metadata.keywords));

    // Tab: Author
    ui->lineEditAuthorName->setText(QString::fromStdString(metadata.author.name));
    if(!metadata.author.email.id.empty())
        ui->lineEditAuthorEmail->setText(QString::fromStdString(metadata.author.email.id) + "@" + QString::fromStdString(metadata.author.email.domain));
    ui->lineEditAuthorLinkURL->setText(QString::fromStdString(metadata.author.link.href));
    ui->lineEditAuthorLinkText->setText(QString::fromStdString(metadata.author.link.text));
    ui->lineEditAuthorLinkType->setText(QString::fromStdString(metadata.author.link.type));

    // Tab: Copyright
    ui->lineEditCopyrightAuthor->setText(QString::fromStdString(metadata.copyright.author));
    ui->lineEditCopyrightYear->setText(QString::fromStdString(metadata.copyright.year));
    ui->lineEditCopyrightLicense->setText(QString::fromStdString(metadata.copyright.license));

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
    timestamp.setTime_t(metadata.timestamp);
    QString sTime = timestamp.toString("dd.MM.yyyy H:mm:ss");
    if (metadata.millisecond)
        sTime += "." + QString("%1").arg(metadata.millisecond, 3, 10, QChar('0'));
    ui->labelCreationDate->setText(sTime);
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
    ui->labelDistance->setText(QString::number(stats.distance, 'f', 1) + " " + tr("km"));
    ui->labelSpeed->setText(QString::number(stats.speed, 'f', 1) + " " + tr("km/h"));
    ui->labelHeightMin->setText(QString::number(stats.minhei, 'f', 0) + " " + tr("m"));
    ui->labelHeightMax->setText(QString::number(stats.maxhei, 'f', 0) + " " + tr("m"));
    ui->labelHeightIntUp->setText(QString::number(stats.heightIntUp, 'f', 0) + " " + tr("m"));
    ui->labelHeightIntDown->setText(QString::number(stats.heightIntDown, 'f', 0) + " " + tr("m"));

    // set modified flag after setText()
    setModified(false);
}

Dialog_modelProperties::~Dialog_modelProperties()
{
    delete ui;
}

void Dialog_modelProperties::on_Dialog_modelProperties_accepted()
{
    if (modified)
    {
        setModified(false);

        // Tab: Information
        metadata.name = ui->lineEditName->text().toStdString();
        metadata.desc = ui->lineEditDesc->text().toStdString();
        metadata.keywords = ui->textEditKeywords->toPlainText().toStdString();

        // Tab: Author
        metadata.author.name = ui->lineEditAuthorName->text().toStdString();
        string email = ui->lineEditAuthorEmail->text().toStdString();
        size_t atPos = email.find_last_of("@");
        if (atPos > 0)
        {
            metadata.author.email.id = email.substr(0, atPos);
            metadata.author.email.domain = email.substr(atPos + 1);
        }
        metadata.author.link.href = ui->lineEditAuthorLinkURL->text().toStdString();
        metadata.author.link.text = ui->lineEditAuthorLinkText->text().toStdString();
        metadata.author.link.type = ui->lineEditAuthorLinkType->text().toStdString();

        // Tab: Copyright
        metadata.copyright.author = ui->lineEditCopyrightAuthor->text().toStdString();
        metadata.copyright.year = ui->lineEditCopyrightYear->text().toStdString();
        metadata.copyright.license = ui->lineEditCopyrightLicense->text().toStdString();

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

void Dialog_modelProperties::setModified(bool modified)
{
    if (modified)
        setWindowTitle("*" + QString::fromStdString(metadata.name) + " - " + tr("File Properties"));
    else
        setWindowTitle(QString::fromStdString(metadata.name) + " - " + tr("File Properties"));
    this->modified = modified;
}

void Dialog_modelProperties::on_pushButtonTableAdd_clicked()
{
    int row = ui->tableLinks->rowCount();
    ui->tableLinks->insertRow(row);
    ui->tableLinks->setItem(row, 0, new QTableWidgetItem(""));
    ui->tableLinks->setItem(row, 1, new QTableWidgetItem(""));
    ui->tableLinks->setItem(row, 2, new QTableWidgetItem(""));
    setModified(true);
}

void Dialog_modelProperties::on_pushButtonTableDelete_clicked()
{
    QList<QTableWidgetItem*> items = ui->tableLinks->selectedItems();
    if (items.count() > 0)
    {
        ui->tableLinks->removeRow(items[0]->row());
        setModified(true);
    }
}

void Dialog_modelProperties::on_pushButtonUndoName_clicked()
{
    ui->lineEditName->setText(QString::fromStdString(metadata.name));
}

void Dialog_modelProperties::on_pushButtonUndoDesc_clicked()
{
    ui->lineEditDesc->setText(QString::fromStdString(metadata.desc));
}

void Dialog_modelProperties::on_pushButtonUndoKeywords_clicked()
{
    ui->textEditKeywords->setText(QString::fromStdString(metadata.keywords));
}

void Dialog_modelProperties::on_pushButtonUndoAuthorName_clicked()
{
    ui->lineEditAuthorName->setText(QString::fromStdString(metadata.author.name));
}

void Dialog_modelProperties::on_pushButtonUndoAuthorEmail_clicked()
{
    if(!metadata.author.email.id.empty())
        ui->lineEditAuthorEmail->setText(QString::fromStdString(metadata.author.email.id) + "@" + QString::fromStdString(metadata.author.email.domain));
    else
        ui->lineEditAuthorEmail->setText("");
}

void Dialog_modelProperties::on_pushButtonUndoAuthorLinkURL_clicked()
{
    ui->lineEditAuthorLinkURL->setText(QString::fromStdString(metadata.author.link.href));
}

void Dialog_modelProperties::on_pushButtonUndoAuthorLinkText_clicked()
{
    ui->lineEditAuthorLinkText->setText(QString::fromStdString(metadata.author.link.text));
}

void Dialog_modelProperties::on_pushButtonUndoAuthorLinkType_clicked()
{
    ui->lineEditAuthorLinkType->setText(QString::fromStdString(metadata.author.link.type));
}

void Dialog_modelProperties::on_pushButtonUndoCopyrightAuthor_clicked()
{
    ui->lineEditCopyrightAuthor->setText(QString::fromStdString(metadata.copyright.author));
}

void Dialog_modelProperties::on_pushButtonUndoCopyrightYear_clicked()
{
    ui->lineEditCopyrightYear->setText(QString::fromStdString(metadata.copyright.year));
}

void Dialog_modelProperties::on_pushButtonUndoCopyrightLicense_clicked()
{
    ui->lineEditCopyrightLicense->setText(QString::fromStdString(metadata.copyright.license));
}

void Dialog_modelProperties::on_lineEditName_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditDesc_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_textEditKeywords_textChanged()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditAuthorName_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditAuthorEmail_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditAuthorLinkURL_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditAuthorLinkText_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditAuthorLinkType_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditCopyrightAuthor_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditCopyrightYear_editingFinished()
{
    setModified(true);
}

void Dialog_modelProperties::on_lineEditCopyrightLicense_editingFinished()
{
    setModified(true);
}
