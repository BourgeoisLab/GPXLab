#ifndef DIALOG_MODELPROPERTIES_H
#define DIALOG_MODELPROPERTIES_H

#include <QDialog>
#include "gpx_model.h"

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_modelProperties;
}

/**
 * @class Dialog_modelProperties
 *
 * @brief Dialog subclass to edit the model properties (metadata)
 *
 * The dialogue can be used to see and modify the model properties (metadata).
 * Also it shows statistic information about the model.
 *
 * @see GPX_model
 */
class Dialog_modelProperties : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param metadata Model properties (metadata) to modify
     * @param stats Statistic information about the model
     * @param parent Parent
     */
    explicit Dialog_modelProperties(GPX_metadataType &metadata, const GPX_statsType &stats, QWidget *parent = nullptr);
    ~Dialog_modelProperties();

private slots:

    void on_Dialog_modelProperties_accepted();
    void on_pushButtonTableAdd_clicked();
    void on_pushButtonTableDelete_clicked();
    void on_pushButtonUndoName_clicked();
    void on_pushButtonUndoDesc_clicked();
    void on_pushButtonUndoKeywords_clicked();
    void on_pushButtonUndoAuthorName_clicked();
    void on_pushButtonUndoAuthorEmail_clicked();
    void on_pushButtonUndoAuthorLinkURL_clicked();
    void on_pushButtonUndoAuthorLinkText_clicked();
    void on_pushButtonUndoAuthorLinkType_clicked();
    void on_pushButtonUndoCopyrightAuthor_clicked();
    void on_pushButtonUndoCopyrightYear_clicked();
    void on_pushButtonUndoCopyrightLicense_clicked();
    void on_lineEditName_editingFinished();
    void on_lineEditDesc_editingFinished();
    void on_textEditKeywords_textChanged();
    void on_lineEditAuthorName_editingFinished();
    void on_lineEditAuthorEmail_editingFinished();
    void on_lineEditAuthorLinkURL_editingFinished();
    void on_lineEditAuthorLinkText_editingFinished();
    void on_lineEditAuthorLinkType_editingFinished();
    void on_lineEditCopyrightAuthor_editingFinished();
    void on_lineEditCopyrightYear_editingFinished();
    void on_lineEditCopyrightLicense_editingFinished();

private:

    void setModified(bool modified);

private:

    Ui::Dialog_modelProperties *ui;
    bool modified;
    GPX_metadataType &metadata;
};

/** @} Dialogs */

#endif // DIALOG_MODELPROPERTIES_H
