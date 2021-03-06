#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QNetworkInterface>
#include "httputil.h"
#include "jsonutil.h"
#include "appmanagedialog.h"
#include "usesoftwareform.h"
#include "parautil.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void loginResult(QString);

    void newAccountResult(QString);

    void startServiecResult(QString);

    void registerResult(QString);

    void unlockResult(QString);

    void endServiecResult(QString);

    void on_pushButton_6_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void winAppear();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_7_clicked();

    void handleTimeout();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;
    QString ip;
    bool isShare;
    QTimer* unlockTimer;
};

#endif // MAINWINDOW_H
