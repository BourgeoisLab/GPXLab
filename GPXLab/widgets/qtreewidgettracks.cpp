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
#include "qtreewidgettracks.h"

////////////////////////////////////////////////////////////////////////////////

#define TREE_COL_NUMBER             0
#define TREE_COL_NAME               1
#define TREE_COL_DATE               2
#define TREE_COL_DISTANCE           3

////////////////////////////////////////////////////////////////////////////////

/**
 * @brief QTreeWidgetItemExt
 *
 * @brief QTreeWidgetItem subclass
 *
 * Extends QTreeWidgetItem to sort values on an own way.
 *
 */
class QTreeWidgetItemExt : public QTreeWidgetItem
{
public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    QTreeWidgetItemExt(QTreeWidget *parent) : QTreeWidgetItem(parent)
    {
    }

    /**
     * @brief Overloads < operator
     * @param other Other item to compare with
     * @return True if item is smaller than other item
     */
    bool operator< (const QTreeWidgetItem &other) const
    {
        if (!parent())
        {
            // top level nodes
            int column = treeWidget()->sortColumn();
            switch (column)
            {
            case TREE_COL_NUMBER:
                return text(column).toInt() > other.text(column).toInt();
            case TREE_COL_NAME:
                return text(column) < other.text(column);
            case TREE_COL_DATE:
            {
                QDateTime timestamp, timestampOther;
                timestamp = QDateTime::fromString(text(column), "dd.MM.yy");
                timestampOther = QDateTime::fromString(other.text(column), "dd.MM.yy");
                return timestamp < timestampOther;
            }
            case TREE_COL_DISTANCE:
                return text(column).toDouble() < other.text(column).toDouble();
            default:
                return text(column) < other.text(column);
            }
        }
        else
        {
            // child nodes, don't sort
            return false;
        }
    }
};

////////////////////////////////////////////////////////////////////////////////

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
    QTreeWidgetItem *itemTrack = NULL, *itemTrackSegment = NULL;
    QDateTime timestamp;

    // set column widths
    setColumnWidth(TREE_COL_NUMBER, 40);
    setColumnWidth(TREE_COL_NAME, 90);
    setColumnWidth(TREE_COL_DATE, 50);
    setColumnWidth(TREE_COL_DISTANCE, 50);

    // clear tree if necessary
    clear();

    if (gpxmw->initTrkIteration() > 0)
    {
        while ((trk = gpxmw->getNextTrk()))
        {
            QList<QVariant> dataListTrack;
            trackSegmentNumber = 0;
            itemTrack = new QTreeWidgetItemExt(this);
            itemTrack->setText(TREE_COL_NUMBER, QString::number(trackNumber + 1));
            itemTrack->setText(TREE_COL_NAME, QString::fromStdString(trk->metadata.name));
            timestamp.setTime_t(trk->stats.startTime);
            itemTrack->setText(TREE_COL_DATE, timestamp.toString("dd.MM.yy"));
            itemTrack->setText(TREE_COL_DISTANCE, QString::number(trk->stats.distance, 'f', 1));
            itemTrack->setTextAlignment(TREE_COL_DATE, Qt::AlignHCenter);
            itemTrack->setTextAlignment(TREE_COL_DISTANCE, Qt::AlignHCenter);
            dataListTrack.append(trackNumber);
            dataListTrack.append(-1);
            itemTrack->setData(0, Qt::UserRole, dataListTrack);
            if (gpxmw->initTrksegIteration(trackNumber) > 1)
            {
                while ((trkseg = gpxmw->getNextTrkseg()))
                {
                    QList<QVariant> dataListTrackSegment;
                    itemTrackSegment = new QTreeWidgetItemExt(NULL);
                    itemTrackSegment->setText(TREE_COL_NUMBER, QString::number(trackNumber + 1) + "." + QString::number(trackSegmentNumber + 1));
                    itemTrackSegment->setText(TREE_COL_NAME, "#" + QString::number(trackSegmentNumber + 1));
                    itemTrackSegment->setText(TREE_COL_DATE, "");
                    itemTrackSegment->setText(TREE_COL_DISTANCE, QString::number(trkseg->stats.distance, 'f', 1));
                    itemTrackSegment->setTextAlignment(TREE_COL_DISTANCE, Qt::AlignHCenter);
                    dataListTrackSegment.append(trackNumber);
                    dataListTrackSegment.append(trackSegmentNumber);
                    itemTrackSegment->setData(0, Qt::UserRole, dataListTrackSegment);
                    itemTrack->addChild(itemTrackSegment);
                    trackSegmentNumber++;
                }
            }
            trackNumber++;
        }
    }
}

void QTreeWidgetTracks::select(int trackNumber, int trackSegmentNumber)
{
    QList<QVariant> data;
    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        data = (*it)->data(0, Qt::UserRole).toList();
        if (trackNumber == data.at(0).toInt() && trackSegmentNumber == data.at(1).toInt())
        {
            setCurrentItem(*it);
            return;
        }
        ++it;
    }
}

void QTreeWidgetTracks::setItemName(const QString& name, int trackNumber, int trackSegmentNumber)
{
    QList<QVariant> data;
    QTreeWidgetItemIterator it(this);
    while (*it)
    {
        data = (*it)->data(0, Qt::UserRole).toList();
        if (data.at(0).toInt() == trackNumber && data.at(1).toInt() == trackSegmentNumber)
        {
            (*it)->setText(TREE_COL_NAME, name);
            return;
        }
        ++it;
    }
}
