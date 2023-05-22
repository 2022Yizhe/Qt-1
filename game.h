#ifndef GAME_H
#define GAME_H

#include <QWidget>
#include "order.h"      //返回指令需包含类 Order
#include <QKeyEvent>    //键盘事件类
#include <QtCore>       //数组类
#include <QLineEdit>    //LineEdit信息类
#include <QFile>
#include <QTextStream>
#include <QDebug>
using namespace std;

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
public:
    virtual void reMove() = 0;
//槽函数(自动连接)
private slots:
    void on_pushButton_clicked();
    void on_back_pushButton_clicked();
    void on_Again_pushButton_clicked();

    void on_remove_pushButton_clicked();

protected:
    Ui::Game *ui;
};

//====================================================================================================//
//====================================================================================================//

class myLayout:public Game
{   // 宏，在 Qt 中用于启用类的元对象特性。必须出现在类定义的私有部分中，且只能用于继承自 QObject 的类
    Q_OBJECT

public:
    static int size;
    int score;
    int Step;

    int BestScore;

    QTimer *updatedtimer = new QTimer(this);
    QElapsedTimer timer;

    QVector<QVector<int>> array;

public:
    myLayout();
    /* 完成移动时的数字变化 */
    bool moveLeft();
    /* 采用翻转 */
    bool moveUp();
    bool moveDown();
    bool moveRight();
    /* 数字匹配颜色 */
    void color(QLineEdit* le);
    /* 翻转数组 */
    void rotateArray();
    /* 在空余的格子上生成一个2 */
    bool addNumber();
    /* array改变，数字框也改变 */
    void showArray();
    /* 数字框改变，数字框颜色也改变*/
    void showColor();
    /* 判断游戏是否结束 */
    void judgeGame();
    /* 保存棋盘 */
    void saveMove();
    /* 悔棋 */
    void reMove();
    ~myLayout();

signals:
    //键盘信号
    void keyPressed(int key);
    void keyReleased(int key);

protected:
    void keyPressEvent(QKeyEvent *event) override{emit keyPressed(event->key());}
    void keyReleaseEvent(QKeyEvent *event) override{emit keyReleased(event->key());}

private slots:
    //键盘操作
    void onKeyPressed(int key);
    void onKeyReleased(int key);

};

#endif // GAME_H


