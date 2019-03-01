#include "usesoftwareform.h"
#include "ui_usesoftwareform.h"

UseSoftWareForm::UseSoftWareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UseSoftWareForm)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/logo.ico"));
    this->setStyleSheet("QTreeWidget{background-color: rgba(255, 250, 250, 70%);}"
                        "QTreeWidget::indicator:checked {image: url(./photo/5.png);}"
                        "QTreeWidget::indicator:unchecked {image: url(./photo/4.png);}" );
    queryShareUser(false,NULL);
    ui->lineEdit->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
   // ui->label_3->setFrameShape (QFrame::Box);
   // ui->label_3->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
}

UseSoftWareForm::~UseSoftWareForm()
{
    delete ui;
}
void UseSoftWareForm::closeEvent(QCloseEvent *event)
{
    for(int i=0;i<ConnectAddress.size();i++)
    {
        QString add = ConnectAddress[i];
        if(!ConnectUsers.value(add)->isEnd())
        {
            QMessageBox::about(this, "warning", "请结束所有共享再退出！");
            event->ignore();
            return;
        }
    }
    emit winClose();
    event->accept();
}
void UseSoftWareForm::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //draw background
    QPixmap foreImg;
    foreImg.load("./photo/4.jpg");
    if (!foreImg.isNull())
    {
        painter.setOpacity(0.6);//透明度设置
        painter.drawPixmap(0, 0,rect().width(),rect().height(), foreImg);
    }
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
        http0->sendRequest(ParaUtil::url+"getShareUserBySoftware?name="+name,NULL,false);
    }
    else
    {
        http0->sendRequest(ParaUtil::url+"getShareUser",NULL,false);
    }
}
void UseSoftWareForm::queryShareUserInfo(QString address)
{
    HttpUtil * http = new HttpUtil;
    QString req = "address="+address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(shareUserInfoResultForTreeWidget(QString)));
    http->sendRequest(ParaUtil::url+"getShareUserInfo?"+req,NULL,false);
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
    macItem->setText(0,"MAC : "+userInfo.getMac());
    QTreeWidgetItem* ipItem = new QTreeWidgetItem(item);
    ipItem->setText(0,"IP : "+userInfo.getIp());
    QTreeWidgetItem* cpuItem = new QTreeWidgetItem(item);
    cpuItem->setText(0,"CPU : "+userInfo.getCpu());
    QTreeWidgetItem* memItem = new QTreeWidgetItem(item);
    memItem->setText(0,"MemTotal : "+userInfo.getMemory());
    QTreeWidgetItem* scoreItem = new QTreeWidgetItem(item);
    scoreItem->setText(0,"信誉度 : "+QString::number(userInfo.getScore()/10.0,'f',1));
    QTreeWidgetItem* moneyItem = new QTreeWidgetItem(item);
    moneyItem->setText(0,"代币/小时 : "+QString::number(userInfo.getMoney())+" ether");
    QTreeWidgetItem* softwareItem = new QTreeWidgetItem(item);
    softwareItem->setText(0,"共享软件列表");
    QList<int> list = userInfo.getSoftwareIndex();
    for(int i=0;i<list.size();i++)
    {
        software s = userInfo.getUserSoftwares().value(list.at(i));
        QTreeWidgetItem* swItem = new QTreeWidgetItem(softwareItem);
        swItem->setText(0,s.name);
        QTreeWidgetItem* scoreItem = new QTreeWidgetItem(swItem);
        scoreItem->setText(0,"评分:"+QString::number(s.score/10.0,'f',1));
        QTreeWidgetItem* dateItem = new QTreeWidgetItem(swItem);
        dateItem->setText(0,"共享日期:"+s.date);
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
           int count = ui->treeWidget->topLevelItemCount();
           for(int i=0;i<count;i++)
           {
               if(item != ui->treeWidget->topLevelItem(i))
               {
                   ui->treeWidget->topLevelItem(i)->setCheckState(0,Qt::Unchecked);
               }
           }
       }
   }

}

