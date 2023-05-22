#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //为窗口重命名
    this->setWindowTitle("2048-Coloful");
    //为窗口设置图标
    this->setWindowIcon(QIcon("2048-1.png"));

    //为窗口设置背景颜色（浅蓝色）
    this->setStyleSheet("background-color: rgb(249, 255, 255)");
    //为按键设置颜色(浅黄色)
    ui->login_pushButton->setStyleSheet("background-color: rgb(255, 255, 243)");

    //为信息标签设置颜色
    ui->label->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_2->setStyleSheet("color: rgb(95, 71, 41)");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_login_pushButton_clicked()
{
    //隐藏欢迎界面，进入登录界面
    this->hide();
    Login* log = new Login;
    log->show();
}
