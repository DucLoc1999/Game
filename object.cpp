#include "myFunction.h"
#include "object.h"

//ducngx95@...//
//basic object
SDL_Rect object::NewRect(const int& direction)
{
    SDL_Rect newRect = this->rect;
    if(direction != this->dir && direction != STAND)
        this->dir = direction;
    else
    switch (direction)
        {
        case UP:
            if(newRect.y > this->step + 32)
                newRect.y -= this->step;
            else
                newRect.y = 32;
            break;
        case DOWN:
            if(newRect.y + newRect.h + this->step <= SCREEN_HEIGHT)
                newRect.y += this->step;
            else
                newRect.y = SCREEN_HEIGHT - newRect.h;
            break;
        case LEFT:
            if(newRect.x > this->step)
                newRect.x -= this->step;
            else
                newRect.x = 0;
            break;
        case RIGHT :
            if(newRect.x + newRect.w + this->step <= SCREEN_WIDTH)
                newRect.x += this->step;
            else
                newRect.x = SCREEN_WIDTH - newRect.w;
            break;
        }

    return newRect;
}

void object::Draw()
{
    int degrees;
    switch (this->dir)
    {
    case UP: degrees = 0; break;
    case DOWN: degrees = 180; break;
    case LEFT: degrees = 270; break;
    case RIGHT: degrees = 90; break;
    }
    SDL_RenderCopyEx(render, texture, NULL, &this->rect, degrees, NULL, SDL_FLIP_NONE);
}


//bullet
Bullet::Bullet(const int& x, const int& y, const int& direction, const bool& isenemy)
{
    this->texture = Bullet_Texture;

    SDL_QueryTexture(this->texture, NULL, NULL, &(this->rect.w), &(this->rect.h)); // lay con tro thong tin w, h
    this->rect.x = x;
    this->rect.y = y;
    this->rect.h = BULLET_SIZE;
    this->rect.w = BULLET_SIZE;
    this->dir = direction;
    this->step = BULLET_STEP;

    this->isEnemy = isenemy;
}

Bullet::~Bullet()
{
}

void Bullet::GetMove()
{
    SDL_Rect r = this->rect;
    if(r.x <= 0 || r.y <= 32  || r.x+r.w >= SCREEN_WIDTH || r.y+r.h >= SCREEN_HEIGHT)
        this->isShooting = false;
    else
        this->rect = this->NewRect(this->dir);
}

void Bullet::setPos(const int& x, const int& y, const int& direction)
{
    this->rect.x = x;
    this->rect.y = y;
    this->dir = direction;
    if(direction == UP || direction == DOWN) // set posion
    {
        this->rect.x += (TANK_SIZE - BULLET_SIZE)/2;
        if(direction == DOWN)
            this->rect.y += TANK_SIZE;
        else
            this->rect.y -= BULLET_SIZE;
    }
    else
    {
        this->rect.y += (TANK_SIZE - BULLET_SIZE)/2;
        if(direction == RIGHT)
            this->rect.x += TANK_SIZE;
        else
            this->rect.x -= BULLET_SIZE;
    }
    this->isShooting = true;
}


//enemy
Enemy_Tank::Enemy_Tank(const SDL_Rect& yourTank, const vector<Enemy_Tank>& enemy)// constructor
{
    this->texture = Enemy_Texture;

    SDL_QueryTexture(this->texture, NULL, NULL, &(this->rect.w), &(this->rect.h)); // lay con tro thong tin w, h
    this->rect.h = TANK_SIZE;
    this->rect.w = TANK_SIZE;
    this->step = ENEMY_STEP;
    this->isEnemy = true;
    this->deadTimes = 0;
    this->Reborn(yourTank, enemy);

    for(int i = 0; i < AMO_SIZE; i++)
    {
        Bullet pre_Bullet(0, 0, UP, this->isEnemy);
        (this->Bullet_Box).push_back(pre_Bullet);
    }
}

