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
