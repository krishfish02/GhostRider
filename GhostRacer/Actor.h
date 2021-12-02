///*
#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


const int STARTDIRECTION_GHOST_RACER = 90;
const double STARTX_GHOST_RACER = 128;
const double STARTY_GHOST_RACER = 32;
const double SIZE_GHOST_RACER = 4.0;
const int DEPTH_GHOST_RACER = 0;
const double XSPEED_GHOST_RACER = 0;
const double YSPEED_GHOST_RACER = 0;

const int STARTDIRECTION_BORDER = 0;
const double SIZE_BORDER = 2;
const int DEPTH_BORDER = 2;
const double XSPEED_BORDER = 0;
const double YSPEED_BORDER = -4;

const int STARTDIRECTION_OIL = 0;
const int DEPTH_OIL = 2;
const double XSPEED_OIL = 0;
const double YSPEED_OIL = -4;


class StudentWorld;

class Actor : public GraphObject
{
public:
    Actor(StudentWorld* thisWorld, int imageID, bool isAlive, int sDirection,  double sX, double sY, double size, int depth, double xSpeed, double ySpeed, int hitPoints, bool worthy)
        : GraphObject(imageID, sX, sY, sDirection, size, depth), thisWorld(thisWorld), m_isAlive(isAlive), m_xSpeed(xSpeed), m_ySpeed(ySpeed), m_hitPoints(hitPoints), m_worthy(worthy), m_isAffected(true){}
    virtual ~Actor() {}
    virtual void doSomething() = 0;

    bool overlap(Actor* check);
    void newmove();
    
    StudentWorld* getWorld() { return thisWorld; }
    
    bool getisAlive() { return m_isAlive; }
    double getxSpeed() { return m_xSpeed; }
    double getySpeed() { return m_ySpeed; }
    int gethitPoints() { return m_hitPoints; }
    
    
    void setisAlive(bool alive) { m_isAlive = alive; }
    void setxSpeed(double x) { m_xSpeed = x; }
    void setySpeed(double y) { m_ySpeed = y; }
    void sethitPoints(int hitPoints) { m_hitPoints = hitPoints; }
    
    virtual bool beSprayedIfAppropriate();
    
    bool getworthy(){return m_worthy;}
    
    bool getisAffected(){return m_isAffected;}
    void setisAffected(bool temp){m_isAffected = temp;}
    
    void getDamaged(int damage);
    
    virtual void playDeath(){}
    virtual void playHurt(){}



private:
    StudentWorld* thisWorld;
    bool m_isAlive;
    double m_xSpeed;
    double m_ySpeed;
    int m_hitPoints;
    bool m_worthy;
    
    bool m_isAffected;
    

};



class GhostRacer : public Actor
{
public:
    GhostRacer(StudentWorld* thisWorld)
        : Actor(thisWorld, IID_GHOST_RACER, true, STARTDIRECTION_GHOST_RACER, STARTX_GHOST_RACER, STARTY_GHOST_RACER, SIZE_GHOST_RACER, DEPTH_GHOST_RACER, XSPEED_GHOST_RACER, YSPEED_GHOST_RACER, 100, true)
    {
        m_holyWater = 10;
    }
    
    void doSomething();
    void spin();
    
    void setholyWater(int holyWater) { m_holyWater = holyWater; }
    int getholyWater() { return m_holyWater;}
    
    void getHealed(int heal);
    
    void playDeath();

private:
    int m_holyWater;


};

class BorderLine : public Actor
{
public:
    BorderLine(StudentWorld* thisWorld, int imageID, double sX, double sY)
        : Actor(thisWorld, imageID, true, STARTDIRECTION_BORDER, sX, sY, SIZE_BORDER, DEPTH_BORDER, XSPEED_BORDER, YSPEED_BORDER, 0, false)
    {
        setisAffected(false);
    }
    void doSomething();


private:

};


class GhostRacerActivatedObject: public Actor
{
public:
    GhostRacerActivatedObject(StudentWorld* thisWorld, int imageID, int direction, double sX, double sY, double size)
        : Actor(thisWorld, imageID, true, direction, sX, sY, size, 2, 0, -4, 0, false) {}
    void doSomething();
    virtual void doActivity() = 0;
    virtual void rotateClock() {}


private:

};



