#ifndef DISCRIPTION_H
#define DISCRIPTION_H

#include <QWidget>
//返回指令需包含类 Order
#include "order.h"

namespace Ui {
class Discription;
}

class Discription : public QWidget
{
    Q_OBJECT

public:
    explicit Discription(QWidget *parent = nullptr);
    ~Discription();

private slots:
    void on_back_pushButton_clicked();

private:
    Ui::Discription *ui;
};

#endif // DISCRIPTION_H
