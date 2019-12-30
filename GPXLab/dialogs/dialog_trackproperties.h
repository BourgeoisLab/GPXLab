#ifndef DIALOG_TRACKPROPERTIES_H
#define DIALOG_TRACKPROPERTIES_H

#include <QDialog>
#include "gpx_model.h"

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_trackProperties;
}

/**
 * @class Dialog_trackProperties
 *
 * @brief Dialog subclass to edit track properties (metadata)
 *
 * The dialogue can be used to see and modify track properties (metadata).
 * Also it shows statistic information about the track.
 *
 * @see GPX_model
 */
class Dialog_trackProperties : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param metadata Track properties (metadata) to modify
     * @param stats Statistic information about the track
     * @param parent Parent
     */
    explicit Dialog_trackProperties(GPX_trkMetadataType &metadata, const GPX_statsType &stats, QWidget *parent = 0);
    ~Dialog_trackProperties();

private slots:

    void on_Dialog_trackProperties_accepted();
    void on_pushButtonTableAdd_clicked();
    void on_pushButtonTableDelete_clicked();
    void on_pushButtonUndoName_clicked();
    void on_pushButtonUndoCmt_clicked();
    void on_pushButtonUndoDesc_clicked();
    void on_pushButtonUndoSrc_clicked();
    void on_pushButtonUndoType_clicked();
    void on_lineEditName_editingFinished();
    void on_lineEditCmt_editingFinished();
    void on_lineEditDesc_editingFinished();
    void on_lineEditSrc_editingFinished();
    void on_lineEditType_editingFinished();

private:

    void setModified(bool modified);

private:

    Ui::Dialog_trackProperties *ui;
    bool modified;
    GPX_trkMetadataType &metadata;
};

/** @} Dialogs */

#endif // DIALOG_TRACKPROPERTIES_H
