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

#include "qcalendarwidgetext.h"

QCalendarWidgetExt::QCalendarWidgetExt(QWidget *parent) :
    QCalendarWidget(parent),
    selecting(false)
{
    connect(this, SIGNAL(selectionChanged()), this, SLOT(selectionChangedExt()));
    format.setBackground(QBrush(QColor(200,200,200)));
}

QCalendarWidgetExt::~QCalendarWidgetExt()
{

}

void QCalendarWidgetExt::init(const GPX_wrapper *gpxmw)
{
    this->gpxmw = gpxmw;
}

void QCalendarWidgetExt::build()
{
    clear();

    if (gpxmw->initTrkIteration() > 0)
    {
        const GPX_trkType* trk;
        while ((trk = gpxmw->getNextTrk()))
        {
            QDateTime dt = QDateTime::fromTime_t(trk->stats.startTime);
            setDateTextFormat(dt.date(), format);
        }
    }
}

void QCalendarWidgetExt::clear()
{
    setDateTextFormat(QDate(), QTextCharFormat());
}

void QCalendarWidgetExt::selectionChangedExt()
{
    if (selecting)
        return;
    if (gpxmw->initTrkIteration() > 0)
    {
        int trackNumber = 0;
        const GPX_trkType* trk;
        while ((trk = gpxmw->getNextTrk()))
        {
            QDateTime dt = QDateTime::fromTime_t(trk->stats.startTime);
            if (selectedDate() == dt.date())
            {
                emit(selectionChanged(trackNumber));
                return;
            }
            ++trackNumber;
        }
    }
}

void QCalendarWidgetExt::select(int trackNumber)
{
    const GPX_trkType* trk = gpxmw->getTrack(trackNumber);
    if (trk)
    {
        QDateTime dt = QDateTime::fromTime_t(trk->stats.startTime);
        selecting = true;
        setSelectedDate(dt.date());
        selecting = false;
    }
}
