#include "discription.h"
#include "ui_discription.h"

Discription::Discription(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Discription)
{
    ui->setupUi(this);
    //为窗口重命名
    this->setWindowTitle("2048-Coloful");
    //为窗口设置图标
    this->setWindowIcon(QIcon("2048-1.png"));

    //为窗口设置背景颜色（浅蓝色）
    this->setStyleSheet("background-color: rgb(249, 255, 255)");
    //为按键背景设置颜色(浅黄色)
    ui->back_pushButton->setStyleSheet("background-color: rgb(255, 255, 243)");

    //为按键字体设置颜色
    ui->back_pushButton->setStyleSheet("color: rgb(95, 71, 41)");

    //为信息标签设置颜色
    ui->label->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_2->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_3->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_4->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_5->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_6->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_7->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_8->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_9->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_10->setStyleSheet("color: rgb(95, 71, 41)");

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
