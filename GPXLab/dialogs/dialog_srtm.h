#ifndef DIALOG_SRTM_H
#define DIALOG_SRTM_H

#include <QDialog>
#include "srtm.h"
#include "gpx_wrapper.h"

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_srtm;
}

/**
 * @class Dialog_srtm
 *
 * @brief Dialog subclass to get altitude values from SRTM
 *
 * The dialogue can be used to get altitude values from SRTM.
 *
 * @see GPX_model and SRTM
 */
class Dialog_srtm : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param gpxmw GPX_model wrapper
     * @param parent Parent
     */
    explicit Dialog_srtm(const GPX_wrapper *gpxmw, QWidget *parent = nullptr);
    ~Dialog_srtm();

    /**
     * @brief Get altitude values
     * @return values
     */
    const QVector<double> &getValues() const;

private slots:

    void on_Dialog_srtm_accepted();
    void on_pushButtonFetchData_clicked();
    void on_horizontalSliderKernelSize_valueChanged(int value);

private:

    void setModified(bool modified);
    bool generateSRTMAltitudeValues(int option);

private:

    Ui::Dialog_srtm *ui;
    bool modified;
    SRTM *srtm;
    const GPX_wrapper *gpxmw;
    QVector<double> values;
};

/** @} Dialogs */

#endif // DIALOG_SRTM_H
