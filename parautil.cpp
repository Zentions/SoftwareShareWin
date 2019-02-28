#include "parautil.h"

ParaUtil::ParaUtil()
{

}
QString ParaUtil::address = "0";
QString ParaUtil::url = "http://127.0.0.1:3000/";
QList<QHostAddress> ParaUtil::get_localmachine_ip()
{
    QString ipAddress;
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddress = ipAddressesList.at(i).toString();
            qDebug()<<ipAddress;
        }
        else
        {
            ipAddressesList.removeAt(i);
            i--;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddressesList.isEmpty())
    {
        ipAddressesList.append(QHostAddress(QHostAddress::LocalHost));
    }
    return ipAddressesList;
}
QString ParaUtil::gethostMac()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning)
                && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            qDebug()<<strMacAddr;
            //break;
        }
    }
    return strMacAddr;
}
void ParaUtil::writeFile(QString fileName,QString name,QString ip,QString pass,QString app)
{
    QFile file(fileName);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing";
        return;
    }
    QTextStream out(&file);
    out << "#!/usr/bin/expect\n";
    out << "spawn ssh -X "+name+"@"+ip+"\n";
    out << "expect {\n";
    out << "\"yes/no\" { send \"yes\\r\"; exp_continue}\n";
    out << "\"password:\" { send \""+pass+"\\r\" }\n";
    out << "}\n";
    out << "expect \":~$\"\n";
    out << "send \""+app+"\\r\"\n";
    out << "set timeout -1\n";
    out << "send \"exit\\r\"\n";
    out << "expect eof\n";
    file.flush();
    file.close();
}
void ParaUtil::writeCloseFile(QString fileName,QString name,QString ip,QString pass,QStringList app)
{
    QFile file(fileName);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing";
        return;
    }
    QTextStream out(&file);
    out << "#!/usr/bin/expect\n";
    out << "spawn ssh -X "+name+"@"+ip+"\n";
    out << "expect {\n";
    out << "\"password:\" { send \""+pass+"\\r\" }\n";
    out << "}\n";
    out << "expect \":~$\"\n";
    for(int i=0;i<app.size();i++) out << "send \"killall "+app.at(i)+"\\r\"\n";
    out << "set timeout -1\n";
    out << "send \"exit\\r\"\n";
    out << "expect eof\n";
    file.flush();
    file.close();
}
void ParaUtil::Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )

    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
