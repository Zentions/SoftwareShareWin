#include "serveritem.h"
#include "ui_serveritem.h"

ServerItem::ServerItem(QFrame *parent) :
    QFrame(parent),
    ui(new Ui::ServerItem)
{
    ui->setupUi(this);
    this->setStyleSheet("ServerItem{border: 2px solid #FF00FF; border-radius: 5px;};");

}

ServerItem::~ServerItem()
{
    delete ui;
}

