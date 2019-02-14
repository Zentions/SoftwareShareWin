#include "parautil.h"

ParaUtil::ParaUtil()
{

}
QString ParaUtil::address = "0";
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