Enemy_Tank::~Enemy_Tank()
{
    for(int i = AMO_SIZE; i >= 0; i--)
        this->Bullet_Box[i].~Bullet();
    this->Bullet_Box.clear();
}

void Enemy_Tank::Reborn(const SDL_Rect& yourTank, const vector<Enemy_Tank>& enemy)
{
    this->rect.x = rand()%(SCREEN_WIDTH - TANK_SIZE + 1);
    this->rect.y = 32 + rand()%(SCREEN_HEIGHT - 32 - TANK_SIZE + 1);

    bool canSetRect = true;
    if(Impact(yourTank, this->rect))
        canSetRect = false;
    else if(enemy.size() > 0)
    {
        for(int i = enemy.size() - 1; i >= 0; i--)
            if(enemy[i].life > 0 && Impact(this->rect, enemy[i].rect))
            {
                canSetRect = false;
                break;
            }
    }

    if(canSetRect)
    {
        int rdir = rand()%4;
        switch (rdir)
        {
            case 0: rdir = UP; break;
            case 1: rdir = DOWN; break;
            case 2: rdir = LEFT; break;
            case 3: rdir = RIGHT; break;
        }
        this->dir = rdir;
        this->reloadTime = RELOAD_TIME / 2 + rand() % (RELOAD_TIME / 2) + 30;
        this->life = ENEMY_LIFE;
        this->thinkTime = ENEMY_THINK_TIME + 30;
        this->rebornTime = ENEMY_REBORN_TIME;
        this->point = 1;
        this->reloadTimeRate = 1.2;

        int n = rand() % (this->deadTimes + 1);

        if(n <= 3)
            this->texture = Enemy_Texture;

        else if(n <= 7)
        {
            this->point = 2;
            this->life = 2;
            this->texture = Enemy_Texture1;
        }
        else if(n <= 10)
        {
            this->point = 3;
            this->life = 2;
            this->rebornTime = ENEMY_REBORN_TIME - 50;
            this->reloadTimeRate = 1;
            this->texture = Enemy_Texture2;
        }
        else
        {
            this->point = 4;
            this->life = 3;
            this->rebornTime = ENEMY_REBORN_TIME + 100;
            this->reloadTimeRate = 0.8;
            this->texture = Enemy_Texture3;
        }

    }
    else
        this->life = 0;
}

void Enemy_Tank::getMove(const SDL_Rect& yourTank, const vector<Enemy_Tank>& enemy)
{
    if(this->reloadTime <= 0 )
        this->Fire();

    else if(this->thinkTime <= 0)
        {
            int oldDir = this->dir;
            int rdir[2];
            if(this->rect.x < yourTank.x)
                rdir[0] = RIGHT;
            else if(this->rect.x > yourTank.x)
                rdir[0] = LEFT;
            else
                rdir[0] = STAND;


            if(this->rect.y < yourTank.y)
                rdir[1] = DOWN;
            else if(this->rect.y > yourTank.y)
                rdir[1] = UP;
            else
                rdir[1] = rdir[0];

            SDL_Rect NewRect;
            int r = rand()%2;

            if(this->dir == rdir[0] || this->dir == rdir[1])
                NewRect = this->NewRect(this->dir);
            else
                NewRect = this->NewRect((rdir[0] == STAND ? rdir[1] : rdir[r]));

            bool canMove = true;
            if(Impact(yourTank, NewRect))
                canMove = false;
            else if(enemy.size() > 0)
            {
                for(int i = enemy.size() - 1; i >= 0; i--)
                    if(enemy[i].life > 0 && ( this->rect.x != enemy[i].rect.x || this->rect.y != enemy[i].rect.y) && Impact( NewRect, enemy[i].rect))  //SUA LAI DE CHINH TAO DO KHONG CAN OLD_RECT
                    {
                        canMove = false;
                        break;
                    }
            }
            if(canMove)
                this->rect = NewRect;
            else if(rdir[1 - r] != rdir[r] && rdir[1 - r] != STAND)
                {
                    this->dir = oldDir;
                    NewRect = this->NewRect(rdir[1 - r]);
                    canMove = true;
                    if(Impact(yourTank, NewRect))
                        canMove = false;
                    else if(enemy.size() > 0)
                    {
                        for(int i = enemy.size() - 1; i >= 0; i--)
                            if(( this->rect.x != enemy[i].rect.x || this->rect.y != enemy[i].rect.y) && Impact( NewRect, enemy[i].rect))  //SUA LAI DE CHINH TAO DO KHONG CAN OLD_RECT
                            {
                                canMove = false;
                                break;
                            }
                    }
                    if(canMove)
                        this->rect = NewRect;
                }

            this->thinkTime = ENEMY_THINK_TIME - point + 1;
        }

    this->reloadTime--;
    this->thinkTime--;
}

