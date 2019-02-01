#ifndef STARTAPPTHREAD_H
#define STARTAPPTHREAD_H

#include <QObject>
#include <QThread>

class StartAppThread : public QThread
{
    Q_OBJECT
public:
    explicit StartAppThread(QString file,QObject *parent = nullptr);

signals:

public slots:

protected:
 void run();
private:
 QString fileName;

};


#endif // STARTAPPTHREAD_H
