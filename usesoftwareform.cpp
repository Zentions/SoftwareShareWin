#include "usesoftwareform.h"
#include "ui_usesoftwareform.h"

UseSoftWareForm::UseSoftWareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UseSoftWareForm)
{
    ui->setupUi(this);
   // ServerItem *item = new ServerItem;
   // ui->gridLayout->addWidget(item,0,0,1,-1);
//    ServerItem *item1 = new ServerItem;
//    ui->gridLayout->addWidget(item1,0,1,1,-1);
    queryShareUser(false,NULL);
    ui->lineEdit->setEnabled(false);
}

UseSoftWareForm::~UseSoftWareForm()
{
    delete ui;
}

void UseSoftWareForm::on_pushButton_5_clicked()
{
    int count = ui->treeWidget->topLevelItemCount();
    for(int i=0;i<count;i++)
    {
        delete ui->treeWidget->topLevelItem(0);
    }
    queryShareUser(false,NULL);
}

void UseSoftWareForm::queryShareUser(bool byName,QString name)
{
    HttpUtil * http0 = new HttpUtil;
    connect(http0, SIGNAL(httpFinished(QString)), this, SLOT(shareUserResult(QString)));
    if(byName)
    {
        http0->sendRequest("http://127.0.0.1:3000/getShareUserBySoftware?name="+name,NULL,false);
    }
    else
    {
        http0->sendRequest("http://127.0.0.1:3000/getShareUser",NULL,false);
    }
}
void UseSoftWareForm::queryShareUserInfo(QString address)
{
    HttpUtil * http = new HttpUtil;
    QString req = "address="+address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(shareUserInfoResultForTreeWidget(QString)));
    http->sendRequest("http://127.0.0.1:3000/getShareUserInfo?"+req,NULL,false);
}
void UseSoftWareForm::shareUserResult(QString str)
{
    QVector<QString> AddressArray = JsonUtil::ParseShareUserResult(str);
    qDebug()<<AddressArray.length();
    for(int i=0;i<AddressArray.length();i++)
    {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(ui->treeWidget);
        subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        subItem->setText(0,AddressArray[i]);
        subItem->setCheckState(0,Qt::Unchecked);
        QTreeWidgetItem* ChileItem = new QTreeWidgetItem(subItem);
        ChileItem->setText(0,"用户分享信息");
    }
}


void UseSoftWareForm::on_treeWidget_itemExpanded(QTreeWidgetItem *item)
{
    if(item->parent()==NULL)
    {
        QString address = item->text(0);
        AddressMap.insert(address,item);
        queryShareUserInfo(address);
    }
}
void UseSoftWareForm::shareUserInfoResultForTreeWidget(QString str)
{
    UserInfo userInfo = JsonUtil::ParseshareUserInfoResult(str);

    QTreeWidgetItem* item = AddressMap.take(userInfo.getAddress());
    //item->removeChild();
    int count = item->childCount();
    for(int i=0;i<count;i++)
    {
        item->removeChild(item->child(0));
    }
    QTreeWidgetItem* macItem = new QTreeWidgetItem(item);
    macItem->setText(0,userInfo.getMac());
    QTreeWidgetItem* ipItem = new QTreeWidgetItem(item);
    ipItem->setText(0,userInfo.getIp());
    QTreeWidgetItem* scoreItem = new QTreeWidgetItem(item);
    scoreItem->setText(0,QString::number(userInfo.getScore()));
    QTreeWidgetItem* softwareItem = new QTreeWidgetItem(item);
    softwareItem->setText(0,"共享软件列表");
    for(int i=0;i<userInfo.getSWLen();i++)
    {
        QTreeWidgetItem* swItem = new QTreeWidgetItem(softwareItem);
        swItem->setText(0,userInfo.getSoftwareNameByIndex(i));
    }

}

void UseSoftWareForm::on_pushButton_4_clicked()
{
    int count = ui->treeWidget->topLevelItemCount();
    for(int i=0;i<count;i++)
    {
        delete ui->treeWidget->topLevelItem(0);
    }
    QString name = ui->lineEdit_2->text().trimmed();
    queryShareUser(true,name);
}

