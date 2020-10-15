#include "dialog_checkupdate.h"
#include "ui_dialog_checkupdate.h"
#include <QJsonDocument>
#include <QJsonObject>

Dialog_CheckUpdate::Dialog_CheckUpdate(const QString &url, const QDate& since, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_CheckUpdate),
    url(url),
    mHasUpdate(false),
    mDateSince(since)
{
    ui->setupUi(this);
    adjustSize();
}

Dialog_CheckUpdate::~Dialog_CheckUpdate()
{
    delete ui;
}

void Dialog_CheckUpdate::checkForUpdate()
{
    mHasUpdate = false;
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(httpFinished()));
}

bool Dialog_CheckUpdate::hasUpdate() const
{
    return mHasUpdate;
}

bool Dialog_CheckUpdate::ignoreUpdate() const
{
    return ui->checkBox_ignor->isChecked();
}

bool Dialog_CheckUpdate::doCheckUpdate() const
{
    return ui->cbCheckForUpdates->isChecked();
}

void Dialog_CheckUpdate::httpFinished()
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray byteArray = reply->readAll();
        if (url.host() == "api.github.com")
            mHasUpdate = parseReplyGithub(byteArray);
        else
            qWarning() << "Dialog_CheckUpdate: Unknown host:" << url.host();
    }
    else
    {
        qWarning() << "Dialog_CheckUpdate: Network error:" << reply->error();
    }
    emit finished();
}

bool Dialog_CheckUpdate::parseReplyGithub(const QByteArray& str)
{
    QJsonParseError jsonError;
    QJsonDocument reply = QJsonDocument::fromJson(str, &jsonError);
    if (jsonError.error != QJsonParseError::ParseError::NoError)
    {
        qWarning("Dialog_CheckUpdate: Failed to parse JSON");
        return false;
    }

    QDate dtPublished = QDate::fromString(reply.object().value("published_at").toString(), Qt::ISODate);

    if (dtPublished > mDateSince)
    {
        QLocale locale = QLocale();
        ui->lblName->setText(reply.object().value("name").toString());
        ui->lblDate->setText(locale.toString(dtPublished, QLocale::ShortFormat));
        ui->textEdit->setText(reply.object().value("body").toString());
        ui->lblUrl->setText("<a href=\"" + reply.object().value("html_url").toString() + "\">Download</a>");
        return true;
    }

    return false;
}