void UseSoftWareForm::on_pushButton_clicked()
{
    if(ui->lineEdit->text()!="")
    {
        QString add = ui->lineEdit->text();
        if(this->ConnectUsers.contains(add))
        {
            if(ConnectUsers.value(add)->isEnd())
            {
                ConnectUsers.value(add)->FirstStoreRecord();
            }
            else
            {
                QMessageBox::about(this, "warning", "您已经连接该服务端");
            }
        }
        else
        {
            HttpUtil * http = new HttpUtil;
            QString req = "address="+ui->lineEdit->text();
            //qDebug()<<req;
            connect(http, SIGNAL(httpFinished(QString)), this, SLOT(shareUserInfoResultForConnect(QString)));
            http->sendRequest(ParaUtil::url+"getShareUserInfo?"+req,NULL,false);
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
        QString req = "server_mac="+userInfo.getMac()+"&server_ip="+userInfo.getIp()+"&server_address="+userInfo.getAddress()+"&start_timestap="
                +QString::number(timeT)+"&client_mac="+mac+"&client_ip="+ip+"&client_address="+ParaUtil::address+"&money="+QString::number(userInfo.getMoney());
        //qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(connectServerResult(QString)));
        http->sendRequest(ParaUtil::url+"firstStoreRecord",req,true);
    }
}
void UseSoftWareForm::connectServerResult(QString str)
{
    QString address = JsonUtil::ParseConnectServerResult(str);
    if(address != nullptr)
    {
        if(address=="0")
        {
            QMessageBox::about(this, "warning", "代币数量不足");
        }
        else
        {
            UserInfo userInfo = PendUsers.take(address);
            //ConnectUsers.insert(address,userInfo);
            ConnectAddress.append(address);
            if(ConnectAddress.size()%2==0)
            {

                current = ConnectAddress.size() -2;
                ServerItem* item = new ServerItem(userInfo,ConnectAddress.size()-1);
                QWidget *w = ui->gridLayout->itemAt(1)->widget();
                ui->gridLayout->removeWidget(w);
                delete w;
                ui->gridLayout->addWidget(item,0,1,1,-1);
                ConnectUsers.insert(address,item);
            }
            else
            {
                qDebug()<<1234;
                current = ConnectAddress.size()-1;
                QLayoutItem *layoutItem0 = ui->gridLayout->itemAt(0);
                if(layoutItem0!=NULL)
                {
                    QWidget* w0 = layoutItem0->widget();
                    ui->gridLayout->removeWidget(w0);
                    w0->setParent(NULL);
                }
                QLayoutItem *layoutItem1 = ui->gridLayout->itemAt(0);
                if(layoutItem1!=NULL)
                {
                    QWidget* w1 = layoutItem1->widget();
                    ui->gridLayout->removeWidget(w1);
                    w1->setParent(NULL);
                }
                ServerItem* item0 = new ServerItem(userInfo,ConnectAddress.size()-1);
                ServerItem* item1 = new ServerItem;
                ui->gridLayout->addWidget(item0,0,0,1,-1);
                ui->gridLayout->addWidget(item1,0,1,1,-1);
                ConnectUsers.insert(address,item0);
            }
            ui->pushButton_3->setEnabled(false);
            if(current==0) ui->pushButton_2->setEnabled(false);
            else ui->pushButton_2->setEnabled(true);
            ui->label_2->setText("共"+QString::number((ConnectUsers.size()+1)/2)+"页");
            ui->label->setText("当前第"+QString::number(current/2+1)+"页");
        }

    }
    else
    {
        QMessageBox::about(this, "warning", "连接服务端发生错误");
    }

}

void UseSoftWareForm::on_pushButton_2_clicked()
{
    QLayoutItem *layoutItem0 = ui->gridLayout->itemAt(0);
    if(layoutItem0!=NULL)
    {
        QWidget* w0 = layoutItem0->widget();
        ui->gridLayout->removeWidget(w0);
        w0->setParent(NULL);
    }
    QLayoutItem *layoutItem1 = ui->gridLayout->itemAt(0);
    if(layoutItem1!=NULL)
    {
        QWidget* w1 = layoutItem1->widget();
        ui->gridLayout->removeWidget(w1);
        w1->setParent(NULL);
    }
    QString address1 = ConnectAddress[current-2];
    QString address2 = ConnectAddress[current-1];
    ServerItem* item1 = ConnectUsers[address1];
    ServerItem* item2 = ConnectUsers[address2];
    current = current -2;
    ui->gridLayout->addWidget(item1,0,0,1,-1);
    ui->gridLayout->addWidget(item2,0,1,1,-1);
    ui->pushButton_3->setEnabled(true);
    if(current==0) ui->pushButton_2->setEnabled(false);
    else ui->pushButton_2->setEnabled(true);
    ui->label_2->setText("共"+QString::number((ConnectUsers.size()+1)/2)+"页");
    ui->label->setText("当前第"+QString::number(current/2+1)+"页");
}

void UseSoftWareForm::on_pushButton_3_clicked()
{
    QLayoutItem *layoutItem0 = ui->gridLayout->itemAt(0);
    if(layoutItem0!=NULL)
    {
        QWidget* w0 = layoutItem0->widget();
        ui->gridLayout->removeWidget(w0);
        w0->setParent(NULL);
    }
    QLayoutItem *layoutItem1 = ui->gridLayout->itemAt(0);
    if(layoutItem1!=NULL)
    {
        QWidget* w1 = layoutItem1->widget();
        ui->gridLayout->removeWidget(w1);
        w1->setParent(NULL);
    }
    current = current +2;
    if(ConnectUsers.size()-current>=2)
    {
        QString address1 = ConnectAddress[current];
        QString address2 = ConnectAddress[current+1];
        ServerItem* item1 = ConnectUsers[address1];
        ServerItem* item2 = ConnectUsers[address2];
        ui->gridLayout->addWidget(item1,0,0,1,-1);
        ui->gridLayout->addWidget(item2,0,1,1,-1);
    }
    else
    {
        QString address = ConnectAddress[current];
        ServerItem* item0 = ConnectUsers[address];
        ServerItem* item1 = new ServerItem;
        ui->gridLayout->addWidget(item0,0,0,1,-1);
        ui->gridLayout->addWidget(item1,0,1,1,-1);
    }
    ui->pushButton_2->setEnabled(true);
    if(ConnectUsers.size()-current<=2) ui->pushButton_3->setEnabled(false);
    else ui->pushButton_3->setEnabled(true);
    ui->label_2->setText("共"+QString::number((ConnectUsers.size()+1)/2)+"页");
    ui->label->setText("当前第"+QString::number(current/2+1)+"页");
}