void Enemy_Tank::Fire()
{
    int i = 0;
    for(; i < AMO_SIZE && this->Bullet_Box[i].isShooting; i++);

    this->Bullet_Box[i].setPos(this->rect.x, this->rect.y, this->dir);
    this->reloadTime = RELOAD_TIME * this->reloadTimeRate;
}

void Enemy_Tank::TankDraw()
{
    if(this->life > 0)
        this->Draw();
    for(int i = 0; i < AMO_SIZE; i++)
        if(this->Bullet_Box[i].isShooting)
        {
            this->Bullet_Box[i].GetMove();
            this->Bullet_Box[i].Draw();
        }
}


//you
My_Tank::My_Tank(const int& x, const int& y, const int& direction)// constructor
{
    this->texture = YourTank_Texture;

    SDL_QueryTexture(this->texture, NULL, NULL, &(this->rect.w), &(this->rect.h)); // lay con tro thong tin w, h
    this->rect.x = x;
    this->rect.y = y;
    this->rect.h = TANK_SIZE;
    this->rect.w = TANK_SIZE;

    this->dir = direction;
    this->step = MY_STEP;
    this->isEnemy = false;
    this->life = MY_LIFE;
    this->reloadTime = RELOAD_TIME;
    this->isMove = false;
    this->score = 0;

    for(int i = 0; i < AMO_SIZE; i++)
    {
        Bullet pre_Bullet(0, 0, UP, this->isEnemy);
        (this->Bullet_Box).push_back(pre_Bullet);
    }

}

My_Tank::~My_Tank()
{
    for(int i = AMO_SIZE; i >= 0; i--)
        this->Bullet_Box[i].~Bullet();
    this->Bullet_Box.clear();
}

void My_Tank::GetMove(const vector<Enemy_Tank>& enemy)
{
    int nDir = this->dir;
    SDL_Rect screenRect = {0, 32, SCREEN_WIDTH, SCREEN_HEIGHT - 32};

    if(this->reloadTime > 0) reloadTime--;

    if(MouseIn(screenRect) && event.button.button == SDL_BUTTON_LEFT && event.button.type == SDL_MOUSEBUTTONDOWN && this->reloadTime <= 0)
        this->Fire();

    else if(event.type == SDL_KEYUP)
        switch(event.key.keysym.sym)
        {
            case SDLK_UP: if(this->dir == UP) isMove = false; break;
            case SDLK_DOWN: if(this->dir == DOWN) isMove = false; break;
            case SDLK_LEFT: if(this->dir == LEFT) isMove = false; break;
            case SDLK_RIGHT: if(this->dir == RIGHT) isMove = false; break;
        }
    else if(event.type == SDL_KEYDOWN)
        switch(event.key.keysym.sym)
        {
            case SDLK_UP: nDir = UP ; isMove = true; break;
            case SDLK_DOWN: nDir = DOWN; isMove = true; break;
            case SDLK_LEFT: nDir = LEFT; isMove = true; break;
            case SDLK_RIGHT: nDir = RIGHT; isMove = true; break;
        }

    if(this->isMove)
    {
        SDL_Rect NewRect = this->NewRect(nDir);
        bool canMove = true;

        if(enemy.size() > 0)
            for(int i = enemy.size() - 1; i >= 0; i--)
                if(enemy[i].life > 0 && Impact( NewRect, enemy[i].rect))
                {
                    canMove =false;
                    break;
                }

        if(canMove)
            this->rect = NewRect;
    }

}

