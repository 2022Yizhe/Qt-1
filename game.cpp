#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    //为窗口重命名
    this->setWindowTitle("2048-Coloful");
    //为窗口设置图标
    this->setWindowIcon(QIcon("2048-1.png"));

    //为窗口背景设置颜色（浅蓝色）
    this->setStyleSheet("background-color: rgb(249, 255, 255)");
    //为按键背景设置颜色(浅黄色) -- 无效
    ui->Again_pushButton->setStyleSheet("background-color: rgb(255, 253, 246)");
    ui->back_pushButton->setStyleSheet("background-color: rgb(255, 253, 246)");

    //为按键字体设置颜色
    ui->Again_pushButton->setStyleSheet("color: rgb(95, 71, 41)");
    ui->back_pushButton->setStyleSheet("color: rgb(95, 71, 41)");
    ui->remove_pushButton->setStyleSheet("color: rgb(95, 71, 41)");

    //为信息标签设置颜色
    ui->label_1->setStyleSheet("color: rgb(95, 71, 41)");
    ui->label_2->setStyleSheet("color: rgb(95, 71, 41)");
    ui->Best_label->setStyleSheet("color: rgb(95, 71, 41)");
    ui->Score_label->setStyleSheet("color: rgb(111, 67, 255)");
    ui->BestScore_label->setStyleSheet("color: rgb(211, 82, 8)");
    ui->EndGame_label->setStyleSheet("color: rgb(255, 0, 0)");

    //ui->One_lineEdit->setStyleSheet("color: rgb(0,0,0));

    //初始化分数为 0
    ui->Score_label->setText("0");
    //初始化步数为 0
    ui->Step_label ->setText("0");
    //初始化时间为当前时间
    ui->Time_label->setText("00:00:00");
    //隐藏“游戏结束”
    ui->EndGame_label->hide();

    //初始化最好成绩为 (.txt)
    QFile file("record.txt");
    if(!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug() << "Current working directory:" << QDir::currentPath();
        qDebug()<<"打开文档失败!";
        return;
    }
    QTextStream in(&file);
    QString line;
    while(!in.atEnd()){
        line = in.readLine();
        qDebug() << line;
    }
    ui->BestScore_label->setText(line);
    file.close();
}

Game::~Game()
{
    delete ui;
}

void Game::on_pushButton_clicked(){ return; }
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
    myLayout* newmylayout = new myLayout;
    newmylayout->show();
}
void Game::on_remove_pushButton_clicked()
{
    //执行悔棋，需重写虚拟函数
    reMove();
}


//====================================================================================================//
//====================================================================================================//

myLayout::myLayout(): array(size, QVector<int>(size)){
//连接信号与槽(谁发出信号、发出什么信号、谁处理信号、怎么处理)，通常写在类构造函数中!
    //按键监测
    connect(this, &myLayout::keyPressed, this, &myLayout::onKeyPressed);
    connect(this, &myLayout::keyReleased, this, &myLayout::onKeyReleased);

    //初始化分数、最佳分数、步数
    score = 0;
    Step = 0;
    BestScore = ui->BestScore_label->text().toInt();

    //初始化 saveMove 游戏局势
    saveMove();

    // 开始计时
    timer.start();

    // 每隔一秒钟更新标签的文本(lambda表达式)
    connect(updatedtimer, &QTimer::timeout, [=]() {
        int elapsed = timer.elapsed() / 1000;
        int hours = elapsed / 3600;
        int minutes = (elapsed % 3600) / 60;
        int seconds = elapsed % 60;
        ui->Time_label->setText(QString("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0')));
    });
    // 启动定时器，每隔一秒钟触发一次 timeout 信号
    updatedtimer->start(1000);

    //初始化array为0
    for(int i=0;i<size;++i)
        for(int j=0;j<size;++j)
            array[i][j]=0;

    //梅森旋转法生成伪随机数，以时间为种子，产生gen()序列
    std::mt19937 gen(time(nullptr));
    //产生一个 0 ~ size-1 的随机数分布
    std::uniform_int_distribution<> dis(0, size * size - 1);
    //dis()调用 operator() 并使用参数 gen 来生成一个随机数, 赋值给pos
    int pos = dis(gen);
    //在随机位置放置 2
    array[pos / size][pos % size] = 2;

    //初始化数框
    showArray();
}

myLayout::~myLayout(){}

int myLayout::size = 4;

void myLayout::onKeyPressed(int key) {

    // 按下Esc, 游戏退出
    if (key == Qt::Key_Escape)
        this->close();
    //按键WASD，游戏控制
    bool moved = false;
    if (key ==Qt::Key_W)
        moved  = this->moveDown();
    if (key ==Qt::Key_A)
        moved  = this->moveLeft();
    if(key ==Qt::Key_S)
        moved  = this->moveUp();
    if(key ==Qt::Key_D)
        moved  = this->moveRight();

    if(moved){
        //先保证
        saveMove();
        //再更新当前步数
        Step++;

        this->addNumber();
        this->showArray();
    }
    //在一次运行终点之前为游戏是否结束作判断
    judgeGame();

}
void myLayout::onKeyReleased(int key) {
    if (key == Qt::Key_Escape) {
        // 释放Esc键时执行的操作
    }
}

void myLayout::rotateArray(){
    //重新创建二维数组 newArray（size * size）
    QVector<QVector<int>> newarray = array;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            newarray[j][size - i - 1] =array[i][j];
        }
    }
    array = newarray;
}

