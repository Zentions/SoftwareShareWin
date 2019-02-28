#include "markdialog.h"
#include "ui_markdialog.h"
#include <QDebug>

MarkDialog::MarkDialog(UserInfo user,QSet<int> indexs,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarkDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/logo.ico"));
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setWindowTitle("评分");
    ui->pushButton->setStyleSheet("border:2px groove gray;border-radius:20px;");
    if(leftImage.load("./photo/left.png"))
    {
        QPixmap pix = QPixmap::fromImage(leftImage);
        ui->pushButton->setIcon(pix);
    }
    ui->pushButton_2->setStyleSheet("border:2px groove gray;border-radius:20px;");
    if(leftImage.load("./photo/right.png"))
    {
        QPixmap pix1 = QPixmap::fromImage(leftImage);
        ui->pushButton_2->setIcon(pix1);
    }
    ui->label->setAlignment(Qt::AlignHCenter);
    ui->label->setStyleSheet("color:red;");
    ui->label->setTextInteractionFlags(Qt::TextSelectableByMouse);
//    item1 = new MarkItem;
//    ui->gridLayout->addWidget(item1,0,0,-1,1);
    this->user = user;
    list = indexs.toList();
    current=0;
    total = indexs.size();
    foreach (const int index, indexs)
    {
        software s = user.getUserSoftwares().value(index);
        MarkItem* item = new MarkItem(s.name,QString::number((s.score/10.0),'f',1));
        items.insert(index,item);
    }
    ui->pushButton->setEnabled(false);
    if(total>3)
    {
         ui->pushButton_2->setEnabled(true);
         for(int i=0;i<3;i++)
         {
             MarkItem* item = items.value(list.at(current+i));
             ui->gridLayout->addWidget(item,i,0,-1,1);
         }
    }
    else
    {
        ui->pushButton_2->setEnabled(false);
        MarkItem* item0 = items.value(list.at(0));
        ui->gridLayout->addWidget(item0,0,0,-1,1);
        if(total>=2)
        {
            MarkItem* item1 = items.value(list.at(1));
            ui->gridLayout->addWidget(item1,1,0,-1,1);
        }
        if(total==3)
        {
            MarkItem* item2 = items.value(list.at(2));
            ui->gridLayout->addWidget(item2,2,0,-1,1);
        }
    }
    mark= "";
}

MarkDialog::~MarkDialog()
{
    delete ui;
}

void MarkDialog::on_pushButton_clicked()
{
    for(int i=0;i<3;i++)
    {
        QLayoutItem *layoutItem0 = ui->gridLayout->itemAt(0);
        if(layoutItem0!=NULL)
        {
            QWidget* w0 = layoutItem0->widget();
            ui->gridLayout->removeWidget(w0);
            w0->setParent(NULL);
        }
    }
    current -= 3;
    if(total -current>=3)
    {
        for(int i=0;i<3;i++)
        {
            MarkItem* item = items.value(list.at(current+i));
            ui->gridLayout->addWidget(item,i,0,-1,1);
        }
    }
    else if(total-current>=1)
    {
        MarkItem* item = items.value(list.at(current));
        ui->gridLayout->addWidget(item,0,0,-1,1);
    }
    else if(total-current==2)
    {
        MarkItem* item = items.value(list.at(current+1));
        ui->gridLayout->addWidget(item,1,0,-1,1);
    }
    if(total - current >3) ui->pushButton_2->setEnabled(true);
    else ui->pushButton_2->setEnabled(false);
    if(current==0) ui->pushButton->setEnabled(false);
    else ui->pushButton->setEnabled(true);
}

void MarkDialog::on_pushButton_2_clicked()
{
    for(int i=0;i<3;i++)
    {
        QLayoutItem *layoutItem0 = ui->gridLayout->itemAt(0);
        if(layoutItem0!=NULL)
        {
            QWidget* w0 = layoutItem0->widget();
            ui->gridLayout->removeWidget(w0);
            w0->setParent(NULL);
        }
    }
    current += 3;
    if(total -current>=3)
    {
        for(int i=0;i<3;i++)
        {
            MarkItem* item = items.value(list.at(current+i));
            ui->gridLayout->addWidget(item,i,0,-1,1);
        }
    }
    else if(total-current>=1)
    {
        MarkItem* item = items.value(list.at(current));
        ui->gridLayout->addWidget(item,0,0,-1,1);
    }
    else if(total-current==2)
    {
        MarkItem* item = items.value(list.at(current+1));
        ui->gridLayout->addWidget(item,1,0,-1,1);
    }
    if(total - current >3) ui->pushButton_2->setEnabled(true);
    else ui->pushButton_2->setEnabled(false);
    if(current==0) ui->pushButton->setEnabled(false);
    else ui->pushButton->setEnabled(true);
}

void MarkDialog::on_pushButton_3_clicked()
{
    mark="";
    int i;
    for(i=0;i<list.size()-1;i++)
    {
        int soft_mark = items.value(list.at(i))->getValue();
        if(soft_mark==0)
        {
            ui->label->setText("请将所有使用过的软件完成评分");
            return;
        }
        QString singleMark = QString::number(list.at(i))+":"+QString::number(soft_mark*10)+";";
        mark+=singleMark;
    }
    int soft_mark = items.value(list.at(i))->getValue();
    if(soft_mark==0)
    {
        ui->label->setText("请将所有使用过的软件完成评分");
        return;
    }
    QString singleMark = QString::number(list.at(i))+":"+QString::number(soft_mark*10);
    mark+=singleMark;
    qDebug()<<mark;
    sendRequest();
}
void MarkDialog::undateScoreResult(QString str)
{
     bool result = JsonUtil::ParseSimpleResult(str);
     if(!result)
     {
        ParaUtil::Delay_MSec(5000);
        sendRequest();
     }
}
void MarkDialog::sendRequest()
{
    HttpUtil * http = new HttpUtil;
    QString req = "server_address="+user.getAddress()+"&client_address="+ParaUtil::address+"&data="+mark;
    qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(undateScoreResult(QString)));
    http->sendRequest(ParaUtil::url+"undateScore",req,true);
    this->close();
}
