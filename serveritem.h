#ifndef SERVERITEM_H
#define SERVERITEM_H

#include <QFrame>
#include "userinfo.h"
#include <QImage>
#include <QTimer>
#include "httputil.h"
#include "jsonutil.h"
#include "parautil.h"
#include "markdialog.h"
#include "startappthread.h"
#include <QDateTime>
#include <QVector>
#include <QSet>
#include <QPainter>
using namespace std;
namespace Ui {
class ServerItem;
}

class ServerItem : public QFrame
{
    Q_OBJECT


public:
    explicit ServerItem(UserInfo userInfo,int index,QFrame *parent = 0);
    explicit ServerItem(QFrame *parent = 0);
    void endUse(bool);
    void closeAllSoftware();
    ~ServerItem();

private slots:
    void on_pushButton_2_clicked();

    void endStoreRecordResult(QString);

    void on_pushButton_clicked();

    void canUseResult(QString);

    void on_pushButton_3_clicked();

    void handleTimeout();

    void continueToPayResult(QString);

    void connectServerResult(QString);

protected:
    void paintEvent(QPaintEvent *);

public:
    void init(QString);
    bool isEnd();
    void FirstStoreRecord();
private:
    Ui::ServerItem *ui;
    QImage image;
    UserInfo userInfo;
    int index;
    bool use;
    QTimer* m_pTimer;
    int time;
    QSet<int> opened;
};

#endif // SERVERITEM_H
