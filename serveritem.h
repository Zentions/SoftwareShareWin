#ifndef SERVERITEM_H
#define SERVERITEM_H

#include <QFrame>
#include "userinfo.h"
#include <QImage>
#include "httputil.h"
#include "jsonutil.h"
#include "parautil.h"
#include "startappthread.h"
#include <QDateTime>
#include <QVector>
namespace Ui {
class ServerItem;
}

class ServerItem : public QFrame
{
    Q_OBJECT

signals:
    void connectEnd(QString);
public:
    explicit ServerItem(UserInfo userInfo,int index,QFrame *parent = 0);
    explicit ServerItem(QFrame *parent = 0);
    ~ServerItem();

private slots:
    void on_pushButton_2_clicked();

    void endStoreRecordResult(QString);

    void on_pushButton_clicked();

    void canUseResult(QString);

    void on_pushButton_3_clicked();

private:
    Ui::ServerItem *ui;
    QImage image;
    UserInfo userInfo;
    int index;
    bool use;
    QVector<StartAppThread*> threads;
};

#endif // SERVERITEM_H