class OilSlick : public GhostRacerActivatedObject
{
public:
    OilSlick(StudentWorld* thisWorld, double sX, double sY, double size)
        : GhostRacerActivatedObject(thisWorld, IID_OIL_SLICK, STARTDIRECTION_OIL, sX, sY, size)
    {
        setisAffected(false);
    }

    void doActivity();


private:

};


class HealingGoodie : public GhostRacerActivatedObject
{
public:
    HealingGoodie(StudentWorld* thisWorld, double sX, double sY)
        : GhostRacerActivatedObject(thisWorld, IID_HEAL_GOODIE, 0, sX, sY, 1) {}

    void doActivity();


private:

};

class HolyWaterGoodie : public GhostRacerActivatedObject
{
public:
    HolyWaterGoodie(StudentWorld* thisWorld, double sX, double sY)
        : GhostRacerActivatedObject(thisWorld, IID_HOLY_WATER_GOODIE, 90, sX, sY, 2) {}

    void doActivity();


private:

};


class SoulGoodie : public GhostRacerActivatedObject
{
public:
    SoulGoodie(StudentWorld* thisWorld, double sX, double sY)
        : GhostRacerActivatedObject(thisWorld, IID_SOUL_GOODIE, 0, sX, sY, 4)
    {
        setisAffected(false);
    }

    void doActivity();
    void rotateClock();


private:

};



class Pedestrian : public Actor
{
public:
    Pedestrian(StudentWorld* thisWorld, int imageID, double sX, double sY, double size)
        : Actor(thisWorld, imageID, true, 0, sX, sY, size, 0, 0, -4, 2, true), m_movementPlan(0) {}
    virtual void doSomething() = 0;
    int getmovementPlan(){ return m_movementPlan;}
    void setmovementPlan(int temp){ m_movementPlan = temp;}
    void newmovementPlan();
    
    virtual void playDeath() {}
    virtual void playHurt() {}
    

private:
    int m_movementPlan;
};

class HumanPed : public Pedestrian
{
public:
    HumanPed(StudentWorld* thisWorld,double sX, double sY)
        : Pedestrian(thisWorld, IID_HUMAN_PED, sX, sY, 2) {}
    void doSomething();
    bool beSprayedIfAppropriate();

private:
    
};


class ZombiePed : public Pedestrian
{
public:
    ZombiePed(StudentWorld* thisWorld,double sX, double sY)
        : Pedestrian(thisWorld, IID_ZOMBIE_PED, sX, sY, 3), m_grunt(0) {}
    void doSomething();
    
    int getGrunt(){ return m_grunt;}
    void setGrunt(int temp){ m_grunt = temp;}
    
    void playDeath();
    void playHurt();

private:
    int m_grunt;
};


class ZombieCab : public Actor
{
public:
    ZombieCab(StudentWorld* thisWorld,double sX, double sY, double ySpeed, int currLane)
        : Actor(thisWorld, IID_ZOMBIE_CAB, true, 90,  sX, sY, 4, 0, 0, ySpeed, 3, true),m_damagedGhostRider(false), m_planLength(0), m_currLane(currLane)  {}
    void doSomething();
    
    bool getdamagedGhostRider(){ return m_damagedGhostRider;}
    void setdamagedGhostRider(bool temp){ m_damagedGhostRider = temp;}
    
    int getplanLength(){ return m_planLength;}
    void setplanLength(int temp){ m_planLength = temp;}
    
    int getcurrLane(){ return m_currLane;}

    void playDeath();
    void playHurt();

    
    

private:
    bool m_damagedGhostRider;
    int m_planLength;
    int m_currLane;
};


class HolyWaterSpray : public Actor
{
public:
    HolyWaterSpray(StudentWorld* thisWorld,double sX, double sY, int dir)
        : Actor(thisWorld, IID_HOLY_WATER_PROJECTILE, true, dir,  sX, sY, 1, 1, 0, 0, 0, false),m_travelDistance(160)  {}
    void doSomething();
    
    int gettravelDistance(){ return m_travelDistance;}
    void settravelDistance(int temp){ m_travelDistance = temp;}

private:
    int m_travelDistance;
};


#endif // ACTOR_H_
//*/
