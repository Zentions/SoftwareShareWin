#ifndef JSONUTIL_H
#define JSONUTIL_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QVector>
#include <QVariantMap>
#include "userinfo.h"

class JsonUtil : public QObject
{
    Q_OBJECT
public:
    explicit JsonUtil(QObject *parent = nullptr);
    static bool ParseSimpleResult(QString);
    static int ParseIsRegisterResult(QString);
    static QString ParseNewAccountResult(QString);
    static QVector<software*> ParseSoftwareResult(QString);
    static software ParseSingleSoftwareResult(QString);
    static QString ParseDelSoftwareResult(QString);
    static QString ParseGetPassResult(QString);
    static QVector<QString> ParseShareUserResult(QString);
    static UserInfo ParseshareUserInfoResult(QString);
signals:

public slots:
};

#endif // JSONUTIL_H
