#ifndef DIALOG_CHECKUPDATE_H
#define DIALOG_CHECKUPDATE_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

namespace Ui {
class Dialog_CheckUpdate;
}

class Dialog_CheckUpdate : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_CheckUpdate(const QString &url, const QDate& since, QWidget *parent = nullptr);
    ~Dialog_CheckUpdate();
    void checkForUpdate();
    bool hasUpdate() const;
    bool ignoreUpdate() const;
    bool doCheckUpdate() const;

signals:
    void finished();

private slots:
    void httpFinished();

private:
    bool parseReplyGithub(const QByteArray &str);

private:
    Ui::Dialog_CheckUpdate *ui;
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool mHasUpdate;
    QDate mDateSince;
};

#endif // DIALOG_CHECKUPDATE_H
