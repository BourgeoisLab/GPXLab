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

#include <QFileDialog>
#include "dialog_settings.h"
#include "ui_dialog_settings.h"

Dialog_settings::Dialog_settings(Settings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings),
    settings(settings)
{
    ui->setupUi(this);

    ui->checkBoxMapPersistentCache->setChecked(settings->doPersistentCaching);
    ui->lineEditMapCachePath->setText(settings->cachePath);
    ui->pushButtonMapClearCache->setEnabled(settings->doPersistentCaching);
    ui->lineEditMapCachePath->setEnabled(settings->doPersistentCaching);
    ui->pushButtonMapCacheLocationSelect->setEnabled(settings->doPersistentCaching);
    ui->pushButtonMapCacheLocationDefault->setEnabled(settings->doPersistentCaching);
}

Dialog_settings::~Dialog_settings()
{
    delete ui;
}

void Dialog_settings::on_checkBoxMapPersistentCache_toggled(bool checked)
{
    settings->doPersistentCaching = checked;
    ui->pushButtonMapClearCache->setEnabled(settings->doPersistentCaching);
    ui->lineEditMapCachePath->setEnabled(settings->doPersistentCaching);
    ui->pushButtonMapCacheLocationSelect->setEnabled(settings->doPersistentCaching);
    ui->pushButtonMapCacheLocationDefault->setEnabled(settings->doPersistentCaching);
}

void Dialog_settings::on_pushButtonMapClearCache_clicked()
{
    settings->clearCache();
}

void Dialog_settings::on_lineEditMapCachePath_editingFinished()
{
    settings->cachePath = ui->lineEditMapCachePath->text();
}

void Dialog_settings::on_pushButtonMapCacheLocationSelect_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Cache Location"), settings->cachePath);
    if (!path.isEmpty())
    {
        ui->lineEditMapCachePath->setText(path);
        on_lineEditMapCachePath_editingFinished();
    }
}

void Dialog_settings::on_pushButtonMapCacheLocationDefault_clicked()
{
    ui->lineEditMapCachePath->setText(settings->defaultCachePath());
    on_lineEditMapCachePath_editingFinished();
}
