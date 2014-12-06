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
 
#include "qtreewidgettracks.h"

QTreeWidgetTracks::QTreeWidgetTracks(QWidget *parent) :
    QTreeWidget(parent)
{
}

void QTreeWidgetTracks::build(const GPX_wrapper *gpxmw)
{
    int trackNumber = 0;
    int trackSegmentNumber = 0;
    const GPX_trkType* trk;
    const GPX_trksegType* trkseg;
    QTreeWidgetItem *itemTrack = NULL, *itemTrackSegment;

    // clear tree if necessary
    clear();

    if (gpxmw->initTrkIteration() > 0)
    {
        while ((trk = gpxmw->getNextTrk()))
        {
            QList<QVariant> dataListTrack;
            trackSegmentNumber = 0;
            itemTrack = new QTreeWidgetItem((QTreeWidget*)0, QStringList(QString::fromStdString(trk->metadata.name)));
            dataListTrack.append(trackNumber);
            dataListTrack.append(-1);
            itemTrack->setData(0, Qt::UserRole, dataListTrack);
            if (gpxmw->initTrksegIteration(trackNumber) > 1)
            {
                while ((trkseg = gpxmw->getNextTrkseg(trackNumber)))
                {
                    QList<QVariant> dataListTrackSegment;
                    itemTrackSegment = new QTreeWidgetItem((QTreeWidget*)0, QStringList("#" + QString::number(trackSegmentNumber + 1)));
                    dataListTrackSegment.append(trackNumber);
                    dataListTrackSegment.append(trackSegmentNumber);
                    itemTrackSegment->setData(0, Qt::UserRole, dataListTrackSegment);
                    itemTrack->addChild(itemTrackSegment);
                    trackSegmentNumber++;
                }
            }
            addTopLevelItem(itemTrack);
            trackNumber++;
        }
    }
}

void QTreeWidgetTracks::select(int trackNumber, int trackSegmentNumber)
{
    if (trackNumber == -1)
    {
        //setCurrentIndex(model()->index(topLevelItemCount() - 1, 0));
    }
    else if (trackSegmentNumber == -1)
    {
        setCurrentIndex(model()->index(trackNumber, 0));
    }
    else
    {
        QList<QVariant> data;
        QTreeWidgetItemIterator it(this);
        while (*it)
        {
            data = (*it)->data(0, Qt::UserRole).toList();
            if (trackNumber == data.at(0).toInt() && trackSegmentNumber == data.at(1).toInt())
            {
                setCurrentItem(*it);
                break;
            }
            ++it;
        }
    }
}

void QTreeWidgetTracks::setTrackName(int trackNumber, const QString& name)
{
    QList<QVariant> data;
    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        data = (*it)->data(0, Qt::UserRole).toList();
        if (trackNumber == data.at(0).toInt() && -1 == data.at(1).toInt())
        {
            (*it)->setText(0, name);
            return;
        }
        ++it;
    }
}
