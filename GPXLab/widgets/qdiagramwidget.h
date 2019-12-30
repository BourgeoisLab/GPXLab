#ifndef QDIAGRAMWIDGET_H
#define QDIAGRAMWIDGET_H

#include "qcustomplotext.h"
#include "gpx_wrapper.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QDiagramWidget
 *
 * @brief QCustomPlotExt subclass
 *
 * Extends the QCustomPlotExt class by adding a setData() function.
 *
 * @see QCustomPlotExt
 */
class QDiagramWidget : public QCustomPlotExt
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent
     */
    explicit QDiagramWidget(QWidget *parent = nullptr);

    /**
     * @brief init
     * @param gpxmw GPX_model wrapper
     */
    void init(const GPX_wrapper *gpxmw);

    /**
     * @brief Sets new diagram values
     */
    void build();

    /**
     * @brief Clears the diagram
     */
    void clear();

    /**
     * @brief Selects a new value
     * @param timestamp New timestamp
     */
    void select(time_t timestamp);

    /**
     * @brief Gets the property of the main curve
     * @return Property
     */
    GPX_wrapper::TrackPointProperty getCurveMain() const;

    /**
     * @brief Gets the property of the secondary curve
     * @return Property
     */
    GPX_wrapper::TrackPointProperty getCurveSecondary() const;

signals:

    /**
     * @brief Signal when the selection changed
     * @param timestamp New timestamp
     */
    void selectionChanged(time_t timestamp);

private slots:

    void newSelection(QMouseEvent* event);
    void on_actionCurveMain_triggered();
    void on_actionCurveSecondary_triggered();

private:

    const GPX_wrapper *gpxmw;
    time_t startTimestamp;
    GPX_wrapper::TrackPointProperty curveMain;
    GPX_wrapper::TrackPointProperty curveSecondary;
};

/** @} Widgets */

#endif // QDIAGRAMWIDGET_H