void My_Tank::Fire()
{
    int i = 0;
    for(; i < AMO_SIZE && this->Bullet_Box[i].isShooting; i++);

    this->Bullet_Box[i].setPos(this->rect.x, this->rect.y, this->dir);
    this->reloadTime = RELOAD_TIME;
}

void My_Tank::TankDraw()
{
    if(this->life > 0)
        this->Draw();

    this->Draw();
    for(int i = 0; i < AMO_SIZE; i++)
        if(this->Bullet_Box[i].isShooting)
        {
            this->Bullet_Box[i].GetMove();
            this->Bullet_Box[i].Draw();
        }
    if(this->reloadTime > 0)this->reloadTime--;
}



//other game fuction
bool Impact(const SDL_Rect& rA, const SDL_Rect& rB)  //SUA
{
    return (rA.x+rA.w > rB.x && rA.y+rA.h > rB.y  && rA.x < rB.x+rB.w && rA.y < rB.y+rB.h);
}


void checkObj(My_Tank &yourTank, vector<Enemy_Tank> &enemy) // kieemr tra xem  xe naof cheets
{
    for(int i = AMO_SIZE - 1; i >= 0 ; i--)
        for(int j = enemy.size() - 1; j >= 0; j--)
            if(enemy[j].life > 0 && yourTank.Bullet_Box[i].isShooting && Impact(yourTank.Bullet_Box[i].rect,enemy[j].rect ))
            {
                enemy[j].life--;
                if(enemy[j].life <= 0)
                {
                    enemy[j].deadTimes++;
                    yourTank.score += enemy[j].point;
                }
                yourTank.Bullet_Box[i].isShooting = false;
            }
    for(int j = enemy.size() - 1; j >= 0; j--)
        for(int k = AMO_SIZE - 1; k >= 0; k--)
            if(yourTank.life > 0 && enemy[j].Bullet_Box[k].isShooting && Impact(enemy[j].Bullet_Box[k].rect, yourTank.rect))
            {
                yourTank.life--;
                enemy[j].Bullet_Box[k].isShooting = false;
            }

}




=======
#include "myFunction.h"
#include "object.h"

//ducngx95@...//
//basic object
SDL_Rect object::NewRect(int direction)
{
    SDL_Rect newRect = this->rect;
    if(direction != this->dir && direction != STAND)
        this->dir = direction;
    else
    switch (direction)
        {
        case UP:
            if(newRect.y >= this->step )
                newRect.y -= this->step;
            else
                newRect.y = 0;
            break;
        case DOWN:
            if(newRect.y + newRect.h + this->step <= SCREEN_HEIGHT)
                newRect.y += this->step;
            else
                newRect.y = SCREEN_HEIGHT - newRect.h;
            break;
        case LEFT:
            if(newRect.x >= this->step)
                newRect.x -= this->step;
            else
                newRect.x = 0;
            break;
        case RIGHT :
            if(newRect.x + newRect.w + this->step <= SCREEN_WIDTH)
                newRect.x += this->step;
            else
                newRect.x = SCREEN_WIDTH - newRect.w;
            break;
        }

    return newRect;
}

void object::Draw()
{
    int degrees;
    switch (this->dir)
    {
    case UP: degrees = 0; break;
    case DOWN: degrees = 180; break;
    case LEFT: degrees = 270; break;
    default : degrees = 90;
    }
    SDL_RenderCopyEx(render, texture, NULL, &this->rect, degrees, NULL, SDL_FLIP_NONE);
}


