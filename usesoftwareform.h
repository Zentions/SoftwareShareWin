#ifndef USESOFTWAREFORM_H
#define USESOFTWAREFORM_H

#include <QWidget>
#include "serveritem.h"
#include "jsonutil.h"
#include "httputil.h"
#include <QTreeWidgetItem>
#include <QMap>
#include <QDateTime>
#include <QMessageBox>
#include "userinfo.h"
#include "parautil.h"
namespace Ui {
class UseSoftWareForm;
}

class UseSoftWareForm : public QWidget
{
    Q_OBJECT

public:
    explicit UseSoftWareForm(QWidget *parent = 0);
    ~UseSoftWareForm();

private slots:
    void on_pushButton_5_clicked();

    void shareUserResult(QString);

    void on_treeWidget_itemExpanded(QTreeWidgetItem *item);

    void shareUserInfoResultForTreeWidget(QString);

    void shareUserInfoResultForConnect(QString);

    void connectServerResult(QString);

    void on_pushButton_4_clicked();

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();
signals:
    void winClose();
protected:
    void closeEvent(QCloseEvent *);

private:
    void queryShareUser(bool byName,QString name);
    void queryShareUserInfo(QString address);
    Ui::UseSoftWareForm *ui;
    QMap<QString,QTreeWidgetItem*> AddressMap;
    QMap<QString,ServerItem*> ConnectUsers;
    QMap<QString,UserInfo> PendUsers;
    QVector<QString> ConnectAddress;
    int current;
};

#endif // USESOFTWAREFORM_H
