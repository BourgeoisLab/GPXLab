#ifndef QUTILS_H
#define QUTILS_H

#include <QString>
#include <QVector>

/**
 * @ingroup GPXLab
 * @{
 */

/**
 * @namespace QUtils
 *
 * @brief Utility functions.
 *
 * Utility functions.
 * These functions may use Qt stuff.
 */
namespace QUtils
{
    /**
     * @brief Converts an amount of seconds in a human readable string.
     * @param seconds Amount of seconds
     * @param milliseconds Amount of milliseconds, default 0
     * @return String in format "m:s[.mmm]", "h:m:s[.mmm]" or "ddh:m:s[.mmm]"
     */
    QString seconds_to_DHMS(unsigned int seconds, unsigned int milliseconds = 0);


    /**
     * @brief Averages the values with a moving average algorithm
     * @param values Values to average
     * @param window_size Window size of moving average
     * @return Averaged values
     */
    QVector<double> movingAverage(const QVector<double> &values, int window_size);
}

/** @} GPXLab */

#endif // QUTILS_H
