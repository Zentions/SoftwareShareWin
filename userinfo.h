#ifndef USERINFO_H
#define USERINFO_H

#include <QString>
#include <QMap>
#include <QList>

struct software{
    int index;
    QString date;
    QString name;
    int score;
    QString start;
};

class UserInfo
{
private:
    QString address;
    QString mac;
    QString ip;
    QString pass;
    double money;
    int score;
    int start_timestap;
    QMap<int,software> UserSoftwares;
    bool end;
public:
    UserInfo();
    UserInfo(QString address,QString mac,QString ip,QString pass,uint score);
    void insetUserSoftwares(software);
    QMap<int,software> getUserSoftwares();
    QString getAddress();
    QString getMac();
    QString getIp();
    int getScore();
    void setAddress(QString);
    void setMac(QString);
    void setIP(QString);
    void setScore(int);
    void setTimestap(int);
    int getTimestap();
    QString getPass();
    bool isEnd();
    void setEnd(bool);
    void setPass(QString);
    int getSWLen();
    double getMoney();
    void setMoney(double);
    QList<int> getSoftwareIndex();
};

#endif // USERINFO_H
