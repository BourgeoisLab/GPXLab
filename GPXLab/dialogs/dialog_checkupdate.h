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
 
#ifndef _DIALOG_CHECKUPDATE_H_
#define _DIALOG_CHECKUPDATE_H_

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

namespace Ui {
class Dialog_CheckUpdate;
}

class Dialog_CheckUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_CheckUpdate(const QString &url, const QDate& since, QWidget *parent = nullptr);
    ~Dialog_CheckUpdate();
    void checkForUpdate();
    bool hasUpdate() const;
    bool ignoreUpdate() const;
    bool doCheckUpdate() const;

signals:
    void finished();

private slots:
    void httpFinished();

private:
    bool parseReplyGithub(const QByteArray &str);

private:
    Ui::Dialog_CheckUpdate *ui;
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool mHasUpdate;
    QDate mDateSince;
};

#endif // _DIALOG_CHECKUPDATE_H_
