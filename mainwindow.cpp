#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


//void MainWindow::on_pushButton_clicked(){}

void MainWindow::on_login_pushButton_clicked()
{
    //隐藏欢迎界面，进入登录界面
    this->hide();
    Login* log = new Login;
    log->show();

}
