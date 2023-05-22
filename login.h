#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
//包含类 Order (向下包含原则)
#include "order.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_TryLogin_pushButton_clicked();

private:
    Ui::Login *ui;

    //储存账号密码
    QString prev_acc = "LIU";
    QString prev_pass= "abc";

};

#endif // LOGIN_H
