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
   // qDebug()<<userInfo;
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
    ui->pushButton_3->setEnabled(false);
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
    this->use = false;
    ui->label_10->setText("0 min");
    this->time = 0;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
    m_pTimer->start(60000);
}
ServerItem::~ServerItem()
{
    delete ui;
}


void ServerItem::on_pushButton_2_clicked()
{
    for(int i=0;i<threads.size();i++)
    {
        if(threads.at(i)->isRunning())
        {
            ui->label_8->setText("请结束应用");
            return;
        }

    }
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int endTime = time.toTime_t();   //将当前时间转为时间戳
    int total = endTime - userInfo.getTimestap();
    int money = total/3600 + (total%(3600)==0?0:1);
    qDebug()<<endTime;
    qDebug()<<money;
    HttpUtil * http = new HttpUtil;
    QString req = "money="+QString::number(money)+"&total_time="+QString::number(total)+"&start_timestap="+QString::number(userInfo.getTimestap())
            +"&end_timestap="+QString::number(endTime)+"&client_address="+ParaUtil::address+"&server_address="+userInfo.getAddress();
    qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(endStoreRecordResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/endStoreRecord",req,true);
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
    }
    else
    {
        ui->label_8->setText("共享结束异常");
    }
}

void ServerItem::on_pushButton_clicked()
{
    HttpUtil * http = new HttpUtil;
    QString req = "key="+userInfo.getAddress()+QString::number(userInfo.getTimestap());
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(canUseResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/canUseNow?"+req,NULL,false);
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
        ParaUtil::writeFile(name+".exp","yifei",userInfo.getIp(),userInfo.getPass(),softw.start);
        StartAppThread *thread = new StartAppThread(name+".exp");
        threads.append(thread);
        thread->start();
    }
}
void ServerItem::handleTimeout()
{
    this->time++;
    ui->label_10->setText(QString::number(time)+" min");
}
