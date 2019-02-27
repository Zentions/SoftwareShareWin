#ifndef MARKITEM_H
#define MARKITEM_H

#include <QFrame>
#include <QPainter>
namespace Ui {
class MarkItem;
}

class MarkItem : public QFrame
{
    Q_OBJECT

public:
    explicit MarkItem(QString name,QString grade,QFrame *parent = 0);
    ~MarkItem();

    int getValue();

protected:
    void paintEvent(QPaintEvent *);

private slots:
    void on_spinBox_valueChanged(int arg1);

private:
    Ui::MarkItem *ui;
    int value;
};

#endif // MARKITEM_H