bool myLayout::addNumber() {
    vector<int> emptyPos;
    for (int i = 0; i < size * size; ++i) {
        //遍历16个格子数字，记录每个“0”格子的位置
        if (array[i / size][i % size] == 0) {
            emptyPos.push_back(i);
        }
    }
    if (emptyPos.empty())
        return false;

    //梅森旋转法生成伪随机数，以时间为种子，产生gen()序列
    std::mt19937 gen(time(nullptr));
    //在剩余的零格子随机挑选一个格子
    std::uniform_int_distribution<> dis(0, emptyPos.size() - 1);
    //获取被挑到的格子的位置
    int pos = emptyPos[dis(gen)];
    //通过格子的位置，反向取址，单走一个 2
    array[pos / size][pos % size] = 2;
    return true;
}

/* 左移, 合并同类项。可以举例：4 0 2 2 1 */
bool myLayout::moveLeft() {
    bool moved = false;
    for (auto& row : array) {
        //检查“空隙0”
        for (int i = 0; i < size - 1; i++) {
            if (row[i] == 0 && row[i] < row[i + 1])
                moved = true;
        }
        //开始左移
        //remove()将数组(row)的指定数字(0)全部移除，非零数组成新序列，并将新序列指向末尾元素的指针(迭代器)返回给it
        auto it = remove(row.begin(), row.end(), 0);
        if (moved == true)
            fill(it, row.end(), 0);
        //->4 2 2 1 0

        //检查相邻相同数，左数*2，右数置0
        for (int i = 0; i < size - 1; ++i) {
            if (row[i] == row[i + 1] && row[i] != 0) {
                row[i] *= 2;
                //实时更新当局积分
                score += row[i];
                ui->Score_label->setText(QString::number(score));

                row[i + 1] = 0;
                moved = true;
            }
        }
        //->4 4 0 1 0

        //再次检查“空隙0”
        for (int i = 0; i < size - 1; i++) {
            if (row[i] == 0 && row[i] < row[i + 1])
                moved = true;
        }
        //再次左移
        //remove()再次将迭代器(row)的指定数字(0)全部移除，非零数组成新序列，将新序列指向末尾元素的指针(迭代器)返回给it
        it = remove(row.begin(), row.end(), 0);
        if (moved == true)
            fill(it, row.end(), 0);
        //->4 4 1 0 0

    }
    return moved;
}

bool myLayout::moveDown() {
    rotateArray();
    /*
       |4  0  1 |
       |0  3  0 |
       |5  0  2 |
    */
    bool moved = moveLeft();
    /*
       |4  1  0 |
       |3  0  0 |
       |5  2  0 |
    */
    rotateArray();
    /*
       |5  3  4 |
       |2  0  1 |
       |0  0  0 |
    */
    rotateArray();
     /*
         |0  2  5 |
         |0  0  3 |
         |0  1  4 |
      */
    rotateArray();
    /*
        |0  0  0 |
        |1  0  2 |
        |4  3  5 |
    */
    return moved;
}

bool myLayout::moveRight() {
    rotateArray();
    rotateArray();
    bool moved = moveLeft();
    rotateArray();
    rotateArray();
    return moved;
}

bool myLayout::moveUp() {
    rotateArray();
    rotateArray();
    rotateArray();
    bool moved = moveLeft();
    rotateArray();
    return moved;
}

