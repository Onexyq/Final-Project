#include "widget.h"
#include "ui_widget.h"
#include <functional>
#include <QFont>
#include <QDebug>

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

    connect(movementTimer, SIGNAL(timeout()), this, SLOT(Player2_Move()));

    connect(movementTimer, SIGNAL(timeout()), this, SLOT(start_moving()));

    connect(repeatTimer, SIGNAL(timeout()), this, SLOT(change_direction()));


    ui->label_2->setText("Game Over");
    ui->label_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);


    ui->label_3->setVisible(false);
    ui->label_3->setEnabled(false);
    ui->lcdNumber_2->setVisible(false);
    ui->lcdNumber_2->setEnabled(false);

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

    QFont Font1("Ebrima", player.radius ,true);
    p.setPen(QPen(Qt::blue, 3));
    p.drawEllipse(player.x, player.y, player.radius, player.radius);
    p.drawText(player.x, player.y,"P1");

    if(gamemode==2)
    {
        QFont Font1("Ebrima", player2.radius ,true);
        p.setPen(QPen(Qt::red, 3));
        p.drawEllipse(player2.x, player2.y, player2.radius, player2.radius);
        p.drawText(player2.x, player2.y,"P2");
    }

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
    if(static_cast<Qt::Key>(e->key()));
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

void Widget::Player2_Move()
{

    if(is_pressing(Qt::Key_W))
    {
        player2.goUp();
    }
    if(is_pressing(Qt::Key_S))
    {
        player2.goDown();
    }
    if(is_pressing(Qt::Key_A))
    {
        player2.goLeft();
    }
    if(is_pressing(Qt::Key_D))
    {
        player2.goRight();
    }
}


