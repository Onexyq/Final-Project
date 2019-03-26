#include"ball.h"
#include<ctime>
#include<QDebug>

Ball::Ball(){

}


void Ball::update_ball()
{

}

void Ball::incre_speed(double x)
{
    speed*= (1+x);
}

 double Ball::current_speed() const
 {
     return speed;
 }

 void Ball::incre_size(double r)
 {
     radius += sqrt(r);
 }


 void Ball::init()
 {
     radius = 50;
     speed = 50;

     x=540;
     y=360;
 }

void Ball::destroy()
{

}




EBall::EBall()
{
    x=20;
    y=20;
    radius=20;
}

EBall::EBall(Ball p)
{
    color=QColor(rand()%255,rand()%255,rand()%255);

    srand(time(NULL));

    bool a=rand()%2;
    bool b=rand()%2;

    if(a)
         x = rand()%(p.x-p.radius*3/2+1000)-1000;
    else
         x = rand()%(2000-p.x-p.radius*3/2)+p.x+p.radius*3/2;

    if(b)
         y = rand()%(p.y-p.radius*3/2+800)-800;
    else
         y = rand()%(1400-p.y-p.radius*3/2)+p.y+p.radius*3/2;


     radius=rand()%(p.radius)+3/2*rand()%(p.radius);
}

EBall &EBall::operator=(EBall b)
{
    EBall temp = b;
    std::swap(x, b.x);
    std::swap(y, b.y);
    std::swap(radius, b.radius);

    return *this;
}

void EBall::random_move()
{

     switch(direction){
       case 0:
         goUp();
         break;
       case 1:
         goDown();
         break;
       case 2:
         goLeft();
         break;
       case 3:
         goRight();
         break;
       case 4:
         goUp();
         goLeft();
         break;
       case 5:
         goUp();
         goRight();
         break;
       case 6:
         goDown();
         goLeft();
         break;
       case 7:
         goDown();
         goRight();
         break;
    }
     return;
}