void UseSoftWareForm::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
   if(item->parent() ==NULL)
   {
       if(item->checkState(0)==Qt::Checked)
       {
           QString address  = item->text(0);
           ui->lineEdit->setText(address);
       }
       else
       {
            ui->lineEdit->setText("");
       }
   }
}

void UseSoftWareForm::on_pushButton_clicked()
{
    if(ui->lineEdit->text()!="")
    {
        if(this->ConnectUsers.contains(ui->lineEdit->text()))
        {
             QMessageBox::about(this, "warning", "您已经连接该服务端");
        }
        else
        {
            HttpUtil * http = new HttpUtil;
            QString req = "address="+ui->lineEdit->text();
            //qDebug()<<req;
            connect(http, SIGNAL(httpFinished(QString)), this, SLOT(shareUserInfoResultForConnect(QString)));
            http->sendRequest("http://127.0.0.1:3000/getShareUserInfo?"+req,NULL,false);
        }
    }
}
void UseSoftWareForm::shareUserInfoResultForConnect(QString str)
{
    UserInfo userInfo = JsonUtil::ParseshareUserInfoResult(str);
    if(userInfo.getAddress()=="error")
    {
        QMessageBox::about(this, "warning", "连接服务端发生错误");
    }
    else
    {
        QString ip = ParaUtil::get_localmachine_ip().at(0).toString();
        QString mac = ParaUtil::gethostMac();
        QDateTime time = QDateTime::currentDateTime();   //获取当前时间
        int timeT = time.toTime_t();   //将当前时间转为时间戳
        qDebug()<<"timestap="<<timeT;
        userInfo.setTimestap(timeT);
        PendUsers.insert(userInfo.getAddress(),userInfo);

        HttpUtil * http = new HttpUtil;
        QString req = "server_mac="+userInfo.getMac()+"&server_ip="+userInfo.getIp()+"&server_address="+userInfo.getAddress()+"&start_timestap="+QString::number(timeT)+"&client_mac="+mac+"&client_ip="+ip+"&client_address="+ParaUtil::address;
        qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(connectServerResult(QString)));
        http->sendRequest("http://127.0.0.1:3000/firstStoreRecord",req,true);
    }
}
void UseSoftWareForm::connectServerResult(QString str)
{
    QString address = JsonUtil::ParseConnectServerResult(str);
    if(address != nullptr)
    {
        UserInfo userInfo = PendUsers.take(address);
        ConnectUsers.insert(address,userInfo);
        ConnectAddress.append(address);
        if(ConnectAddress.size()%2==0)
        {
            current = ConnectAddress.size() -2;
            ServerItem* item = new ServerItem(userInfo,ConnectUsers.size());
            QWidget *w = ui->gridLayout->itemAt(1)->widget();
            ui->gridLayout->removeWidget(w);
            delete w;
            ui->gridLayout->addWidget(item,0,1,1,-1);
        }
        else
        {
            current = ConnectAddress.size()-1;
            QLayoutItem *w0 = ui->gridLayout->itemAt(0);
            if(w0!=NULL)
            {
                ui->gridLayout->removeWidget(w0->widget());
                delete w0->widget();
            }
            QLayoutItem *w1 = ui->gridLayout->itemAt(1);
            if(w1!=NULL)
            {
                ui->gridLayout->removeWidget(w1->widget());
                delete w1->widget();
            }
            ServerItem* item0 = new ServerItem(userInfo,ConnectUsers.size());
            ServerItem* item1 = new ServerItem;
            ui->gridLayout->addWidget(item0,0,0,1,-1);
            ui->gridLayout->addWidget(item1,0,1,1,-1);
        }
        ui->pushButton_3->setEnabled(false);
        if(current==0) ui->pushButton_2->setEnabled(false);
        ui->label_2->setText("共"+QString::number((ConnectUsers.size()+1)/2)+"页");
        ui->label->setText("当前第"+QString::number(current+1)+"页");
    }
    else
    {
        QMessageBox::about(this, "warning", "连接服务端发生错误");
    }

}

void UseSoftWareForm::on_pushButton_2_clicked()
{

}

void UseSoftWareForm::on_pushButton_3_clicked()
{

}
