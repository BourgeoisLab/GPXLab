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

#include "dialog_datetime.h"
#include "ui_dialog_datetime.h"

Dialog_DateTime::Dialog_DateTime(const QString title, const time_t timestamp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_DateTime)
{
    ui->setupUi(this);
    setWindowTitle(title);
    QDateTime dt;
    dt.setTime_t(timestamp);
    ui->dateTimeEdit->setDateTime(dt);
}

Dialog_DateTime::~Dialog_DateTime()
{
    delete ui;
}

QDateTime Dialog_DateTime::getDateTime() const
{
    return ui->dateTimeEdit->dateTime();
}

time_t Dialog_DateTime::getTimestamp() const
{
    QDateTime dt = ui->dateTimeEdit->dateTime();
    return dt.toTime_t();
}
