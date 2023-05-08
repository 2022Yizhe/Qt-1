#include "order.h"
#include "ui_order.h"

Order::Order(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Order)
{
    ui->setupUi(this);
}

Order::~Order()
{
    delete ui;
}

void Order::on_StartGame_pushButton_clicked()
{
    //隐藏菜单界面，进入游戏界面
    this->hide();
    Game* game = new Game;
    game->show();

}

void Order::on_ExplainGame_pushButton_clicked()
{
    //隐藏菜单界面，进入说明界面
    this->hide();
    Discription* discription = new Discription;
    discription->show();
}
