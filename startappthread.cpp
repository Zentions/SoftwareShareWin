#include "startappthread.h"

StartAppThread::StartAppThread(QString file,QObject *parent) : QThread(parent)
{
     fileName = file;
}

void StartAppThread::run()
{
   QString cmd = "expect "+fileName;
   char*  ch;
   QByteArray ba = cmd.toLatin1(); // must
   ch=ba.data();
   system(ch);
}
