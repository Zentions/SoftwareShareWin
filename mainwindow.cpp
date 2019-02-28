#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
   // setWindowOpacity(0.7) ;
    //this->setAttribute(Qt::WA_WState_WindowOpacitySet);
    this->setWindowIcon(QIcon(":/logo.ico"));
    this->setWindowTitle("软件共享");
    ui->layoutWidget1->hide();
    ui->layoutWidget2->hide();
    ui->layoutWidget3->hide();
    ui->layoutWidget4->hide();
    ui->lineEdit_2->setEchoMode(QLineEdit::Password);
    ui->lineEdit_3->setEchoMode(QLineEdit::Password);
    ui->lineEdit_4->setEchoMode(QLineEdit::Password);
    ui->label->setStyleSheet("color:white;");
    ui->label_2->setStyleSheet("color:white;");
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
   if(pass!="" && ParaUtil::address!="")
   {
       HttpUtil * http0 = new HttpUtil;
       QString req = "address="+ParaUtil::address+"&pass="+pass;
       //qDebug()<<req;
       connect(http0, SIGNAL(httpFinished(QString)), this, SLOT(loginResult(QString)));
       http0->sendRequest(ParaUtil::url+"login",req,true);
   }
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
        http->sendRequest(ParaUtil::url+"newAccount",req,true);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->layoutWidget1->hide();
    ui->layoutWidget->show();
}
void MainWindow::loginResult(QString str)
{
    int result = JsonUtil::ParseUnlockAccountResult(str);
    if(result !=0 )
    {
        unlockTimer = new QTimer(this);
        connect(unlockTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));
        unlockTimer->start(125000);
        if(result ==1)
        {
            ui->layoutWidget->hide();
            ui->layoutWidget4->setGeometry(QRect(60, 130, 311,100));
            ui->pushButton_3->setEnabled(false);
            ui->pushButton_6->setEnabled(false);
            ui->layoutWidget4->show();
        }
        else if(result==2)
        {
            ui->layoutWidget->hide();
            ui->layoutWidget4->setGeometry(QRect(60, 130, 311,100));
            ui->pushButton_7->setEnabled(false);
             ui->label_8->setText("");
            ui->layoutWidget4->show();
        }
        else if(result==3)
        {
            ui->layoutWidget->hide();
            ui->layoutWidget3->setGeometry(QRect(10, 130, 421,91));
            QList<QHostAddress> list = ParaUtil::get_localmachine_ip();
            QHostAddress ip;
            ui->comboBox->clear();
            foreach (ip, list) {
                ui->comboBox->addItem(ip.toString());
            }
            this->ip = ui->comboBox->currentText();
            ui->layoutWidget3->show();

            ui->label_12->setStyleSheet("color:blue;");
            ui->label_12->setText("开启分享");
            ui->pushButton_11->setEnabled(false);
            ui->pushButton_12->setEnabled(true);
            isShare = true;
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
    int result = JsonUtil::ParseEndServiceResult(str);
    if(result==1)
    {
        ui->label_12->setStyleSheet("color:blue;");
        ui->label_12->setText("结束分享");
        ui->pushButton_12->setEnabled(false);
        ui->pushButton_11->setEnabled(true);
        isShare = false;
    }
    else if(result==0)
    {
        ui->label_12->setStyleSheet("color:red;");
        ui->label_12->setText("结束异常");
        isShare = true;
    }
    else
    {
        ui->label_12->setStyleSheet("color:red;");
        ui->label_12->setText(QString::number(result-1)+"人正在使用");
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
    form->setFixedSize(972,675);
    form->show();
    connect(form, SIGNAL(winClose()), this, SLOT(winAppear()));
    this->hide();
}

void MainWindow::on_pushButton_10_clicked()
{
    //manage share app
    AppManageDialog *dialog = new AppManageDialog;
    connect(dialog, SIGNAL(winClose()), this, SLOT(winAppear()));
    dialog->setFixedSize(730,514);
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
    QString req = "address="+ParaUtil::address+"&mac="+mac+"&ip="+ip+"&moneyPerHour=5";
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(startServiecResult(QString)));
    http->sendRequest(ParaUtil::url+"startService",req,true);
}

void MainWindow::on_pushButton_12_clicked()
{
    //end share
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(endServiecResult(QString)));
    http->sendRequest(ParaUtil::url+"endService",req,true);
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
        ui->layoutWidget2->setGeometry(QRect(60, 130, 311,31));
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
    http->sendRequest(ParaUtil::url+"register",req,true);
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
void MainWindow::unlockResult(QString str)
{
    bool result = JsonUtil::ParseSimpleResult(str);
    if(!result)
    {
        QMessageBox::about(this, "warning", "解锁账户异常");
    }
}
void MainWindow::handleTimeout()
{
    HttpUtil * http = new HttpUtil;
    QString req = "address="+ParaUtil::address;
    //qDebug()<<req;
    connect(http, SIGNAL(httpFinished(QString)), this, SLOT(unlockResult(QString)));
    http->sendRequest(ParaUtil::url+"unlock",req,true);
}
 void MainWindow::paintEvent(QPaintEvent *event)
 {
     Q_UNUSED(event);
     QPainter painter(this);
     //draw background
     QPixmap foreImg;
     foreImg.load("./photo/1.jpg");
     if (!foreImg.isNull())
     {
         painter.setOpacity(0.8);//透明度设置
         painter.drawPixmap(0, 0, foreImg);
      }

     // 设置画笔颜色
     painter.setPen(Qt::white);
     painter.setOpacity(1);
     QFont font;
     font.setFamily("Microsoft YaHei");
     // 大小
     font.setPointSize(20);
     // 设置字符间距
     font.setLetterSpacing(QFont::AbsoluteSpacing, 20);
     // 使用字体
     painter.setFont(font);
     // 获取字体信息
     QFontInfo info = painter.fontInfo();
     info.family();
     info.italic();
     // 绘制区域为当前界面的整个区域（默认-左上角开始）
     painter.drawText(QRect(0,0,446,100), Qt::AlignCenter, QStringLiteral("区块链助力共享"));

 }
