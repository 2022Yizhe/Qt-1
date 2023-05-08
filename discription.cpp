#include "discription.h"
#include "ui_discription.h"

Discription::Discription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Discription)
{
    ui->setupUi(this);
}

Discription::~Discription()
{
    delete ui;
}

void Discription::on_back_pushButton_clicked()
{
    //退出当前窗口，进入菜单窗口
    this->deleteLater();
    Order* backO = new Order;
    backO->show();
}
