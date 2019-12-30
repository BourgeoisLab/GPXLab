#include "dialog_about.h"
#include "ui_dialog_about.h"
#include "gpxlab.h"

Dialog_about::Dialog_about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_about)
{
    ui->setupUi(this);

    ui->labelMain->setText(GPXLab::appName + " v" + GPXLab::appVersion);
    ui->labelCopyright->setText("© 2014 - 2019 " + GPXLab::organisationName);
    ui->labelQtVersion->setText("Qt " + QString(QT_VERSION_STR));
}

Dialog_about::~Dialog_about()
{
    delete ui;
}
