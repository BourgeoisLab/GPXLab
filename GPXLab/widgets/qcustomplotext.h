#ifndef QCUSTOMPLOTEXT_H
#define QCUSTOMPLOTEXT_H

#include "qcustomplot.h"

/**
 * @addtogroup Widgets Widgets
 * @brief Widgets related functions
 * @{
 */

/**
 * @class QCustomPlotExt
 *
 * @brief QCustomPlot subclass
 *
 * Extends the QCustomPlot class by adding a marker and defines the
 * behaviour of panning and zooming.
 *
 * The marker shows a location on the plot. The marker can be moved by
 * clicking inside the plot or using the function setMarkerValue().
 *
 * Zooming behaviour:
 * - Mouse wheel: zoom both x and y axis
 * - SHIFT + mouse wheel: zoom y axis only
 * - CTRL + mouse wheel: zoom x axis only
 *
 * Both yAxis and yAxis2 are zoomed and panned. The aspect ratio between
 * both are kept.
 *
 * @see QCustomPlot
 */
class QCustomPlotExt : public QCustomPlot
{
    Q_OBJECT

public:

    /**
     * @brief Gets the minimal and maximal value from a vector
     * @param vec Vector
     * @param min Minimal value
     * @param max Maximal value
     */
    static void getMinMax(const QVector<double>& vec, double &min, double &max);

    /**
     * @brief Constructor
     * @param parent Parent
     */
    explicit QCustomPlotExt(QWidget *parent = nullptr);

    /**
     * @brief Adds a marker with the default QPen
     */
    void addMarker();

    /**
     * @brief Adds a marker
     * @param pen QPen to draw the marker
     */
    void addMarker(const QPen &pen);

    /**
     * @brief Gets the marker value
     * @return Marker value on the x axis
     */
    double getMarkerValue() const;

    /**
     * @brief Sets the marker value
     * @param value Value on the x axis
     */
    void setMarkerValue(double value);

    /**
     * @brief Clear extensions
     * @note Call this function before calling any setRange()
     */
    void clearExt();

    /**
     * @brief Updates extensions
     * @note Call this function after calling any setData()
     */
    void updateExt();

private slots:

    void yAxisRangeChanged(const QCPRange& range);
    void xAxisRangeChanged(const QCPRange& range);
    void mouseWheel(QWheelEvent *event);
    void mousePress(QMouseEvent* event);

private:

    QCPCurve *marker;
    QCPRange xAxisRangeOrg;
    double plotYOffset;
    double plotYGain;
    double markerValue;
    void updateMarker();
};

/** @} Widgets */

#endif // QCUSTOMPLOTEXT_H
