#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //为窗口重命名
    this->setWindowTitle("2048-Coloful");
    //为窗口设置图标
    this->setWindowIcon(QIcon("2048-1.png"));

    //为窗口设置背景颜色（浅蓝色）
    this->setStyleSheet("background-color: rgb(249, 255, 255)");
    //为按键设置颜色(浅黄色)
    ui->TryLogin_pushButton->setStyleSheet("background-color: rgb(255, 255, 243)");

    //为信息标签设置颜色
    ui->label->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_2->setStyleSheet("color: rgb(95, 71, 41)");
    ui->Wrong_label->setStyleSheet("color: rgb(255, 0, 0)");


}

Login::~Login()
{
    delete ui;
}

void Login::on_TryLogin_pushButton_clicked()
{
    //获取账号密码
    QString ACC = ui->Account_lineEdit->text();
    QString PASS = ui->Password_lineEdit->text();

    //比较账号密码
    if(QString::compare("",ACC)==0){
          ui->Wrong_label ->setText("请输入账号!");
          return;
    }
    if(QString::compare("",PASS)==0){
          ui->Wrong_label ->setText("请输入密码!");
          return;
    }
    if(QString::compare(prev_acc,ACC)!=0||QString::compare(prev_pass,PASS)!=0){
        ui->Wrong_label ->setText("账号或者密码错误!请重新输入");
    }else{
        //隐藏登录界面，进入菜单界面
        ui->Wrong_label ->hide();
        this->hide();
        Order* ord = new Order;
        ord ->show();
    }
}
