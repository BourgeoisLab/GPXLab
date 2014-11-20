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

#include "dialog_about.h"
#include "ui_dialog_about.h"
#include "gpxlab.h"

Dialog_about::Dialog_about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_about)
{
    ui->setupUi(this);

    ui->labelMain->setText(GPXLab::appName + " " + GPXLab::appVersion);
    ui->labelCopyright->setText(" ©2014 " + GPXLab::organisationName);
    ui->labelLinkHomepage->setOpenExternalLinks(true);
    ui->labelLinkSourceCode->setOpenExternalLinks(true);
}

Dialog_about::~Dialog_about()
{
    delete ui;
}
