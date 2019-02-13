#ifndef SERVERITEM_H
#define SERVERITEM_H

#include <QFrame>

namespace Ui {
class ServerItem;
}

class ServerItem : public QFrame
{
    Q_OBJECT

public:
    explicit ServerItem(QFrame *parent = 0);
    ~ServerItem();

private:
    Ui::ServerItem *ui;
};

#endif // SERVERITEM_H
