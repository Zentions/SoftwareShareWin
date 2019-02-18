#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->layoutWidget1->hide();
    ui->layoutWidget2->hide();
    ui->layoutWidget3->hide();
    ui->layoutWidget4->hide();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    ui->label_7->setAlignment(Qt::AlignHCenter);
    ui->label_7->setStyleSheet("color:red;");
    ui->label_7->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_5->setAlignment(Qt::AlignHCenter);
    ui->label_5->setStyleSheet("color:red;");
    ui->label_5->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_6->setAlignment(Qt::AlignHCenter);
    ui->label_6->setStyleSheet("color:blue;");
    ui->label_6->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_8->setAlignment(Qt::AlignHCenter);
    ui->label_8->setStyleSheet("color:red;");
    ui->label_8->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->label_12->setAlignment(Qt::AlignHCenter);
    ui->label_12->setStyleSheet("color:blue;");
    ui->label_12->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->pushButton_12->setEnabled(false);
    isShare = false;
    //get_localmachine_ip();
    //gethostMac();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

   ParaUtil::address = ui->lineEdit->text();
   QString pass = ui->lineEdit_2->text();
   HttpUtil * http0 = new HttpUtil;
   QString req = "address="+ParaUtil::address+"&pass="+pass;
   //qDebug()<<req;
   connect(http0, SIGNAL(httpFinished(QString)), this, SLOT(loginResult(QString)));
   http0->sendRequest("http://127.0.0.1:3000/login",req,true);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->layoutWidget->hide();
    ui->layoutWidget1->setGeometry(QRect(0, 110, 401, 131));
    ui->layoutWidget1->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString pass1 = ui->lineEdit_3->text();
    QString pass2 = ui->lineEdit_4->text();
    if(pass1==NULL || pass2 == NULL) ui->label_7->setText("密码为空!");
    else if(pass1.length()<6) ui->label_7->setText("密码长度应大于6!");
    else if(pass1 != pass2) ui->label_7->setText("密码不一致");
    else
    {
        ui->label_7->setText("");
        HttpUtil * http = new HttpUtil;
        QString req = "pass="+pass1;
        qDebug()<<req;
        connect(http, SIGNAL(httpFinished(QString)), this, SLOT(newAccountResult(QString)));
        http->sendRequest("http://127.0.0.1:3000/newAccount",req,true);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->layoutWidget1->hide();
    ui->layoutWidget->show();
}
void MainWindow::loginResult(QString str)
{
    int result = JsonUtil::ParseSuccessAndBoolResult(str,"Register");
    if(result !=0 )
    {
        if(result ==1)
        {
            ui->layoutWidget->hide();
            ui->layoutWidget4->setGeometry(QRect(60, 130, 311,100));
            ui->pushButton_7->setEnabled(false);
             ui->label_8->setText("");
            ui->layoutWidget4->show();
        }
        else
        {
            ui->layoutWidget->hide();
            ui->layoutWidget4->setGeometry(QRect(60, 130, 311,100));
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            ui->layoutWidget4->show();
        }
    }
    else
    {
       ui->label_5->setText("账户异常");
    }
}
void MainWindow::startServiecResult(QString str)
{
    bool result = JsonUtil::ParseSimpleResult(str);
    if(result)
    {
        ui->label_12->setStyleSheet("color:blue;");
        ui->label_12->setText("开启分享");
        ui->pushButton_11->setEnabled(false);
        ui->pushButton_12->setEnabled(true);
        isShare = true;
    }
    else
    {
        ui->label_12->setStyleSheet("color:red;");
        ui->label_12->setText("开启异常");
        isShare = false;
    }
}
void MainWindow::endServiecResult(QString str)
{
    bool result = JsonUtil::ParseSimpleResult(str);
    if(result)
    {
        ui->label_12->setStyleSheet("color:blue;");
        ui->label_12->setText("结束分享");
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_11->setEnabled(true);
        isShare = false;
    }
    else
    {
        ui->label_12->setStyleSheet("color:red;");
        ui->label_12->setText("结束异常");
        isShare = true;
    }
}
void MainWindow::newAccountResult(QString str)
{
    QString result = JsonUtil::ParseNewAccountResult(str);
    if(str!="{\"success\":false}")
    {
        ui->label_6->setText(result);
    }
    else
    {
        ui->label_7->setText("发生错误!");
    }

}

void MainWindow::on_pushButton_6_clicked()
{
    ui->layoutWidget4->hide();
    ui->layoutWidget2->setGeometry(QRect(60, 130, 311,31));
    ui->layoutWidget2->show();
}

void MainWindow::on_pushButton_3_clicked()
{
    UseSoftWareForm *form = new UseSoftWareForm;
    form->show();
}

void MainWindow::on_pushButton_10_clicked()
{
    //manage share app
    AppManageDialog *dialog = new AppManageDialog;
    connect(dialog, SIGNAL(winClose()), this, SLOT(winAppear()));
    dialog->show();
    this->hide();
}

void MainWindow::on_pushButton_9_clicked()
{
    //manage share(start or end)
    ui->layoutWidget2->hide();
    ui->layoutWidget3->setGeometry(QRect(10, 130, 421,91));
    QList<QHostAddress> list = ParaUtil::get_localmachine_ip();
    QHostAddress ip;
    ui->comboBox->clear();
    foreach (ip, list) {
        ui->comboBox->addItem(ip.toString());
    }
    this->ip = ui->comboBox->currentText();
    ui->layoutWidget3->show();
}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    qDebug()<<arg1;
    ip = ui->comboBox->currentText();
}

void MainWindow::on_pushButton_11_clicked()
{
    //start share
    QString mac = ParaUtil::gethostMac();
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address+"&mac="+mac+"&ip="+ip;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(startServiecResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/startService",req,true);
}

void MainWindow::on_pushButton_12_clicked()
{
    //end share
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(endServiecResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/endService",req,true);
}
void MainWindow::winAppear()
{
    this->show();
}

void MainWindow::on_pushButton_13_clicked()
{
    ui->layoutWidget2->hide();
    ui->layoutWidget4->show();
}

void MainWindow::on_pushButton_15_clicked()
{
    if(isShare)
    {
        ui->label_12->setStyleSheet("color:red;");
        ui->label_12->setText("请结束分享!");
    }
    else
    {
        ui->layoutWidget3->hide();
        ui->layoutWidget2->show();
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    //register
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(registerResult(QString)));
    http->sendRequest("http://127.0.0.1:3000/register",req,true);
}
void MainWindow::registerResult(QString str)
{
    bool result = JsonUtil::ParseSimpleResult(str);
    if(result)
    {
        ui->pushButton_3->setEnabled(true);
        ui->pushButton_6->setEnabled(true);
        ui->pushButton_7->setEnabled(false);
        ui->label_8->setText("");
    }
    else
    {
        ui->label_8->setText("发生错误，请重试!");
    }
}