void myLayout::showArray(){

    ui->One_lineEdit->setText(QString::number(array[0][0]));
    ui->Two_lineEdit->setText(QString::number(array[0][1]));
    ui->Three_lineEdit->setText(QString::number(array[0][2]));
    ui->Four_lineEdit->setText(QString::number(array[0][3]));

    ui->Five_lineEdit->setText(QString::number(array[1][0]));
    ui->Six_lineEdit->setText(QString::number(array[1][1]));
    ui->Seven_lineEdit->setText(QString::number(array[1][2]));
    ui->Eight_lineEdit->setText(QString::number(array[1][3]));

    ui->Nine_lineEdit->setText(QString::number(array[2][0]));
    ui->Ten_lineEdit->setText(QString::number(array[2][1]));
    ui->Eleven_lineEdit->setText(QString::number(array[2][2]));
    ui->Twelve_lineEdit->setText(QString::number(array[2][3]));

    ui->Thirteen_lineEdit->setText(QString::number(array[3][0]));
    ui->Fourteen_lineEdit->setText(QString::number(array[3][1]));
    ui->Fifteen_lineEdit->setText(QString::number(array[3][2]));
    ui->Sixthteen_lineEdit->setText(QString::number(array[3][3]));

    //实时显示步数
    ui->Step_label->setText(QString::number(Step));

    //实时更新颜色
    showColor();
}

void myLayout::color(QLineEdit* le){
    //根据方框内容，改变数字颜色
    if(le->text().toUInt() == 0)
        le->setStyleSheet("color: rgb(0,0,0)"); // 0 设置为黑色
    else if(le->text().toInt() == 2)
        le->setStyleSheet("color: rgb(100,150,100)"); // 2 设置为暗绿色
    else if(le->text().toInt() == 4)
        le->setStyleSheet("color: rgb(100,200,200)"); // 4 设置为蓝绿色
    else if(le->text().toInt() == 8)
        le->setStyleSheet("color: rgb(20,150,255)"); // 8 设置为蓝色
    else if(le->text().toInt() == 16)
        le->setStyleSheet("color: rgb(100,255,20)"); // 16 设置为亮绿色
    else if(le->text().toInt() == 32)
        le->setStyleSheet("color: rgb(0,100,255)"); // 32 设置为深蓝色
    else if(le->text().toInt() == 64)
        le->setStyleSheet("color: rgb(255,200,0)"); // 64 设置为黄色
    else if(le->text().toInt() == 128)
        le->setStyleSheet("color: rgb(255,50,50)"); // 128 设置为亮红色
    else if(le->text().toInt() == 256)
        le->setStyleSheet("color: rgb(255,25,255)"); // 256 设置为红紫色
    else if(le->text().toInt() == 512)
        le->setStyleSheet("color: rgb(150,1,1)"); // 512 设置为棕红色
    else if(le->text().toInt() == 1024)
        le->setStyleSheet("color: rgb(150,20,250)"); // 1028 设置为紫色
    else if(le->text().toInt() == 2048)
        le->setStyleSheet("color: rgb(250,140,50)"); // 2056 设置为橙色
    else if(le->text().toInt() == 5096)
        le->setStyleSheet("color: rgb(250,100,100)"); // 5096 设置为橙红色
    else if(le->text().toInt() == 10192)
        le->setStyleSheet("color: rgb(250,0,0)"); // 10192 设置为红色


}

void myLayout::showColor(){
    //改变所有方块的颜色
    color(ui->One_lineEdit);
    color(ui->Two_lineEdit);
    color(ui->Three_lineEdit);
    color(ui->Four_lineEdit);
    color(ui->Five_lineEdit);

    color(ui->Six_lineEdit);
    color(ui->Seven_lineEdit);
    color(ui->Eight_lineEdit);
    color(ui->Nine_lineEdit);
    color(ui->Ten_lineEdit);

    color(ui->Eleven_lineEdit);
    color(ui->Twelve_lineEdit);
    color(ui->Thirteen_lineEdit);
    color(ui->Fourteen_lineEdit);
    color(ui->Fifteen_lineEdit);

    color(ui->Sixthteen_lineEdit);
}

