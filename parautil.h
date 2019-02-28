#ifndef PARAUTIL_H
#define PARAUTIL_H
#include <QNetworkInterface>
#include <QFile>
#include <QTime>
#include <QCoreApplication>
class ParaUtil
{
public:
    ParaUtil();

    static QString address;

    static QString url;

    static QList<QHostAddress> get_localmachine_ip();

    static QString gethostMac();

    static void writeFile(QString fileName,QString name,QString ip,QString pass,QString app);

    static void writeCloseFile(QString fileName,QString name,QString ip,QString pass,QStringList app);

    static void Delay_MSec(unsigned int msec);

};

#endif // PARAUTIL_H
