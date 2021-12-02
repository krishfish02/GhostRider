///*

#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
class GhostRacer;



bool Actor::overlap(Actor* check)
{
    if(this == check)
        return false;
    double delta_x = abs(this->getX() - check->getX());
    double delta_y = abs(this->getY() - check->getY());
    double sum = this->getRadius() + check->getRadius();
    
    if(delta_x < sum*.25 && delta_y < sum * .6)
    {
        return true;
    }

    return false;

}

void Actor::newmove()
{
    double vert_speed = getySpeed() - getWorld()->getGhostRacer()->getySpeed();
    double horiz_speed = getxSpeed();

    double new_y = getY() + vert_speed;
    double new_x = getX() + horiz_speed;

    moveTo(new_x, new_y);

    if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT)
        setisAlive(false);

}


void GhostRacer::doSomething()
{

    bool alive = getisAlive();
    if (!alive)
    {
        return;
    }

    int ch = 0;

    if (getX() <= LEFT_EDGE)
    {
        if (getDirection() > 90)
        {
            getDamaged(10);
        }
        setDirection(82);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    
    else if (getX() >= RIGHT_EDGE)
    {
        if (getDirection() < 90)
        {
            getDamaged(10);
        }
        setDirection(98);
        getWorld()->playSound(SOUND_VEHICLE_CRASH);
    }
    
   else if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
        case KEY_PRESS_SPACE:
                
            if (getholyWater()>0)
            {
                double delta_x = cos(getDirection()*3.1415/180) *SPRITE_HEIGHT;
                double delta_y = sin(getDirection()*3.1415/180) *SPRITE_HEIGHT;
                
                double newx = delta_x + getX();
                double newy = delta_y + getY();
                
                getWorld()->addActor(new HolyWaterSpray(getWorld(), newx,newy, getDirection()));
                getWorld()->playSound(SOUND_PLAYER_SPRAY);
                setholyWater(getholyWater()-1);
    
            }
            break;
            
        case KEY_PRESS_LEFT:
            
            if (getDirection() < 114)
            {
                int newDirection = getDirection() + 8;
                setDirection(newDirection);
            }
            break;
        case KEY_PRESS_RIGHT:
            
            if (getDirection() > 66)
            {
                int newDirection = getDirection() - 8;
                setDirection(newDirection);
            }
            break;
        case KEY_PRESS_UP:
            if (getySpeed() < 5)
            {
                double newSpeed = getySpeed() + 1;
                setySpeed(newSpeed);
            }
            break;
        case KEY_PRESS_DOWN:
            if (getySpeed() > -1)
            {
                double newSpeed = getySpeed() - 1;
                setySpeed(newSpeed);
            }
            break;
        default:
            break;
        }
    }

    double max_shift_per_tick = 4.0;
    int direction = getDirection();
    double delta_x;
    if(direction == 90)
        delta_x = 0;
    else
        delta_x = cos(direction* 3.1415 /180.0) * max_shift_per_tick;
    
    double cur_x = getX();
    double cur_y = getY();
    moveTo(cur_x + delta_x, cur_y);

}

void GhostRacer::spin()
{
    
    int rand = randInt(5,20);
    int lr = randInt(0,1);
    int counter =getDirection() + rand;
    int clock =getDirection() - rand;
    if(lr == 0 && counter <= 120)
    {
        setDirection(counter);
    }
    else if(lr == 1 && clock >= 60)
    {
        setDirection(clock);
    }
    else if(counter <= 120)
    {
        setDirection(counter);
    }
    else
    {
        setDirection(clock);
    }
}

void Actor::getDamaged(int damage)
{
    sethitPoints(gethitPoints()-damage);
    
    if(gethitPoints()<=0)
    {
        setisAlive(false);
        playDeath();
    }
    playHurt();
}

void GhostRacer::playDeath()
{
    getWorld()->decLives();
    getWorld()->playSound(SOUND_PLAYER_DIE);
}


void ZombieCab::playDeath()
{
    getWorld()->playSound(SOUND_VEHICLE_DIE);
    int rand = randInt(1,5);
    if(rand == 1)
    {
        rand = randInt(2,5);
        int x = getX();
        int y = getY();
        getWorld()->addActor(new OilSlick(getWorld(), x, y, rand));
    }
    getWorld()->increaseScore(200);
}

void ZombiePed::playDeath()
{
    getWorld()->playSound(SOUND_PED_DIE);
    int rand = randInt(1,5);
    if(rand == 1)
    {
        int x = getX();
        int y = getY();
        getWorld()->addActor(new HealingGoodie(getWorld(), x, y));
    }
    getWorld()->increaseScore(150);
}

void ZombiePed::playHurt()
{
    if(getisAlive())
        getWorld()->playSound(SOUND_PED_HURT);
}
void ZombieCab::playHurt()
{
    if(getisAlive())
        getWorld()->playSound(SOUND_VEHICLE_HURT);
}




void GhostRacer::getHealed(int heal)
{
    sethitPoints(gethitPoints()+heal);
    
    if(gethitPoints()>=100)
    {
        sethitPoints(100);
    }
}




void BorderLine::doSomething()
{
    this->newmove();
    return;
}

void OilSlick::doActivity()
{
    
        getWorld()->getGhostRacer()->spin();
        getWorld()->playSound(SOUND_OIL_SLICK);

}


void HealingGoodie::doActivity()
{
        getWorld()->getGhostRacer()->getHealed(10);
    this->setisAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(250);
 
}

void HolyWaterGoodie::doActivity()
{
        int newwater = getWorld()->getGhostRacer()->getholyWater()+10;
        getWorld()->getGhostRacer()->setholyWater(newwater);
    this->setisAlive(false);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->increaseScore(50);

}

