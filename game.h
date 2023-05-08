#ifndef GAME_H
#define GAME_H

#include <QWidget>
//返回指令需包含类 Order
#include "order.h"
//键盘事件类
#include <QKeyEvent>

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT

    //信号
signals:
    void updateLabels(const QString &text1, const QString &text2, const QString &text3);

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();
    // 键盘事件处理函数
    QString KeyPressEvent(QKeyEvent* key);

private slots:
    void on_pushButton_clicked();

    void on_back_pushButton_clicked();

    void on_Again_pushButton_clicked();

private:
    Ui::Game *ui;
};

#endif // GAME_H

