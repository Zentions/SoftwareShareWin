#include "serveritem.h"
#include "ui_serveritem.h"
#include <QDebug>

ServerItem::ServerItem(QFrame *parent) :
    QFrame(parent),
    ui(new Ui::ServerItem)
{
    ui->setupUi(this);
    this->setStyleSheet("ServerItem{border: 2px solid #FF00FF; border-radius: 5px;};");
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->label_10->setText("0 min");
    if(image.load("./photo/unknown.png"))
    {
        QPixmap pix = QPixmap::fromImage(image);
        QPixmap fitpixmap=pix.scaled(ui->label_7->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_7->setAlignment(Qt::AlignCenter);
        ui->label_7->setPixmap(fitpixmap);
    }
}
ServerItem::ServerItem(UserInfo userInfo,int index,QFrame *parent) :
    QFrame(parent),
    ui(new Ui::ServerItem)
{
    ui->setupUi(this);
    this->setStyleSheet("ServerItem{border: 2px solid #FF00FF; border-radius: 5px;};");
    ui->listWidget->setStyleSheet("QListWidget{border:1px solid gray; color:black;background-image: url(5.png)}"
                                   "QListWidget::Item{padding-top:10px; padding-bottom:10px; }"
                                   "QListWidget::Item:hover{background:skyblue; }"
                                   "QListWidget::item:selected{background:lightgray; color:red; }"
                                   "QListWidget::item:selected:!active{border-width:0px; background:lightgreen; }"
                                   );

    ui->label_8->setAlignment(Qt::AlignHCenter);
    this->userInfo = userInfo;
    ui->label_2->setText(userInfo.getAddress());
    ui->label_2->setWordWrap(true);
    ui->label_2->setAlignment(Qt::AlignTop);
    ui->label_2->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_4->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_6->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_8->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_4->setText(userInfo.getIp());
    ui->label_6->setText(userInfo.getMac());
    if(userInfo.isEnd())
    {
        qDebug()<<userInfo.isEnd();
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->label_8->setText("共享结束");
    }
    QList<QString> list = userInfo.getSoftwareName();
    for(int i=0;i<list.size();i++)
    {
        QListWidgetItem *item = new QListWidgetItem(list.at(i), ui->listWidget);
    }
    this->index = index;
    QString path = "./photo/"+QString::number(index % 4)+".png";
    if(image.load(path))
    {
        QPixmap pix = QPixmap::fromImage(image);
        QPixmap fitpixmap=pix.scaled(ui->label_7->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->label_7->setAlignment(Qt::AlignCenter);
        ui->label_7->setPixmap(fitpixmap);
    }
    init("已连接");
}
void ServerItem::init(QString tip)
{
    ui->pushButton_3->setEnabled(false);
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(true);
    userInfo.setEnd(false);
    ui->label_8->setText(tip);
    this->use = false;
    ui->label_10->setText("0 min");
    this->time = 0;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(60000);
}
bool ServerItem::isEnd()
{
    return userInfo.isEnd();
}
void ServerItem::FirstStoreRecord()
{
    QString ip = ParaUtil::get_localmachine_ip().at(0).toString();
    QString mac = ParaUtil::gethostMac();
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int timeT = time.toTime_t();   //将当前时间转为时间戳
    qDebug()<<"timestap="<<timeT;
    userInfo.setTimestap(timeT);

    HttpUtil * http = new HttpUtil;
    QString req = "server_mac="+userInfo.getMac()+"&server_ip="+userInfo.getIp()+"&server_address="+userInfo.getAddress()+"&start_timestap="
            +QString::number(timeT)+"&client_mac="+mac+"&client_ip="+ip+"&client_address="+ParaUtil::address+"&money="+QString::number(userInfo.getMoney());
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(connectServerResult(QString)));
    http->sendRequest(ParaUtil::url+"firstStoreRecord",req,true);
}
void ServerItem::connectServerResult(QString str)
{
    QString address = JsonUtil::ParseConnectServerResult(str);
    if(address != nullptr)
    {
        if(address=="0")
        {
            ui->label_8->setText("代币数量不足");
        }
        else
        {
           init("已重新初始化");
        }

    }
    else
    {
        ui->label_8->setText("连接服务端发生错误");
    }

}
ServerItem::~ServerItem()
{
    delete ui;
}
void Delay_MSec(unsigned int msec)
{
    QTime _Timer = QTime::currentTime().addMSecs(msec);

    while( QTime::currentTime() < _Timer )

    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
void ServerItem::endUse(bool endByHand)
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int endTime = time.toTime_t();   //将当前时间转为时间戳
    int total = endTime - userInfo.getTimestap();
    double money;
    if(endByHand) money = (total/3600 + (total%(3600)==0?0:1))*userInfo.getMoney();
    else money = (total/3600)*userInfo.getMoney();
    qDebug()<<endTime;
    qDebug()<<money;
    HttpUtil * http = new HttpUtil;
    QString req = "money="+QString::number(money)+"&total_time="+QString::number(total)+"&start_timestap="+QString::number(userInfo.getTimestap())
            +"&end_timestap="+QString::number(endTime)+"&client_address="+ParaUtil::address+"&server_address="+userInfo.getAddress();
    qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(endStoreRecordResult(QString)));
    http->sendRequest(ParaUtil::url+"endStoreRecord",req,true);
}

