#ifndef SERVERITEM_H
#define SERVERITEM_H

#include <QFrame>
#include "userinfo.h"
#include <QImage>
#include "httputil.h"
#include "jsonutil.h"
#include "parautil.h"
#include <QDateTime>
namespace Ui {
class ServerItem;
}

class ServerItem : public QFrame
{
    Q_OBJECT

public:
    explicit ServerItem(UserInfo userInfo,int index,QFrame *parent = 0);
    explicit ServerItem(QFrame *parent = 0);
    ~ServerItem();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ServerItem *ui;
    QImage image;
    UserInfo userInfo;
    int index;
};

#endif // SERVERITEM_H