void myLayout::judgeGame(){
    if(
       (
                // 相邻格不相等
        ui->One_lineEdit->text().toInt()!=ui->Two_lineEdit->text().toInt()&&
        ui->Two_lineEdit->text().toInt()!=ui->Three_lineEdit->text().toInt()&&
        ui->Three_lineEdit->text().toInt()!=ui->Four_lineEdit->text().toInt()&&
        ui->Five_lineEdit->text().toInt()!=ui->Six_lineEdit->text().toInt()&&
        ui->Six_lineEdit->text().toInt()!=ui->Seven_lineEdit->text().toInt()&&
        ui->Seven_lineEdit->text().toInt()!=ui->Eight_lineEdit->text().toInt()&&
        ui->Nine_lineEdit->text().toInt()!=ui->Ten_lineEdit->text().toInt()&&
        ui->Ten_lineEdit->text().toInt()!=ui->Eleven_lineEdit->text().toInt()&&
        ui->Eleven_lineEdit->text().toInt()!=ui->Twelve_lineEdit->text().toInt()&&
        ui->Thirteen_lineEdit->text().toInt()!=ui->Fourteen_lineEdit->text().toInt()&&
        ui->Fourteen_lineEdit->text().toInt()!=ui->Fifteen_lineEdit->text().toInt()&&
        ui->Fifteen_lineEdit->text().toInt()!=ui->Sixthteen_lineEdit->text().toInt()
        )&&(
        ui->One_lineEdit->text().toInt()!=ui->Five_lineEdit->text().toInt()&&
        ui->Two_lineEdit->text().toInt()!=ui->Six_lineEdit->text().toInt()&&
        ui->Three_lineEdit->text().toInt()!=ui->Seven_lineEdit->text().toInt()&&
        ui->Four_lineEdit->text().toInt()!=ui->Eight_lineEdit->text().toInt()&&
        ui->Five_lineEdit->text().toInt()!=ui->Nine_lineEdit->text().toInt()&&
        ui->Six_lineEdit->text().toInt()!=ui->Ten_lineEdit->text().toInt()&&
        ui->Seven_lineEdit->text().toInt()!=ui->Eleven_lineEdit->text().toInt()&&
        ui->Eight_lineEdit->text().toInt()!=ui->Twelve_lineEdit->text().toInt()&&
        ui->Nine_lineEdit->text().toInt()!=ui->Thirteen_lineEdit->text().toInt()&&
        ui->Ten_lineEdit->text().toInt()!=ui->Fourteen_lineEdit->text().toInt()&&
        ui->Eleven_lineEdit->text().toInt()!=ui->Fifteen_lineEdit->text().toInt()&&
        ui->Twelve_lineEdit->text().toInt()!=ui->Sixthteen_lineEdit->text().toInt()
        )&&(
                // 没有 “0” 格
        ui->One_lineEdit->text().toInt()!=0&&
        ui->Two_lineEdit->text().toInt()!=0&&
        ui->Three_lineEdit->text().toInt()!=0&&
        ui->Four_lineEdit->text().toInt()!=0&&
        ui->Five_lineEdit->text().toInt()!=0&&
        ui->Six_lineEdit->text().toInt()!=0&&
        ui->Seven_lineEdit->text().toInt()!=0&&
        ui->Eight_lineEdit->text().toInt()!=0&&
        ui->Nine_lineEdit->text().toInt()!=0&&
        ui->Ten_lineEdit->text().toInt()!=0&&
        ui->Eleven_lineEdit->text().toInt()!=0&&
        ui->Twelve_lineEdit->text().toInt()!=0&&
        ui->Thirteen_lineEdit->text().toInt()!=0&&
        ui->Fourteen_lineEdit->text().toInt()!=0&&
        ui->Fifteen_lineEdit->text().toInt()!=0&&
        ui->Sixthteen_lineEdit->text().toInt()!=0
        )
         )
    {
        ui->EndGame_label->show();
        //计时结束
        updatedtimer->stop();

        // 检查并保存 Bestscore
        if(score>BestScore){
            //打印新的记录
            BestScore = score;
            ui->BestScore_label->setText(QString::number(BestScore));
            //保存新的记录
            QFile file("record.txt");
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){     //覆盖原来文本
                qDebug() <<"当前工作目录"<<QDir::currentPath();
                qDebug() <<"打开文档失败!";
                return ;
            }
            QTextStream out(&file);
            out << QString::number(BestScore);
            file.close();
        }
    }
}

