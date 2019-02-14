#ifndef PARAUTIL_H
#define PARAUTIL_H
#include <QNetworkInterface>

class ParaUtil
{
public:
    ParaUtil();

    static QString address;

    static QList<QHostAddress> get_localmachine_ip();

    static QString gethostMac();
};

#endif // PARAUTIL_H
