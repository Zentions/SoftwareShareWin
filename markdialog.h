#ifndef MARKDIALOG_H
#define MARKDIALOG_H

#include <QDialog>
#include <QSet>
#include <QMap>
#include <QList>
#include "markitem.h"
#include "userinfo.h"
#include "httputil.h"
#include "jsonutil.h"
#include "parautil.h"
namespace Ui {
class MarkDialog;
}

class MarkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MarkDialog(UserInfo user,QSet<int> indexs,QWidget *parent = 0);
    ~MarkDialog();

private slots:
    void on_pushButton_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void undateScoreResult(QString);

private:
    Ui::MarkDialog *ui;
    QImage leftImage;
    QImage rightImage;
    int current;
    int total;
    UserInfo user;
    QMap<int,MarkItem*> items;
    QList<int> list;
};

#endif // MARKDIALOG_H
