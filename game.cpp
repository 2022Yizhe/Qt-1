#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    //连接信号与槽(谁发出信号、发出什么信号、谁处理信号、怎么处理)，通常写在类构造函数中!

}

Game::~Game()
{
    delete ui;
}

void Game::on_back_pushButton_clicked()
{
    //退出游戏界面，进入菜单界面，不保存正在进行的游戏数据
    this->deleteLater();
    Order* backO = new Order;
    backO->show();
}

void Game::on_Again_pushButton_clicked()
{
    //重新开始游戏,不保存正在进行的游戏数据
    this->deleteLater();
    Game* newGame= new Game;
    newGame->show();
}

QString Game::KeyPressEvent(QKeyEvent* key){
    //创建数字框数组，并初始化
    int array[16]={0};
    for (int i = 0; i<16 ;i++){
        QString str =ui->One_lineEdit->text();
        array[i] = str.toInt();
    }


    //用字符串记录键盘操作,返回操作符
    QString opers;
    if (key->key()==Qt::Key_W)
        opers += "w";
    if (key->key()==Qt::Key_A)
        opers += "a";
    if(key->key()==Qt::Key_S)
        opers += "s";
    if(key->key()==Qt::Key_D)
        opers += "d";

    return opers;
}
