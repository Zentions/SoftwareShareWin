#include "usesoftwareform.h"
#include "ui_usesoftwareform.h"

UseSoftWareForm::UseSoftWareForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UseSoftWareForm)
{
    ui->setupUi(this);
    ServerItem *item = new ServerItem;
    ui->gridLayout->addWidget(item,0,0,1,-1);
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
