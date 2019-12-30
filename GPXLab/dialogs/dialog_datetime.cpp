#include "dialog_datetime.h"
#include "ui_dialog_datetime.h"

Dialog_DateTime::Dialog_DateTime(const QString title, const time_t timestamp, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_DateTime)
{
    ui->setupUi(this);
    setWindowTitle(title);
    QDateTime dt;
    dt.setTime_t(timestamp);
    ui->dateTimeEdit->setDateTime(dt);
}

Dialog_DateTime::~Dialog_DateTime()
{
    delete ui;
}

QDateTime Dialog_DateTime::getDateTime() const
{
    return ui->dateTimeEdit->dateTime();
}

time_t Dialog_DateTime::getTimestamp() const
{
    QDateTime dt = ui->dateTimeEdit->dateTime();
    return dt.toTime_t();
}