void Widget::init()
{

    ui->label_mode->setVisible(false);
    ui->pushButton_single->setVisible(false);
    ui->pushButton_single->setEnabled(false);
    ui->pushButton_double->setVisible(false);
    ui->pushButton_double->setEnabled(false);

    gameTimer->start();
    score = 0;
    ui->lcdNumber->display(score);
    player.init();
    Enemy_Balls.clear(); 

    if(gamemode == 2)
    {
        score2 = 0;
        ui->lcdNumber_2->display(score2);
        player2.init();
    }
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
         if(gamemode==1){
             vector<EBall>::iterator it=Enemy_Balls.begin();
             for (; it!=Enemy_Balls.end();it++) {
                 int x=(it->x+it->radius/2-player.x-player.radius/2);
                 int y=(it->y+it->radius/2-player.y-player.radius/2);
                 int d = sqrt(x*x+y*y);

                 if(d<=(player.radius/2-it->radius/2))
                 {
                     QSound::play("../InflatingBalls/Sounds/eat.wav");
                     player.incre_size(it->radius);
                     score += (it->radius)*(it->radius)/10;
                     ui->lcdNumber->display(score);
                     it = Enemy_Balls.erase(it);
                     if(player.radius>=700)
                     {
                         ui->label_2->setText("You Win! You are large enough! ");
                         ui->label_2->setVisible(true);
                         ui->pushButton->setVisible(true);
                         ui->pushButton_2->setVisible(true);
                         ui->pushButton->setEnabled(true);
                         ui->pushButton_2->setEnabled(true);
                     }
                 }
                 else if(d<=(it->radius/2-player.radius/2))
                 {

                     player.radius = 1;
                     ui->label_2->setText("Game Over");
                     ui->label_2->setVisible(true);
                     ui->pushButton->setVisible(true);
                     ui->pushButton_2->setVisible(true);
                     ui->pushButton->setEnabled(true);
                     ui->pushButton_2->setEnabled(true);

                 }
             }
         }

         else if(gamemode==2)
         {
             vector<EBall>::iterator it=Enemy_Balls.begin();
             for (; it!=Enemy_Balls.end();it++) {
                 int x=(it->x+it->radius/2-player.x-player.radius/2);
                 int y=(it->y+it->radius/2-player.y-player.radius/2);
                 int d = sqrt(x*x+y*y);

                 if(d<=(player.radius/2-it->radius/2))
                 {
                     QSound::play("../InflatingBalls/Sounds/eat.wav");
                     player.incre_size(it->radius);
                     score += (it->radius)*(it->radius)/10;
                     ui->lcdNumber->display(score);
                     it = Enemy_Balls.erase(it);
                 }
                 else if(d<=(it->radius/2-player.radius/2))
                 {

                     player.radius = 1;
                     ui->label_2->setText("Player2 Win");
                     ui->label_2->setVisible(true);
                     ui->pushButton->setVisible(true);
                     ui->pushButton_2->setVisible(true);
                     ui->pushButton->setEnabled(true);
                     ui->pushButton_2->setEnabled(true);

                 }
             }
             for (it=Enemy_Balls.begin(); it!=Enemy_Balls.end();it++) {
                 int x=(it->x+it->radius/2-player2.x-player2.radius/2);
                 int y=(it->y+it->radius/2-player2.y-player2.radius/2);
                 int d = sqrt(x*x+y*y);

                 if(d<=(player2.radius/2-it->radius/2))
                 {
                     QSound::play("../InflatingBalls/Sounds/eat.wav");
                     player2.incre_size(it->radius);
                     score2 += (it->radius)*(it->radius)/10;
                     ui->lcdNumber_2->display(score2);
                     it = Enemy_Balls.erase(it);
                 }
                 else if(d<=(it->radius/2-player.radius/2))
                 {

                     player2.radius = 1;
                     ui->label_2->setText("Player1 Win");
                     ui->label_2->setVisible(true);
                     ui->label_2->setEnabled(true);
                     ui->pushButton->setVisible(true);
                     ui->pushButton_2->setVisible(true);
                     ui->pushButton->setEnabled(true);
                     ui->pushButton_2->setEnabled(true);

                 }
             }

             int x=(player.x+player.radius/2-player2.x-player2.radius/2);
             int y=(player.y+player.radius/2-player2.y-player2.radius/2);
             int d = sqrt(x*x+y*y);
             if(d<player.radius/2-player2.radius/2)
             {
                    QSound::play("../InflatingBalls/Sounds/eat.wav");
                    player2.radius = 1;
                    player.incre_size(player2.radius);
                    ui->label_2->setText("Player1 Win! ");
                    ui->label_2->setVisible(true);
                    ui->label_2->setEnabled(true);
                    ui->pushButton->setVisible(true);
                    ui->pushButton_2->setVisible(true);
                    ui->pushButton->setEnabled(true);
                    ui->pushButton_2->setEnabled(true);
                    gameTimer->stop();

             }
             else if(d<player2.radius/2-player.radius/2)
             {

                    QSound::play("../InflatingBalls/Sounds/eat.wav");
                    player.radius = 1;
                    player2.incre_size(player.radius);
                    ui->label_2->setText("Player2 Win! ");
                    ui->label_2->setVisible(true);
                    ui->label_2->setEnabled(true);
                    ui->pushButton->setVisible(true);
                    ui->pushButton_2->setVisible(true);
                    ui->pushButton->setEnabled(true);
                    ui->pushButton_2->setEnabled(true);
                    gameTimer->stop();

             }

         }



     }
 }




void Widget::on_pushButton_clicked()
{
    gameTimer->stop();

    ui->label_2->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(false);
    ui->label_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(false);


    ui->label_mode->setVisible(true);
    ui->pushButton_single->setVisible(true);
    ui->pushButton_single->setEnabled(true);
    ui->pushButton_double->setVisible(true);
    ui->pushButton_double->setEnabled(true);
}

void Widget::on_pushButton_2_clicked()
{
    this->close();
}

void Widget::on_pushButton_single_clicked()
{
    gamemode = 1;

    ui->lcdNumber_2->setVisible(false);
    ui->lcdNumber_2->setEnabled(false);
    ui->label_3->setVisible(false);
    ui->label_3->setEnabled(false);

    init();
}

void Widget::on_pushButton_double_clicked()
{
    gamemode = 2;

    ui->label_3->setVisible(true);
    ui->label_3->setEnabled(true);
    ui->lcdNumber_2->setVisible(true);
    ui->lcdNumber_2->setEnabled(true);
    init();
    player2.x=player.x-100;

}