//bullet
Bullet::Bullet(int x, int y, int direction, bool isenemy)
{
    this->texture = Bullet_Texture;

    SDL_QueryTexture(this->texture, NULL, NULL, &(this->rect.w), &(this->rect.h)); // lay con tro thong tin w, h
    this->rect.x = x;
    this->rect.y = y;
    this->rect.h = BULLET_SIZE;
    this->rect.w = BULLET_SIZE;
    this->dir = direction;
    this->step = BULLET_STEP;

    this->isEnemy = isenemy;
}

Bullet::~Bullet()
{
}

void Bullet::GetMove()
{
    SDL_Rect r = this->rect;
    if(r.x == 0 || r.y == 0 || r.x+r.w == SCREEN_WIDTH || r.y+r.h == SCREEN_HEIGHT)
        this->isShooting = false;
    else
        this->rect = this->NewRect(this->dir);
}

void Bullet::setPos(int x, int y, int direction)
{
    this->rect.x = x;
    this->rect.y = y;
    this->dir = direction;
    if(direction == UP || direction == DOWN) // set posion
    {
        this->rect.x += (TANK_SIZE - BULLET_SIZE)/2;
        if(direction == DOWN)
            this->rect.y += TANK_SIZE;
        else
            this->rect.y -= BULLET_SIZE;
    }
    else
    {
        this->rect.y += (TANK_SIZE - BULLET_SIZE)/2;
        if(direction == RIGHT)
            this->rect.x += TANK_SIZE;
        else
            this->rect.x -= BULLET_SIZE;
    }
    this->isShooting = true;
}


//enemy
Enemy_Tank::Enemy_Tank(SDL_Rect yourTank, vector<Enemy_Tank> enemy)// constructor
{
    this->texture = Enemy_Texture;

    SDL_QueryTexture(this->texture, NULL, NULL, &(this->rect.w), &(this->rect.h)); // lay con tro thong tin w, h
    this->rect.h = TANK_SIZE;
    this->rect.w = TANK_SIZE;
    this->Reborn(yourTank, enemy);
    this->step = ENEMY_STEP;
    this->isEnemy = true;
    this->rebornTime = ENEMY_REBORN_TIME;

    for(int i = 0; i < AMO_SIZE; i++)
    {
        Bullet pre_Bullet(0, 0, UP, this->isEnemy);
        (this->Bullet_Box).push_back(pre_Bullet);
        printf("%d\n",i);
    }
}

Enemy_Tank::~Enemy_Tank()
{
    for(int i = AMO_SIZE; i >= 0; i--)
        this->Bullet_Box[i].~Bullet();
    this->Bullet_Box.clear();
}

void Enemy_Tank::Reborn(SDL_Rect yourTank, vector<Enemy_Tank> enemy)
{
    this->rect.x = rand()%(SCREEN_WIDTH - TANK_SIZE + 1);
    this->rect.y = rand()%(SCREEN_HEIGHT - TANK_SIZE + 1);

    bool canSetRect = true;
    if(Impact(yourTank, this->rect))
        canSetRect = false;
    else if(enemy.size() > 0)
    {
        for(int i = enemy.size() - 1; i >= 0; i--)
            if(enemy[i].life > 0 && Impact(this->rect, enemy[i].rect))
            {
                canSetRect = false;
                break;
            }
    }

    if(canSetRect)
    {
        int rdir = rand()%4;
        switch (rdir)
        {
            case 0: rdir = UP; break;
            case 1: rdir = DOWN; break;
            case 2: rdir = LEFT; break;
            case 3: rdir = RIGHT; break;
        }
        this->dir = rdir;
        this->life = ENEMY_LIFE;
        this->thinkTime = ENEMY_THINK_TIME;
        this->reloadTime = ENEMY_RELOAD_TIME / 2 + rand() % (ENEMY_RELOAD_TIME / 2);
    }
    else
        this->life = 0;
}

