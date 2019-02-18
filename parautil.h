#ifndef PARAUTIL_H
#define PARAUTIL_H
#include <QNetworkInterface>
#include <QFile>

class ParaUtil
{
public:
    ParaUtil();

    static QString address;

    static QList<QHostAddress> get_localmachine_ip();

    static QString gethostMac();

    static void writeFile(QString fileName,QString name,QString ip,QString pass,QString app);
};

#endif // PARAUTIL_H
