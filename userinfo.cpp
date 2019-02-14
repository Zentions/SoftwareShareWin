#include "userinfo.h"


UserInfo::UserInfo(QString address,QString mac,QString ip,QString pass,uint score)
{
    this->address = address;
    this->mac = mac;
    this->ip = ip;
    this->pass = pass;
    this->score = score;
}
UserInfo::UserInfo()
{

}
void UserInfo::insetUserSoftwares(software sw)
{
    UserSoftwares.append(sw);
}
QVector<software> UserInfo::getUserSoftwares()
{
    return UserSoftwares;
}
QString UserInfo::getAddress()
{
    return this->address;
}
QString UserInfo::getMac()
{
    return this->mac;
}
QString UserInfo::getIp()
{
    return this->ip;
}
int UserInfo::getScore()
{
    return this->score;
}
void UserInfo::setAddress(QString address)
{
    this->address = address;
}
void UserInfo::setMac(QString mac)
{
    this->mac = mac;
}
void UserInfo::setIP(QString ip)
{
    this->ip = ip;
}
void UserInfo::setScore(int score)
{
    this->score = score;
}
QString UserInfo::getPass()
{
    return pass;
}
void UserInfo::setPass(QString pass)
{
    this->pass = pass;
}
int UserInfo::getSWLen()
{
    return this->UserSoftwares.size();
}
QString UserInfo::getSoftwareNameByIndex(int i)
{
    return this->UserSoftwares.at(i).name;
}
void UserInfo::setTimestap(int time)
{
    this->start_timestap = time;
}
int UserInfo::getTimestap()
{
    return start_timestap;
}
