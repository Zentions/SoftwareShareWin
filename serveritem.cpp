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
    this->userInfo = userInfo;
    ui->label_2->setText(userInfo.getAddress());
    ui->label_2->setWordWrap(true);
    ui->label_2->setAlignment(Qt::AlignTop);
    ui->label_2->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_4->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_6->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_4->setText(userInfo.getIp());
    ui->label_6->setText(userInfo.getMac());
    for(int i=0;i<userInfo.getSWLen();i++)
    {
        QListWidgetItem *item = new QListWidgetItem(userInfo.getSoftwareNameByIndex(i), ui->listWidget);
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
}
ServerItem::~ServerItem()
{
    delete ui;
}


void ServerItem::on_pushButton_2_clicked()
{
    QDateTime time = QDateTime::currentDateTime();   //获取当前时间
    int endTime = time.toTime_t();   //将当前时间转为时间戳
    int total = endTime - userInfo.getTimestap();
    int money = total/3600 + (total%(3600)==0?0:1);
    qDebug()<<endTime;
    qDebug()<<money;
    HttpUtil * http = new HttpUtil;
    QString req = "money="+QString::number(money)+"&total_time="+QString::number(total)+"&start_timestap="+QString::number(userInfo.getTimestap())
            +"&end_timestap="+QString::number(endTime)+"&client_address="+ParaUtil::address;
    qDebug()<<req;
   // connect(http, SIGNAL(httpFinished(QString)), this, SLOT(newAccountResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/endStoreRecord",req,true);
}
