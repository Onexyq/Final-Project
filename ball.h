#ifndef BALL_H
#define BALL_H

#include <cmath>
#include <QColor>


using namespace std;

class Ball{
private:
    int radius = 50;
    int speed = 50;

    //initial position
    int x=540;
    int y=360;

public:

    Ball(){}

    void init();

    void incre_speed(double x);

    double current_speed() const;

    void incre_size(double r);

    void destroy();

    void goUp() { if(y>0) y-=speed/25; }

    void goDown(){ if(y<720-radius) y+=speed/25; }

    void goLeft(){ if(x>0) x-=speed/25; }

    void goRight(){ if(x<1080-radius) x+=speed/25; }


    friend class Widget;
    friend class EBall;


};


//Enemy Balls
class EBall:public Ball{
private:
    int radius;
    int speed = 50;

    int x;
    int y;

    QColor color;

public:
    int direction= rand()%8;

    EBall();

    EBall(Ball p);

    EBall &operator=(EBall b);

    void random_move();

    void goUp() { if(y>0) y-=1;}

    void goDown(){ if(y<720-radius) y+=1; }

    void goLeft(){ if(x>0) x-=1; }

    void goRight(){ if(x<1080-radius) x+=1; }

    ~EBall(){}

    friend class Widget;
};



#endif // BALL_H