void ServerItem::on_pushButton_2_clicked()
{
    closeAllSoftware();
    endUse(true);
}
void ServerItem::endStoreRecordResult(QString str)
{
    bool result = JsonUtil::ParseSimpleResult(str);
    if(result)
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->label_8->setText("共享结束");
        userInfo.setEnd(true);
        m_pTimer->stop();
    }
    else
    {
        ui->label_8->setText("共享结束异常");
        Delay_MSec(5000);
        endUse(true);
    }
}

void ServerItem::on_pushButton_clicked()
{
    HttpUtil * http = new HttpUtil;
    QString req = "key="+ParaUtil::address+QString::number(userInfo.getTimestap());
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(canUseResult(QString)));
    http->sendRequest(ParaUtil::url+"canUseNow?"+req,NULL,false);
}
void ServerItem::canUseResult(QString str)
{
    int result = JsonUtil::ParseSuccessAndBoolResult(str,"use");
    if(result==0)
    {
        ui->label_8->setText("服务异常");
    }
    else if(result==1)
    {
        use = true;
        ui->pushButton_3->setEnabled(true);
        ui->label_8->setText("欢迎使用");
    }
    else
    {
        ui->label_8->setText("请等待交易确认");
    }
}

void ServerItem::on_pushButton_3_clicked()
{
    if(ui->listWidget->currentItem()!=NULL)
    {
        QString name = ui->listWidget->currentItem()->text();
        software softw = userInfo.getUserSoftwares().value(name);
        opened.insert(softw.name);
        ParaUtil::writeFile(name+".exp","yifei",userInfo.getIp(),userInfo.getPass(),softw.start);
        StartAppThread *thread = new StartAppThread(name+".exp");
        thread->start();
    }
}
void ServerItem::handleTimeout()
{
    this->time++;
    if(time % 2==0 && time !=0)
    {
        HttpUtil * http = new HttpUtil;
        QString req = "server_address="+userInfo.getAddress()+"&client_address="+ParaUtil::address+"&money="+QString::number(userInfo.getMoney())
                +"&key="+ParaUtil::address+QString::number(userInfo.getTimestap());
        qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(continueToPayResult(QString)));
        http->sendRequest(ParaUtil::url+"continueToPay",req,true);
    }
    ui->label_10->setText(QString::number(time)+" min");
}
void ServerItem::continueToPayResult(QString str)
{
    QString address = JsonUtil::ParseConnectServerResult(str);
    if(address != nullptr)
    {
        if(address=="0")
        {
            ui->pushButton->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->pushButton_3->setEnabled(false);
            ui->label_8->setText("代币数量不足");
            userInfo.setEnd(true);
            m_pTimer->stop();
            endUse(false);
        }
        else
        {
            ui->label_8->setText("成功续费，继续使用");
        }

    }
    else
    {
        ui->pushButton->setEnabled(false);
        ui->pushButton_2->setEnabled(false);
        ui->pushButton_3->setEnabled(false);
        ui->label_8->setText("连接服务端发生错误");
        userInfo.setEnd(true);
        endUse(false);
        m_pTimer->stop();
    }
}
void ServerItem::closeAllSoftware()
{
    if(opened.size()>0)
    {
        QStringList list;
        foreach (const QString name, opened)
        {
            list.append(userInfo.getUserSoftwares().value(name).start);
        }
        ParaUtil::writeCloseFile("close.exp","yifei",userInfo.getIp(),userInfo.getPass(),list);
        StartAppThread *thread = new StartAppThread("close.exp");
        thread->start();
    }
}

