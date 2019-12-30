#include "qutils.h"

QString QUtils::seconds_to_DHMS(unsigned int seconds, unsigned int milliseconds)
{
    QString str;
    int sec = seconds % 60;
    seconds /= 60;
    int min = seconds % 60;
    seconds /= 60;
    int hours = seconds % 24;
    int days = seconds / 24;
    if(hours == 0 && days == 0)
        str.sprintf("%02d:%02d", min, sec);
    else if (days == 0)
        str.sprintf("%02d:%02d:%02d", hours, min, sec);
    else
        str.sprintf("%dd%02d:%02d:%02d", days, hours, min, sec);
    if (milliseconds > 0)
        str += "." + QString("%1").arg(milliseconds, 3, 10, QChar('0'));
    return str;
}

QVector<double> QUtils::movingAverage(const QVector<double> &values, int window_size)
{
    if (window_size <= 1)
        return values;

    QVector<double> result(values.size());
    int idx = 0;
    double* ptr = &result[idx];

    int n_left, n_right;
    if (window_size % 2 == 0) // even window size
    {
        n_left = window_size / 2;
        n_right = window_size / 2 - 1;
    }
    else // odd window size
    {
        n_left = (window_size - 1) / 2;
        n_right = (window_size - 1) / 2;
    }

    // begin part
    int window_size_trunc = n_right + 1;
    for (; idx < n_left; idx++)
    {
        double sum = 0.0;
        for (int i = 0; i < window_size_trunc; i++)
            sum += values[i];
        *ptr++ = sum / window_size_trunc;
        window_size_trunc++;
    }

    // middle part
    for (; idx < values.size() - n_right; idx++)
    {
        double sum = 0.0;
        for (int i = idx - n_left; i <= idx + n_right; i++)
            sum += values[i];
        *ptr++ = sum / window_size;
    }

    // end part
    window_size_trunc = window_size - 1;
    for (; idx < values.size(); idx++)
    {
        double sum = 0.0;
        for (int i = idx - n_left; i < values.size(); i++)
            sum += values[i];
        *ptr++ = sum / window_size_trunc;
        window_size_trunc--;
    }
    return result;
}
