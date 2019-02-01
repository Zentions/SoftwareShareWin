#include "httputil.h"


HttpUtil::HttpUtil(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getReplyFinished(QNetworkReply*)));//要放在这里，否则会绑定多次

}

/*将reply的结果显示到控制台和MessageBox中*/
void HttpUtil::getReplyFinished(QNetworkReply *reply)
{
    QString replyStr = "{\"success\":false}";

    if (reply->error() == QNetworkReply::NoError)
    {
        /*将reply的结果显示出来*/
        QTextCodec *codec = QTextCodec::codecForName("utf8");
        //使用utf8编码, 这样可以显示中文

        replyStr= codec->toUnicode(reply->readAll());
    }
    else
    {
        qDebug()<<"handle errors here";
        QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //statusCodeV是HTTP服务器的相应码，reply->error()是Qt定义的错误码，可以参考QT的文档
        qDebug( "found error ....code: %d %d\n", statusCodeV.toInt(), (int)reply->error());
    }
    reply->deleteLater();//最后要释放reply对象
    qDebug()<<replyStr;
    emit httpFinished(replyStr);
}


/*发送post或get请求*/
void HttpUtil::sendRequest(QString url,QString str,bool isPost)
{
    /*设置发送数据*/
    network_request.setUrl(QUrl(url));
    network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    if(isPost==true)
    {
        QByteArray array = str.toLocal8Bit();
        network_reply=manager->post(network_request,array);
        if(str!=NULL)
        {
            qDebug()<<str;
        }
    }
    else
    {
        manager->get(network_request);
    }

}
