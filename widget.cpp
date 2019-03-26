#include "widget.h"
#include "ui_widget.h"
#include <functional>
#include<QDebug>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    gameTimer(new QTimer(this))
{
    ui->setupUi(this);

    repeatTimer = new QTimer(this);

    movementTimer = new QTimer(this);

    connect(gameTimer, SIGNAL(timeout()), this, SLOT(update()));

    connect(gameTimer, SIGNAL(timeout()), this, SLOT(checkState()));

    connect(repeatTimer, SIGNAL(timeout()), this, SLOT(generate_balls()));

    connect(movementTimer, SIGNAL(timeout()), this, SLOT(Player_Move()));

    connect(movementTimer, SIGNAL(timeout()), this, SLOT(start_moving()));

    connect(repeatTimer, SIGNAL(timeout()), this, SLOT(change_direction()));

    ui->label_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    time_t t1, t2;
    time(&t1);

    gameTimer->start(2); //2ms
    repeatTimer->start(500);  //0.5s
    movementTimer->start(10); //10ms
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *)
{

    QPainter p(this);

    //Background
    p.drawPixmap(0,0,width(),height(),QPixmap("../InflatingBalls/Images/BG1.jpg"));

    p.setPen(QPen(Qt::blue, 3));
    p.drawEllipse(player.x, player.y, player.radius, player.radius);



    for(auto it: Enemy_Balls)
    {
        p.setPen(QPen(it.color, 2));
        p.drawEllipse(it.x,it.y, it.radius, it.radius);
        p.drawEllipse(it.x+it.radius/2,it.y+it.radius/2, 1, 1);
    }

}


void Widget::keyPressEvent(QKeyEvent *e)
{
    pressedkeys.push_back(static_cast<Qt::Key>(e->key()));

}

void Widget::keyReleaseEvent(QKeyEvent *e)
{
    vector<Qt::Key>temp(pressedkeys.size());
    vector<Qt::Key>::iterator it = pressedkeys.begin();
    copy_if(pressedkeys.begin(),pressedkeys.end(),
                 it, [e, &it](bool){return  *(it++)!= static_cast<Qt::Key>(e->key());  });

    pressedkeys = temp;
}

 bool Widget::is_pressing(Qt::Key k)
 {
     for(auto it: pressedkeys)
     {
         if(k==it)
             return true;
     }
     return false;
 }

void Widget::Player_Move()
{

    if(is_pressing(Qt::Key_Up))
    {
        player.goUp();
    }
    if(is_pressing(Qt::Key_Down))
    {
        player.goDown();
    }
    if(is_pressing(Qt::Key_Left))
    {
        player.goLeft();
    }
    if(is_pressing(Qt::Key_Right))
    {
        player.goRight();
    }
}

void Widget::init()
{
    ui->label_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);

    player.init();
    Enemy_Balls.clear(); 
}

void Widget::generate_balls()
{

    if(Enemy_Balls.size()<20)
    {
        qDebug()<<"new enemy is created";
        EBall *e= new EBall(player);

        Enemy_Balls.push_back(*e);

        delete e;
    }
}

 void Widget::start_moving()
 {
     if(Enemy_Balls.size()!=0)
     {
         vector<EBall>::iterator it=Enemy_Balls.begin();
         for (; it!=Enemy_Balls.end();it++) {
              it->random_move();
         }
     }
 }

 void Widget::change_direction()
 {
     if(Enemy_Balls.size()!=0)
     {
         vector<EBall>::iterator it=Enemy_Balls.begin();
         for (; it!=Enemy_Balls.end();it++) {
              it->direction=rand()%8;
         }
     }
 }

 void Widget::checkState()
 {
     if(Enemy_Balls.size()!=0)
     {
         vector<EBall>::iterator it=Enemy_Balls.begin();
         for (; it!=Enemy_Balls.end();it++) {
             int x=(it->x+it->radius/2-player.x-player.radius/2);
             int y=(it->y+it->radius/2-player.y-player.radius/2);
             int d = sqrt(x*x+y*y);

             if(d<=(player.radius/2-it->radius/2))
             {
                 player.incre_size(it->radius);
                 score += (it->radius)*(it->radius)/10;
                 ui->lcdNumber->display(score);
                 it = Enemy_Balls.erase(it);
             }
             else if(d<=(it->radius/2-player.radius/2))
             {

                 player.radius=1;
                 ui->label_2->setVisible(true);
                 ui->pushButton->setVisible(true);
                 ui->pushButton_2->setVisible(true);
                 ui->label_2->setEnabled(true);
                 ui->pushButton->setEnabled(true);
                 ui->pushButton_2->setEnabled(true);

             }
         }
         /**
         vector<EBall>::iterator iter=Enemy_Balls.begin();
         for (it=Enemy_Balls.begin(); it!=Enemy_Balls.end();it++) {
             for (; iter!=Enemy_Balls.end();iter++)
             {
                 if(it!=iter){
                     int x=(it->x+it->radius/2-iter->x-iter->radius/2);
                     int y=(it->y+it->radius/2-iter->y-iter->radius/2);
                     int d = sqrt(x*x+y*y);
                      if(d<=(it->radius/2-iter->radius/2))
                      {
                         it->incre_size(iter->radius);
                         iter = Enemy_Balls.erase(iter);
                      }
                 }

             }
         }
         */
     }
 }




void Widget::on_pushButton_clicked()
{
    init();
}

void Widget::on_pushButton_2_clicked()
{
    this->close();
}
