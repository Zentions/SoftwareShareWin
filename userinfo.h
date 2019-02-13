#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QVector>


struct software{
    QString date;
    QString name;
    QString score;
    QString start;
};

class UserInfo
{
private:
    QString address;
    QString mac;
    QString ip;
    QString pass;
    int score;
    QVector<software> UserSoftwares;
public:
    UserInfo();
    UserInfo(QString address,QString mac,QString ip,QString pass,uint score);
    void insetUserSoftwares(software);
    QVector<software> getUserSoftwares();
    QString getAddress();
    QString getMac();
    QString getIp();
    int getScore();
    void setAddress(QString);
    void setMac(QString);
    void setIP(QString);
    void setScore(int);
    QString getPass();
    void setPass(QString);
    int getSWLen();
    QString getSoftwareNameByIndex(int i);
};

#endif // USERINFO_H
