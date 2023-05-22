#include "order.h"
#include "ui_order.h"

Order::Order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);
    //为窗口重命名
    this->setWindowTitle("2048-Coloful");
    //为窗口设置图标
    this->setWindowIcon(QIcon("2048-1.png"));

    //为窗口设置背景颜色（浅蓝色）
    this->setStyleSheet("background-color: rgb(249, 255, 255)");
    //为按键设置颜色(浅黄色)
    ui->StartGame_pushButton->setStyleSheet("background-color: rgb(255, 255, 243)");
    ui->ExplainGame_pushButton->setStyleSheet("background-color: rgb(255, 255, 243)");

    //为信息标签设置颜色(棕色)
    ui->label->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_2->setStyleSheet("color: rgb(95, 71, 41)");

}

Order::~Order()
{
    delete ui;
}

void Order::on_StartGame_pushButton_clicked()
{
    //隐藏菜单界面，进入游戏界面
    this->hide();

    myLayout* mylayout = new myLayout;
    mylayout->show();

}

void Order::on_ExplainGame_pushButton_clicked()
{
    //隐藏菜单界面，进入说明界面
    this->hide();
    Discription* discription = new Discription;
    discription->show();
}
