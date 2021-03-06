#include "appmanagedialog.h"
#include "ui_appmanagedialog.h"
#include <QMessageBox>
#include <QGraphicsOpacityEffect>
AppManageDialog::AppManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppManageDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/logo.ico"));
    this->setStyleSheet("QTreeWidget{background-color: rgba(255, 250, 250, 70%);}"
                            "QTreeWidget::item{height:50px;width:50px;}"
                        "QTreeWidget::indicator:checked {image: url(./photo/5.png);}"
                        "QTreeWidget::indicator:unchecked {image: url(./photo/4.png);}" );

    ui->treeWidget->setColumnWidth(0,120);
    ui->treeWidget->setColumnWidth(1,120);
    ui->treeWidget->setColumnWidth(2,120);
    ui->treeWidget->setColumnWidth(3,120);
    ui->treeWidget->setColumnWidth(4,120);
    ui->userEdit->setText("share");
    ui->userEdit->setEnabled(false);
    ui->label_5->setAlignment(Qt::AlignHCenter);
    ui->label_5->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->passEdit->setEchoMode(QLineEdit::Password);
    updateApp();
    pass = nullptr;
    getPass();
}
void AppManageDialog::updateApp()
{
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(getSoftwareResult(QString)));
    http->sendRequest(ParaUtil::url+"getSoftware?"+req,NULL,false);
}
void AppManageDialog::getPass()
{
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(getPassResult(QString)));
    http->sendRequest(ParaUtil::url+"getUserPass?"+req,NULL,false);
}
AppManageDialog::~AppManageDialog()
{
    delete ui;
}

void AppManageDialog::on_addButton_clicked()
{
    //storeSoftware
    QString name = ui->nameEdit->text().trimmed();
    QString path = ui->pathEdit->text().trimmed();
    if(name!=NULL && path!=NULL )
    {
        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy-MM-dd");
        HttpUtil* http = new HttpUtil;
        QString req = "address="+ParaUtil::address+"&name="+name+"&start="+path+"&date="+current_date;
        //qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(storeSoftwareResult(QString)));
        http->sendRequest(ParaUtil::url+"storeSoftware",req,true);
    }
}

void AppManageDialog::on_delButton_clicked()
{
    int count = ui->treeWidget->topLevelItemCount();
    for(int i=0;i<count;i++)
    {
        if(ui->treeWidget->topLevelItem(i)->checkState(0)==Qt::Checked)
        {
            QString index = ui->treeWidget->topLevelItem(i)->text(0).trimmed();
            if(index!="undefine")
            {
                HttpUtil* http = new HttpUtil;
                QString req = "address="+ParaUtil::address+"&index="+index;
                //qDebug()<<req;
                connect(http, SIGNAL(httpFinished(QString)), this, SLOT(delSoftWareResult(QString)));
                http->sendRequest(ParaUtil::url+"deleteSoftWare",req,true);
            }
            else QMessageBox::about(this, "warning", "交易还未确认，稍后再试");
            break;
        }
    }


}

void AppManageDialog::on_addButton_2_clicked()
{
    int count = ui->treeWidget->topLevelItemCount();
    for(int i=0;i<count;i++)
    {
        if(ui->treeWidget->topLevelItem(i)->checkState(0)==Qt::Checked)
        {
            QString name = ui->treeWidget->topLevelItem(i)->text(0).trimmed();
            QString start = ui->treeWidget->topLevelItem(i)->text(1).trimmed();
            ParaUtil::writeFile(name+".exp","share","127.0.0.1",pass,start);
            StartAppThread *thread = new StartAppThread(name+".exp");
            thread->start();
            break;
        }
    }
}
void AppManageDialog::closeEvent(QCloseEvent *)
{
    emit winClose();
}
void AppManageDialog::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    //draw background
    QPixmap foreImg;
    foreImg.load("./photo/3.jpg");
    if (!foreImg.isNull())
    {
        painter.setOpacity(0.4);//透明度设置
        painter.drawPixmap(0, 0,730,514, foreImg);
     }

}
void AppManageDialog::getSoftwareResult(QString str)
{
    QVector<software*> vec = JsonUtil::ParseSoftwareResult(str);
    for(int i=0;i<vec.size();i++)
    {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(ui->treeWidget);
        subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        subItem->setText(0,QString::number(vec[i]->index));
        subItem->setText(1,vec[i]->name);
        subItem->setText(2,vec[i]->start);
        subItem->setText(3,QString::number(vec[i]->score/10.0,'f',1));
        subItem->setText(4,vec[i]->date);
        subItem->setCheckState(0,Qt::Unchecked);
    }
}
void AppManageDialog::getPassResult(QString str)
{
    pass = JsonUtil::ParseGetPassResult(str);
}
void AppManageDialog::storeSoftwareResult(QString str)
{
    software sw = JsonUtil::ParseSingleSoftwareResult(str);
    QTreeWidgetItem* subItem = new QTreeWidgetItem(ui->treeWidget);
    subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    subItem->setText(0,"undefine");
    subItem->setText(1,sw.name);
    subItem->setText(2,sw.start);
    subItem->setText(3,QString::number(sw.score/10.0,'f',1));
    subItem->setText(4,sw.date);
    subItem->setCheckState(0,Qt::Unchecked);
}

void AppManageDialog::on_treeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    int count = ui->treeWidget->topLevelItemCount();
    qDebug()<<count;
    if(Qt::Checked == item->checkState(0))
    {
        for(int i=0;i<count;i++)
        {
            if(item != ui->treeWidget->topLevelItem(i))
            {
                ui->treeWidget->topLevelItem(i)->setCheckState(0,Qt::Unchecked);
            }
        }
    }

}

void AppManageDialog::delSoftWareResult(QString str)
{
    QString index = JsonUtil::ParseDelSoftwareResult(str);
    if(index != nullptr)
    {
            int count  = ui->treeWidget->topLevelItemCount();
            for(int i=0;i<count;i++)
            {
                if(ui->treeWidget->topLevelItem(i)->checkState(0)==Qt::Checked &&
                        ui->treeWidget->topLevelItem(i)->text(0).trimmed() == index)
                {

                    delete ui->treeWidget->topLevelItem(i);
                    break;
                }
            }
    }
}

void AppManageDialog::on_addButton_4_clicked()
{
    QString newPass = ui->passEdit->text().trimmed();
    if(newPass ==NULL)
    {
        ui->label_5->setStyleSheet("color:red;");
        ui->label_5->setText("密码不能为空!");
    }
    else if(newPass.length() <6)
    {
        ui->label_5->setStyleSheet("color:red;");
        ui->label_5->setText("密码长度应大于6!");
    }
    else
    {
        HttpUtil* http = new HttpUtil;
        QString req = "address="+ParaUtil::address+"&pass="+newPass;
        //qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(modifyPassResult(QString)));
        http->sendRequest(ParaUtil::url+"modifyPass",req,true);
    }
}
void AppManageDialog::modifyPassResult(QString str)
{
    bool success = JsonUtil::ParseSimpleResult(str);
    if(success)
    {
        ui->label_5->setStyleSheet("color:white;");
        ui->label_5->setText("密码更改成功");
    }
    else
    {
        ui->label_5->setStyleSheet("color:red;");
        ui->label_5->setText("密码更改失败");
    }
}