void Enemy_Tank::getMove(SDL_Rect yourTank, vector<Enemy_Tank> enemy)
{
    if(this->reloadTime <= 0 )
        this->Fire();

    else if(this->thinkTime <= 0)
        {
            int oldDir = this->dir;
            int rdir[2];
            if(this->rect.x < yourTank.x)
                rdir[0] = RIGHT;
            else if(this->rect.x > yourTank.x)
                rdir[0] = LEFT;
            else
                rdir[0] = STAND;


            if(this->rect.y < yourTank.y)
                rdir[1] = DOWN;
            else if(this->rect.y > yourTank.y)
                rdir[1] = UP;
            else
                rdir[1] = rdir[0];

            SDL_Rect NewRect;
            int r = rand()%2;

            if(this->dir == rdir[0] || this->dir == rdir[1])
                NewRect = this->NewRect(this->dir);
            else
                NewRect = this->NewRect((rdir[0] == STAND ? rdir[1] : rdir[r]));

            bool canMove = true;
            if(Impact(yourTank, NewRect))
                canMove = false;
            else if(enemy.size() > 0)
            {
                for(int i = enemy.size() - 1; i >= 0; i--)
                    if(enemy[i].life > 0 && ( this->rect.x != enemy[i].rect.x || this->rect.y != enemy[i].rect.y) && Impact( NewRect, enemy[i].rect))  //SUA LAI DE CHINH TAO DO KHONG CAN OLD_RECT
                    {
                        canMove = false;
                        break;
                    }
            }
            if(canMove)
                this->rect = NewRect;
            else if(rdir[1 - r] != rdir[r] && rdir[1 - r] != STAND)
                {
                    this->dir = oldDir;
                    NewRect = this->NewRect(rdir[1 - r]);
                    canMove = true;
                    if(Impact(yourTank, NewRect))
                        canMove = false;
                    else if(enemy.size() > 0)
                    {
                        for(int i = enemy.size() - 1; i >= 0; i--)
                            if(( this->rect.x != enemy[i].rect.x || this->rect.y != enemy[i].rect.y) && Impact( NewRect, enemy[i].rect))  //SUA LAI DE CHINH TAO DO KHONG CAN OLD_RECT
                            {
                                canMove = false;
                                break;
                            }
                    }
                    if(canMove)
                        this->rect = NewRect;
                }

            this->thinkTime = ENEMY_THINK_TIME;
        }

    this->reloadTime--;
    this->thinkTime--;
}

void Enemy_Tank::Fire()
{
    int i = 0;
    for(; i < AMO_SIZE && this->Bullet_Box[i].isShooting; i++);

    this->Bullet_Box[i].setPos(this->rect.x, this->rect.y, this->dir);
    this->reloadTime = ENEMY_RELOAD_TIME;
}

void Enemy_Tank::TankDraw()
{
    if(this->life > 0)
        this->Draw();
    for(int i = 0; i < AMO_SIZE; i++)
        if(this->Bullet_Box[i].isShooting)
        {
            this->Bullet_Box[i].GetMove();
            this->Bullet_Box[i].Draw();
        }
}


//you
My_Tank::My_Tank(int x, int y, int direction)// constructor
{
    this->texture = YourTank_Texture;

    SDL_QueryTexture(this->texture, NULL, NULL, &(this->rect.w), &(this->rect.h)); // lay con tro thong tin w, h
    this->rect.x = x;
    this->rect.y = y;
    this->rect.h = TANK_SIZE;
    this->rect.w = TANK_SIZE;

    this->dir = direction;
    this->step = MY_STEP;
    this->isEnemy = false;
    this->life = MY_LIFE;
    this->reloadTime = MY_RELOAD_TIME;
    this->isMove = false;
    this->score = 0;

    for(int i = 0; i < AMO_SIZE; i++)
    {
        Bullet pre_Bullet(0, 0, UP, this->isEnemy);
        (this->Bullet_Box).push_back(pre_Bullet);
    }

}

My_Tank::~My_Tank()
{
    for(int i = AMO_SIZE; i >= 0; i--)
        this->Bullet_Box[i].~Bullet();
    this->Bullet_Box.clear();
}

