#ifndef WIDGET_H
#define WIDGET_H


#include <QWidget>
#include <vector>
#include <QKeyEvent>
#include <QPainter>
#include <QTimer>
#include <QTime>
#include "ball.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{

Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);

    ~Widget();

    void init();

protected:
    void paintEvent(QPaintEvent *);

    void keyPressEvent(QKeyEvent *e);

    void keyReleaseEvent(QKeyEvent *e);

private slots:
    void Player_Move();

    void generate_balls();

    void start_moving();

    void change_direction();

    void checkState();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Widget *ui;

    QTimer *gameTimer;

    QTimer *repeatTimer;

    QTimer *movementTimer;

    Ball player;

    bool Gamestart;

    bool is_pressing(Qt::Key k);

    vector<Qt::Key>pressedkeys;

    vector<EBall>Enemy_Balls;

    int score = 0;

};

#endif // WIDGET_H