void SoulGoodie::doActivity()
{
        int soulsleft = getWorld()->getsoulsLeft()-1;
        getWorld()->setsoulsLeft(soulsleft);
        
    this->setisAlive(false);
        getWorld()->playSound(SOUND_GOT_SOUL);
        getWorld()->increaseScore(100);
}

void SoulGoodie::rotateClock()
{
    this->setDirection(getDirection()-10);
}


void GhostRacerActivatedObject::doSomething()
{
    this->newmove();
    if(!getisAlive())
        return;
    
    if(overlap((getWorld()->getGhostRacer())))
    {
        doActivity();
        
    }
    
    this->rotateClock();
    
    
    
}

void Pedestrian::newmovementPlan()
{
    
    int rand = 0;
    while(rand == 0)
        rand = randInt(-3,3);
    
    setxSpeed(rand);
    
    rand = randInt(4,32);
    setmovementPlan(rand);
    
    if(getxSpeed()<0)
        setDirection(180);
    else
        setDirection(0);
        
}


void HumanPed::doSomething()
{   if(!getisAlive())
        return;
    if(overlap((getWorld()->getGhostRacer())))
    {
        getWorld()->decLives();
        getWorld()->sethitPed(true);
        return;
    }
    
    this->newmove();
    if(!getisAlive())
        return;
    
    setmovementPlan(getmovementPlan()-1);
    
    if(getmovementPlan()>0)
    {
        return;
    }
    
    this->newmovementPlan();
        
}


void ZombiePed::doSomething()
{   if(!getisAlive())
        return;
    
    if(overlap((getWorld()->getGhostRacer())))
    {
        getWorld()->getGhostRacer()->getDamaged(5);
        sethitPoints(gethitPoints()-2);
        if(gethitPoints() <= 0)
        {
            setisAlive(false);
            getWorld()->playSound(SOUND_PED_DIE);
            getWorld()->increaseScore(150);
        }
        else
            getWorld()->playSound(SOUND_PED_HURT);
        
        return;
    }
        
    double ghostx = getWorld()->getGhostRacer()->getX();
    double zombiex = getX();
    
    double ghosty = getWorld()->getGhostRacer()->getY();
    double zombiey = getY();
    
    double dist = abs(ghostx-zombiex);
    
    if(dist <= 30 && ghosty < zombiey)
    {
        setDirection(270);
        
        if(zombiex < ghostx)
        {
            setxSpeed(1);
        }
        else if(zombiex > ghostx)
        {
            setxSpeed(-1);
        }
        else
        {
            setxSpeed(0);
        }
        
        setGrunt(getGrunt()-1);
        
        if(getGrunt()<=0)
        {
            getWorld()->playSound(SOUND_ZOMBIE_ATTACK);
            setGrunt(20);
        }
    }
    
    this->newmove();
    if(!getisAlive())
        return;

    
    
    if(getmovementPlan()>0)
    {
        setmovementPlan(getmovementPlan()-1);
        return;
    }
    
    newmovementPlan();
    
}


void ZombieCab ::doSomething()
{   if(!getisAlive())
        return;
    
    if(overlap((getWorld()->getGhostRacer())))
    {
        if(!getdamagedGhostRider())
        {
            getWorld()->playSound(SOUND_VEHICLE_CRASH);
            getWorld()->getGhostRacer()->getDamaged(20);
            
            double zombiex = getX();
            double ghostx = getWorld()->getGhostRacer()->getX();
            int rand = randInt(0,19);
            
            if(zombiex <= ghostx)
            {
                setxSpeed(-5);
                setDirection(120 +rand);
            }
            else
            {
                setxSpeed(5);
                setDirection(60 - rand);
            }
            
            setdamagedGhostRider(true);
        }
    }
    
    this->newmove();
    if(!getisAlive())
        return;
       
    double ghostyspeed = getWorld()->getGhostRacer()->getySpeed();
    double zombieyspeed =getySpeed();
        
    bool worthyfront = false;
    bool worthyback = false;
    double collfrontY = 257;
    double collbackY = -1;
    
    
    getWorld()->checkFrontBackLanes(this, worthyback, worthyfront, collbackY,collfrontY, getcurrLane());
    
    if(zombieyspeed > ghostyspeed && worthyfront)
    {
        if(collfrontY-getY() < 96)
        {
            setySpeed(getySpeed()-.5);
            return;
        }

    }
    else if(zombieyspeed <= ghostyspeed && worthyback)
    {
        if(getY()-collbackY < 96)
        {
        
            setySpeed(getySpeed()+.5);
            return;
        }

    }
    
    setplanLength(getplanLength()-1);
    
    if(getplanLength()>0)
        return;
    
    
    
    int rand = randInt(4,32);
    setplanLength(rand);
        
    rand = randInt(-2,2);
    setySpeed(getySpeed()+rand);
}

void HolyWaterSpray::doSomething()
{
    
    
    if(!getisAlive())
        return;
    
    if(getWorld()->sprayFirstAppropriateActor(this))
    {
        return;
    }
    
        moveForward(SPRITE_HEIGHT);
        settravelDistance(gettravelDistance()-SPRITE_HEIGHT);
    
    
    
    if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT)
    {
        setisAlive(false);
        return;
    }


    if(gettravelDistance()<=0)
    {
        setisAlive(false);
    }
        
}



bool Actor::beSprayedIfAppropriate()
{
    getDamaged(1);
    return true;
    
}

bool HumanPed::beSprayedIfAppropriate()
{
    setxSpeed(getxSpeed()*-1);
    if(getDirection() == 180)
        setDirection(0);
    else if(getDirection() == 0)
        setDirection(180);
    getWorld()->playSound(SOUND_PED_HURT);
    
    return true;
    
}
