#include "appmanagedialog.h"
#include "ui_appmanagedialog.h"

AppManageDialog::AppManageDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AppManageDialog)
{
    ui->setupUi(this);
    this->setStyleSheet("QTreeWidget{background-image: url(4.png)}"
                            "QTreeWidget::item{height:50px;width:50px;}");
    ui->treeWidget->setColumnWidth(0,150);
    ui->treeWidget->setColumnWidth(1,150);
    ui->treeWidget->setColumnWidth(2,150);
    ui->treeWidget->setColumnWidth(3,150);
    ui->userEdit->setText("share");
    ui->userEdit->setEnabled(false);
    updateApp();

}
void AppManageDialog::updateApp()
{
    HttpUtil * http = new HttpUtil;
    QString req = "address="+MainWindow::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(getSoftwareResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/getSoftware?"+req,NULL,false);
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
        QString req = "address="+MainWindow::address+"&name="+name+"&start="+path+"&date="+current_date;
        //qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(storeSoftwareResult(QString)));
        http->sendRequest("http://127.0.0.1:3000/storeSoftware",req,true);
    }
}

void AppManageDialog::on_delButton_clicked()
{
    int count = ui->treeWidget->topLevelItemCount();
    for(int i=0;i<count;i++)
    {
        if(ui->treeWidget->topLevelItem(i)->checkState(0)==Qt::Checked)
        {
            QString name = ui->treeWidget->topLevelItem(i)->text(0).trimmed();
            HttpUtil* http = new HttpUtil;
            QString req = "address="+MainWindow::address+"&name="+name;
            //qDebug()<<req;
            connect(http, SIGNAL(httpFinished(QString)), this, SLOT(delSoftWareResult(QString)));
            http->sendRequest("http://127.0.0.1:3000/deleteSoftWare",req,true);
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
            writeFile(name+".exp","yifei","123456",start);
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
void AppManageDialog::getSoftwareResult(QString str)
{
    QVector<software*> vec = JsonUtil::ParseSoftwareResult(str);
    for(int i=0;i<vec.size();i++)
    {
        QTreeWidgetItem* subItem = new QTreeWidgetItem(ui->treeWidget);
        subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        subItem->setText(0,vec[i]->name);
        subItem->setText(1,vec[i]->start);
        subItem->setText(2,vec[i]->score);
        subItem->setText(3,vec[i]->date);
        subItem->setCheckState(0,Qt::Unchecked);
    }
}
void AppManageDialog::storeSoftwareResult(QString str)
{
    software sw = JsonUtil::ParseSingleSoftwareResult(str);
    QTreeWidgetItem* subItem = new QTreeWidgetItem(ui->treeWidget);
    subItem->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    subItem->setText(0,sw.name);
    subItem->setText(1,sw.start);
    subItem->setText(2,sw.score);
    subItem->setText(3,sw.date);
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
void AppManageDialog::writeFile(QString fileName,QString name,QString pass,QString app)
{
    QFile file(fileName);
    // Trying to open in WriteOnly and Text mode
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not open file for writing";
        return;
    }
    QTextStream out(&file);
    out << "#!/usr/bin/expect\n";
    out << "spawn ssh -X "+name+"@127.0.0.1\n";
    out << "expect {\n";
    out << "\"yes/no\" { send \"yes\\r\"; exp_continue}\n";
    out << "\"password:\" { send \""+pass+"\\r\" }\n";
    out << "}\n";
    out << "expect \"*#\"\n";
    out << "send \""+app+"\\r\"\n";
    out << "set timeout -1\n";
    out << "send \"exit\\r\"\n";
    out << "expect eof\n";
    file.flush();
    file.close();
}
void AppManageDialog::delSoftWareResult(QString str)
{
    QString name = JsonUtil::ParseDelSoftwareResult(str);
    if(name != nullptr)
    {
            int count  = ui->treeWidget->topLevelItemCount();
            for(int i=0;i<count;i++)
            {
                if(ui->treeWidget->topLevelItem(i)->checkState(0)==Qt::Checked &&
                        ui->treeWidget->topLevelItem(i)->text(0).trimmed() == name)
                {

                    delete ui->treeWidget->topLevelItem(i);
                    break;
                }
            }
    }
}
