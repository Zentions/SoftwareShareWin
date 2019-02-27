#include "markitem.h"
#include "ui_markitem.h"

MarkItem::MarkItem(QString name,QString grade,QFrame *parent) :
    QFrame(parent),
    ui(new Ui::MarkItem)
{
    ui->setupUi(this);
    ui->spinBox->setMaximum(100);
    ui->spinBox->setValue(-1);
    ui->label->setText(name+":");
    ui->label_3->setText(grade);
}

MarkItem::~MarkItem()
{
    delete ui;
}

void MarkItem::on_spinBox_valueChanged(int arg1)
{
    if(arg1<0) ui->spinBox->setValue(0);
    if(arg1>100) ui->spinBox->setValue(100);
}
int MarkItem::getValue()
{
    return ui->spinBox->value();
}
void MarkItem::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 反锯齿;
    QPen pen;
    pen.setWidth(3);
    pen.setColor(QColor("#FF00FF"));
    painter.setPen(pen);
    QRect rect = this->rect();
    rect.setWidth(rect.width()-1);
    rect.setHeight(42);
    painter.drawRoundedRect(rect, 15, 15);
}
