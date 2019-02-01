#ifndef APPMANAGEDIALOG_H
#define APPMANAGEDIALOG_H
#include <QString>
#include <QFile>
#include <QDebug>
#include <QVariantList>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QProcess>
#include "mainwindow.h"
#include "httputil.h"
#include "jsonutil.h"
#include "startappthread.h"

namespace Ui {
class AppManageDialog;
}

class AppManageDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AppManageDialog(QWidget *parent = 0);
    ~AppManageDialog();
    void queryApplication();

private:
    void updateApp();

    void writeFile(QString fileName,QString name,QString pass,QString app);

private slots:
    void on_addButton_clicked();

    void on_delButton_clicked();

    void on_addButton_2_clicked();

    void getSoftwareResult(QString);

    void storeSoftwareResult(QString);

    void delSoftWareResult(QString);

    void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);

signals:
    void winClose();

protected:
    void closeEvent(QCloseEvent *);

private:
    Ui::AppManageDialog *ui;
    bool isSuccess;
};

#endif // APPMANAGEDIALOG_H
