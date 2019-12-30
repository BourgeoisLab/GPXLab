#include <QFileDialog>
#include "dialog_settings.h"
#include "ui_dialog_settings.h"

Dialog_settings::Dialog_settings(Settings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_settings),
    settings(settings)
{
    ui->setupUi(this);

    ui->checkBoxMapPersistentCache->setChecked(settings->doPersistentCaching);
    ui->lineEditMapCachePath->setText(settings->cachePath);
    ui->lineEditTilesURL->setText(settings->tilesURL);
    ui->pushButtonMapClearCache->setEnabled(settings->doPersistentCaching);
    ui->lineEditMapCachePath->setEnabled(settings->doPersistentCaching);
    ui->pushButtonMapCacheLocationSelect->setEnabled(settings->doPersistentCaching);
    ui->pushButtonMapCacheLocationDefault->setEnabled(settings->doPersistentCaching);
    ui->checkBoxAutoload->setChecked(settings->autoLoadLastFile);
    ui->checkBoxCheckUpdates->setChecked(settings->checkUpdate);
}

Dialog_settings::~Dialog_settings()
{
    delete ui;
}

void Dialog_settings::on_Dialog_settings_accepted()
{
    settings->doPersistentCaching = ui->checkBoxMapPersistentCache->isChecked();
    settings->cachePath = ui->lineEditMapCachePath->text();
    settings->autoLoadLastFile = ui->checkBoxAutoload->isChecked();
    settings->checkUpdate = ui->checkBoxCheckUpdates->isChecked();
    settings->tilesURL = ui->lineEditTilesURL->text();

    settings->save();
}

void Dialog_settings::on_checkBoxMapPersistentCache_toggled(bool checked)
{
    ui->pushButtonMapClearCache->setEnabled(checked);
    ui->lineEditMapCachePath->setEnabled(checked);
    ui->pushButtonMapCacheLocationSelect->setEnabled(checked);
    ui->pushButtonMapCacheLocationDefault->setEnabled(checked);
}

void Dialog_settings::on_pushButtonMapClearCache_clicked()
{
    settings->clearCache();
}

void Dialog_settings::on_pushButtonMapCacheLocationSelect_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, tr("Cache Location"), settings->cachePath);
    if (!path.isEmpty())
    {
        ui->lineEditMapCachePath->setText(path);
    }
}

void Dialog_settings::on_pushButtonMapCacheLocationDefault_clicked()
{
    ui->lineEditMapCachePath->setText(settings->defaultCachePath());
}

void Dialog_settings::on_pushButtonTilesURLDefault_clicked()
{
    ui->lineEditTilesURL->setText(settings->defaultTilesURL());
}
