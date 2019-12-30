#ifndef DIALOG_SETTINGS_H
#define DIALOG_SETTINGS_H

#include <QDialog>
#include "settings.h"

/**
 * @addtogroup Dialogs Dialogs
 * @brief Dialogues
 * @{
 */

namespace Ui {
class Dialog_settings;
}

/**
 * @class Dialog_settings
 *
 * @brief Settings dialogue
 */
class Dialog_settings : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructs the dialogue
     * @param settings Settings
     * @param parent Parent
     */
    explicit Dialog_settings(Settings *settings, QWidget *parent = nullptr);
    ~Dialog_settings();

private slots:

    void on_Dialog_settings_accepted();
    void on_checkBoxMapPersistentCache_toggled(bool checked);
    void on_pushButtonMapCacheLocationSelect_clicked();
    void on_pushButtonMapClearCache_clicked();
    void on_pushButtonMapCacheLocationDefault_clicked();
    void on_pushButtonTilesURLDefault_clicked();

private:

    Ui::Dialog_settings *ui;
    Settings *settings;
};

/** @} Dialogs */

#endif // DIALOG_SETTINGS_H
