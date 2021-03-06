#include "userinfo.h"


UserInfo::UserInfo(QString address,QString mac,QString ip,QString pass,uint score)
{
    this->address = address;
    this->mac = mac;
    this->ip = ip;
    this->pass = pass;
    this->score = score;
    this->end = false;
}
UserInfo::UserInfo()
{
    this->end = false;
}
void UserInfo::insetUserSoftwares(software sw)
{
    UserSoftwares.insert(sw.index,sw);
}
QMap<int,software> UserInfo::getUserSoftwares()
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
QList<int> UserInfo::getSoftwareIndex()
{
    return this->UserSoftwares.keys();
}
void UserInfo::setTimestap(int time)
{
    this->start_timestap = time;
}
int UserInfo::getTimestap()
{
    return start_timestap;
}
bool UserInfo::isEnd()
{
    return end;
}
void UserInfo::setEnd(bool end)
{
    this->end = end;
}
double UserInfo::getMoney()
{
    return money;
}
void UserInfo::setMoney(double money)
{
    this->money = money;
}
void UserInfo::setCpu(QString cpu)
{
    this->cpu = cpu;
}
void UserInfo::setMemory(QString mem)
{
    this->memory = mem;
}
QString UserInfo::getCpu()
{
    return cpu;
}
QString UserInfo::getMemory()
{
    return this->memory;
}