void My_Tank::GetMove(SDL_Event event, const vector<Enemy_Tank> enemy)
{
    int nDir = this->dir;
    if(this->reloadTime > 0) reloadTime--;

    if( event.key.keysym.sym == SDLK_SPACE && event.type == SDL_KEYUP && this->reloadTime <= 0)
        {this->Fire();printf("fire");}

    else if(event.type == SDL_KEYUP)
        switch(event.key.keysym.sym)
        {
            case SDLK_UP: if(this->dir == UP) isMove = false; break;
            case SDLK_DOWN: if(this->dir == DOWN) isMove = false; break;
            case SDLK_LEFT: if(this->dir == LEFT) isMove = false; break;
            case SDLK_RIGHT: if(this->dir == RIGHT) isMove = false; break;
        }
    else if(event.type == SDL_KEYDOWN)
        switch(event.key.keysym.sym)
        {
            case SDLK_UP: nDir = UP ; isMove = true; break;
            case SDLK_DOWN: nDir = DOWN; isMove = true; break;
            case SDLK_LEFT: nDir = LEFT; isMove = true; break;
            case SDLK_RIGHT: nDir = RIGHT; isMove = true; break;
        }

    if(this->isMove)
    {
        SDL_Rect NewRect = this->NewRect(nDir);
        bool canMove = true;

        if(enemy.size() > 0)
            for(int i = enemy.size() - 1; i >= 0; i--)
                if(enemy[i].life > 0 && Impact( NewRect, enemy[i].rect))
                {
                    canMove =false;
                    break;
                }

        if(canMove)
            this->rect = NewRect;
    }

}

void My_Tank::Fire()
{
    int i = 0;
    for(; i < AMO_SIZE && this->Bullet_Box[i].isShooting; i++);

    this->Bullet_Box[i].setPos(this->rect.x, this->rect.y, this->dir);
    this->reloadTime = MY_RELOAD_TIME;
}

void My_Tank::TankDraw()
{
    if(this->life > 0)
        this->Draw();
    this->Draw();
    for(int i = 0; i < AMO_SIZE; i++)
        if(this->Bullet_Box[i].isShooting)
        {
            this->Bullet_Box[i].GetMove();
            this->Bullet_Box[i].Draw();
        }
    if(this->reloadTime > 0)this->reloadTime--;
}



//other game fuction
bool Impact(const SDL_Rect rA, const SDL_Rect rB)  //SUA
{
    return (rA.x+rA.w > rB.x && rA.y+rA.h > rB.y  && rA.x < rB.x+rB.w && rA.y < rB.y+rB.h);
}


void checkObj(My_Tank &yourTank, vector<Enemy_Tank> &enemy) // kieemr tra xem  xe naof cheets
{
    for(int i = AMO_SIZE - 1; i >= 0 ; i--)
        for(int j = enemy.size() - 1; j >= 0; j--)
            if(enemy[j].life > 0 && yourTank.Bullet_Box[i].isShooting && Impact(yourTank.Bullet_Box[i].rect,enemy[j].rect ))
            {
                enemy[j].life--;
                enemy[j].rebornTime = ENEMY_REBORN_TIME;
                yourTank.Bullet_Box[i].isShooting = false;
                printf("enemy has been shooted : %d\n", enemy[j].life);
                yourTank.score++;
            }
    for(int j = enemy.size() - 1; j >= 0; j--)
        for(int k = AMO_SIZE - 1; k >= 0; k--)
            if(yourTank.life > 0 && enemy[j].Bullet_Box[k].isShooting && Impact(enemy[j].Bullet_Box[k].rect, yourTank.rect))
            {
                yourTank.life--;
                enemy[j].Bullet_Box[k].isShooting = false;
                printf("you have been shooted : %d\n", yourTank.life);
            }

}




>>>>>>> b771aa9c90c96e47cf569946731800eeaf4a9ebf
