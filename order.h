#ifndef ORDER_H
#define ORDER_H

#include <QWidget>
//包含类 Game、Dsscription
#include "game.h"
#include "discription.h"

namespace Ui {
class Order;
}

class Order : public QWidget
{
    Q_OBJECT

public:
    explicit Order(QWidget *parent = nullptr);
    ~Order();

private slots:
    void on_StartGame_pushButton_clicked();

    void on_ExplainGame_pushButton_clicked();

private:
    Ui::Order *ui;
};

#endif // ORDER_H
