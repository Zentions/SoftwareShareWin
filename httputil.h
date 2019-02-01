#ifndef HTTPUTIL_H
#define HTTPUTIL_H

#include <QObject>
#include <QString>
#include <QTextCodec>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class HttpUtil : public QObject
{
    Q_OBJECT
public:
    explicit HttpUtil(QObject *parent = nullptr);
    void sendRequest(QString,QString,bool);
signals:
    void httpFinished(QString);
public slots:
    void getReplyFinished(QNetworkReply *);


private:
    QNetworkAccessManager *manager;
    QNetworkRequest network_request;
    QNetworkReply *network_reply;
    QString replyStr;
    int type;
};

#endif // HTTPUTIL_H
