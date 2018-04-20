#ifndef BASE_OBJECT
#define BASE_OBJECT

#include "myFunction.h"
#include <ctime>
#include <vector>

#define BULLET_SIZE  12
#define TANK_SIZE 61
#define AMO_SIZE 20
#define RELOAD_TIME 100

#define BULLET_STEP 6

#define NUM_OF_ENEMY 5
#define ENEMY_STEP 5
#define ENEMY_LIFE 1
#define ENEMY_THINK_TIME 5
#define ENEMY_REBORN_TIME 200

#define MY_STEP 2
#define MY_LIFE 5
//#define
enum DIRECTION
{
    STAND = 0,
    UP = -1,
    DOWN = 1,
    LEFT = -2,
    RIGHT = 2
};



class object{
    public:

        SDL_Texture *texture = NULL;
        int dir;
        int step;
        SDL_Rect rect;
        bool isEnemy;

        //int GetStep(){return (this->step);}
        SDL_Rect NewRect(const int& direction);

        void Draw();
};


class Bullet : public object
{
    public:
        bool isShooting;
        Bullet(const int& x, const int& y, const int& direction, const bool& isEnemy);
        ~Bullet();

        void setPos(const int& x, const int& y, const int& direction);

        void GetMove();
};


class Enemy_Tank : public object
{
    public:
        int life;
        int thinkTime;
        int reloadTime;
        float reloadTimeRate;
        int rebornTime;
        int point;
        int deadTimes;
        vector<Bullet> Bullet_Box;

        Enemy_Tank(const SDL_Rect& yourTank, const vector<Enemy_Tank>& enemy);
        ~Enemy_Tank();

        void Reborn(const SDL_Rect& yourTank, const vector<Enemy_Tank>& enemy);
        void getMove(const SDL_Rect& yourTank, const vector<Enemy_Tank>& enemy);
        void Fire();
        void TankDraw();
};


class My_Tank : public object
{
    public:
        int life;
        int score;
        vector<Bullet> Bullet_Box;
        int reloadTime;
        bool isMove;

        My_Tank(const int& x, const int& y, const int& direction);
        ~My_Tank();

        void GetMove(const vector<Enemy_Tank>& enemy);
        void Fire();
        void TankDraw();
};


bool Impact(const SDL_Rect& rA, const SDL_Rect& rB);

void checkObj(My_Tank& yourTank, vector<Enemy_Tank>& enemy);


#endif // BASE_OBJECT
