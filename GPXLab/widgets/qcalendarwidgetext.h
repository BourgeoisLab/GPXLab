#ifndef QCALENDARWIDGETEXT_H
#define QCALENDARWIDGETEXT_H

#include <QCalendarWidget>
#include <QTextCharFormat>
#include "gpx_wrapper.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QCalendarWidgetExt
 *
 * @brief QCalendarWidget subclass
 *
 * Extends the QCalendarWidget class.
 *
 * @see QCalendarWidget
 */
class QCalendarWidgetExt : public QCalendarWidget
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    QCalendarWidgetExt(QWidget *parent = 0);
    ~QCalendarWidgetExt();

    /**
     * @brief Initializes the widget
     * @note Call this function prior to any other function
     * @param gpxmw GPX_model wrapper
     */
    void init(const GPX_wrapper *gpxmw);

    /**
     * @brief Builds the widget
     */
    void build();

    /**
     * @brief Clears the widget
     */
    void clear();

    /**
     * @brief Selects a date corresponding to the track
     * @param trackNumber Track number
     */
    void select(int trackNumber);

signals:

    /**
     * @brief Signal when a new date was selected with a track on this day
     * @see selectionChangedExt()
     * @param trackNumber Track number
     */
    void selectionChanged(int trackNumber);

private slots:

    void selectionChangedExt();

private:

    const GPX_wrapper *gpxmw;
    QTextCharFormat format;
    bool selecting;
};

/** @} Widgets */

#endif // QCALENDARWIDGETEXT_H