void myLayout::saveMove(){
    //保存棋局
    QFile file("situation.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){     //覆盖原来文本
        qDebug() <<"当前工作目录"<<QDir::currentPath();
        qDebug() <<"打开文档失败!";
        return ;
    }
    //按字符串保存, 以回车字符串分隔
    QTextStream out(&file);
    out << ui->One_lineEdit->text()<<'\n'
        << ui->Two_lineEdit->text()<<'\n'
        << ui->Three_lineEdit->text()<<'\n'
        << ui->Four_lineEdit->text()<<'\n'
        << ui->Five_lineEdit->text()<<'\n'

        << ui->Six_lineEdit->text()<<'\n'
        << ui->Seven_lineEdit->text()<<'\n'
        << ui->Eight_lineEdit->text()<<'\n'
        << ui->Nine_lineEdit->text()<<'\n'
        << ui->Ten_lineEdit->text()<<'\n'

        << ui->Eleven_lineEdit->text()<<'\n'
        << ui->Twelve_lineEdit->text()<<'\n'
        << ui->Thirteen_lineEdit->text()<<'\n'
        << ui->Fourteen_lineEdit->text()<<'\n'
        << ui->Fifteen_lineEdit->text()<<'\n'

        << ui->Sixthteen_lineEdit->text()<<'\n';

    file.close();
}

void myLayout::reMove(){
    //写入文档数据
    QFile file("situation.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "当前工作目录"<<QDir::currentPath();
        qDebug() <<"打开文档失败!";
        return ;
    }
    QTextStream in(&file);
    //创建二维string数组
    QStringList lines;
    //循环追加每一行文本至 lines
    while(!in.atEnd()){
        //单行文本用 line 临时储存
        QString line = in.readLine();
        lines.append(line);
    }
    for(int i =0;i<lines.size();i++){
        QString line =lines[i];
        //依次修改数字框文本
        switch (i) {
           case 0:
               ui->One_lineEdit->setText(line);
               break;
           case 1:
               ui->Two_lineEdit->setText(line);
               break;
           case 2:
               ui->Three_lineEdit->setText(line);
               break;
           case 3:
               ui->Four_lineEdit->setText(line);
               break;
           case 4:
               ui->Five_lineEdit->setText(line);
               break;

           case 5:
               ui->Six_lineEdit->setText(line);
               break;
           case 6:
               ui->Seven_lineEdit->setText(line);
               break;
           case 7:
               ui->Eight_lineEdit->setText(line);
               break;
           case 8:
               ui->Nine_lineEdit->setText(line);
               break;
           case 9:
               ui->Ten_lineEdit->setText(line);
               break;

           case 10:
               ui->Eleven_lineEdit->setText(line);
               break;
           case 11:
               ui->Twelve_lineEdit->setText(line);
               break;
           case 12:
               ui->Thirteen_lineEdit->setText(line);
               break;
           case 13:
               ui->Fourteen_lineEdit->setText(line);
               break;
           case 14:
               ui->Fifteen_lineEdit->setText(line);
               break;

           case 15:
               ui->Sixthteen_lineEdit->setText(line);
               break;
           default:
               break;
        }
    }

    //数字改变，数字颜色也改变
    showColor();
    //数字改变，array也改变(双变化!)
    for (int i = 0;i < size; i++)
        for(int j = 0;j < size; j++)
            switch (size*i+j) {
                case 0:
                    array[i][j] = ui->One_lineEdit->text().toInt();
                    break;
                case 1:
                    array[i][j] = ui->Two_lineEdit->text().toInt();
                    break;
                case 2:
                    array[i][j] = ui->Three_lineEdit->text().toInt();
                    break;
                case 3:
                    array[i][j] = ui->Four_lineEdit->text().toInt();
                    break;
                case 4:
                    array[i][j] = ui->Five_lineEdit->text().toInt();
                    break;
                case 5:
                    array[i][j] = ui->Six_lineEdit->text().toInt();
                    break;
                case 6:
                    array[i][j] = ui->Seven_lineEdit->text().toInt();
                    break;
                case 7:
                    array[i][j] = ui->Eight_lineEdit->text().toInt();
                    break;
                case 8:
                    array[i][j] = ui->Nine_lineEdit->text().toInt();
                    break;
                case 9:
                    array[i][j] = ui->Ten_lineEdit->text().toInt();
                    break;
                case 10:
                    array[i][j] = ui->Eleven_lineEdit->text().toInt();
                    break;
                case 11:
                    array[i][j] = ui->Twelve_lineEdit->text().toInt();
                    break;
                case 12:
                    array[i][j] = ui->Thirteen_lineEdit->text().toInt();
                    break;
                case 13:
                    array[i][j] = ui->Fourteen_lineEdit->text().toInt();
                    break;
                case 14:
                    array[i][j] = ui->Fifteen_lineEdit->text().toInt();
                    break;
                case 15:
                    array[i][j] = ui->Sixthteen_lineEdit->text().toInt();
                    break;
            }

    qDebug()<<"已执行悔棋！";
}
