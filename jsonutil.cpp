#include "jsonutil.h"
#include "QDebug"
JsonUtil::JsonUtil(QObject *parent) : QObject(parent)
{

}
bool JsonUtil::ParseSimpleResult(QString str)
{
    QByteArray byteArray = str.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

    //开始解析
    QJsonObject object = doucment.object();  // 转化为对象
    QJsonValue Value = object.value("success");
    return Value.toBool();
}
int JsonUtil::ParseIsRegisterResult(QString str)
{
    QByteArray byteArray = str.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

    //开始解析
    QJsonObject object = doucment.object();  // 转化为对象
    QJsonValue succ_value = object.value("success");
    QJsonValue re_value = object.value("Register");
    if(!succ_value.toBool()) return 0;
    else
    {
        if(re_value.toBool()) return 1;
        else return 2;
    }
}
QString JsonUtil::ParseNewAccountResult(QString str)
{
    QByteArray byteArray = str.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

    //开始解析
    QJsonObject object = doucment.object();  // 转化为对象
    QJsonValue Value = object.value("account");
    return Value.toString();
}
QVector<software*> JsonUtil::ParseSoftwareResult(QString str)
{
    QVector<software*> software_array;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(str.toUtf8(), &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = doucment.object();  // 转化为对象
        QJsonValue swArrayValue = object.value("sw");//获取softwares对应的value数组
        QJsonArray array = swArrayValue.toArray();//将上面获取的value转化为数组
        int nSize =array.size();
        for(int i=0;i<nSize;++i)
        {
            if(array[i].isObject())
            {
                QVariantMap result = array[i].toVariant().toMap();
                QString name,start,date,score;
                name=result["name"].toString();
                start=result["start"].toString();
                date=result["date"].toString();
                score=result["score"].toString();
                software *infor=new software;
                infor->date= date;
                infor->name = name;
                infor->start = start;
                infor->score = score;
                software_array.append(infor);

            }
         }
    }
    return software_array;
}
QVector<QString> JsonUtil::ParseShareUserResult(QString str)
{
    QVector<QString> AddressArray;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(str.toUtf8(), &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = doucment.object();  // 转化为对象
        QJsonValue SuccValue = object.value("success");
        if(SuccValue.toBool())
        {
            QJsonValue AddressValue = object.value("users");
            QJsonArray array = AddressValue.toArray();
            int nSize =array.size();
            qDebug()<<nSize;
            for(int i=0;i<nSize;++i)
            {
                qDebug()<<array[i].toString();
                AddressArray.append(array[i].toString());
             }
        }

    }
    return AddressArray;
}
software JsonUtil::ParseSingleSoftwareResult(QString str)
{
    software sw;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(str.toUtf8(), &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = doucment.object();  // 转化为对象
        QJsonValue sw_success = object.value("success");
        if(sw_success.toBool())
        {
            QJsonValue sw_date = object.value("date");
            QJsonValue sw_start = object.value("start");
            QJsonValue sw_name = object.value("name");
            QJsonValue sw_score = object.value("score");
            sw.date= sw_date.toString();
            sw.name = sw_name.toString();
            sw.start = sw_start.toString();
            sw.score = sw_score.toString();
        }
    }
    return sw;
}
QString JsonUtil::ParseDelSoftwareResult(QString str)
{
    QByteArray byteArray = str.toLatin1();
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

    //开始解析
    QJsonObject object = doucment.object();  // 转化为对象
    QJsonValue value = object.value("success");
    if(value.toBool())
    {
        QJsonValue name_value = object.value("_name");
        return name_value.toString();
    }
    else return nullptr;
}
 QString JsonUtil::ParseGetPassResult(QString str)
 {
     QByteArray byteArray = str.toLatin1();
     QJsonParseError jsonError;
     QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);  // 转化为 JSON 文档

     //开始解析
     QJsonObject object = doucment.object();  // 转化为对象
     QJsonValue value = object.value("success");
     if(value.toBool())
     {
         QJsonValue pass_value = object.value("_pass");
         return pass_value.toString();
     }
     else return nullptr;
 }
UserInfo JsonUtil::ParseshareUserInfoResult(QString str)
{
    UserInfo userInfo;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(str.toUtf8(), &jsonError);  // 转化为 JSON 文档
    if (!doucment.isNull() && (jsonError.error == QJsonParseError::NoError))
    {
        QJsonObject object = doucment.object();  // 转化为对象
        QJsonValue SuccValue = object.value("success");
        if(SuccValue.toBool())
        {
            QJsonValue addressVal = object.value("address");
            QJsonValue macVal = object.value("mac");
            QJsonValue ipVal = object.value("ip");
            QJsonValue passVal = object.value("pass");
            QJsonValue scoreVal = object.value("score");
            userInfo.setAddress(addressVal.toString());
            userInfo.setIP(ipVal.toString());
            userInfo.setMac(macVal.toString());
            userInfo.setPass(passVal.toString());
            userInfo.setScore(scoreVal.toString().toInt());
            qDebug()<<userInfo.getScore();
            //array
            QJsonValue softwareValue = object.value("sws");
            QJsonValue softwareVal = softwareValue.toObject().value("sw");
            QJsonArray array = softwareVal.toArray();
            int nSize =array.size();

            for(int i=0;i<nSize;++i)
            {
                if(array[i].isObject())
                {
                    QVariantMap result = array[i].toVariant().toMap();
                    QString name,start,date,score;
                    name=result["name"].toString();
                    start=result["start"].toString();
                    date=result["date"].toString();
                    score=result["score"].toString();
                    software *infor=new software;
                    infor->date= date;
                    infor->name = name;
                    infor->start = start;
                    infor->score = score;
                    userInfo.insetUserSoftwares(*infor);

                }
             }
        }

    }
    return userInfo;
}
