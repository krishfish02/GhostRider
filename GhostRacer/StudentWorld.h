#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Actor.h"
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


const int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
const int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    ~StudentWorld();
    GhostRacer* getGhostRacer() { return m_GhostRacer; }
    
    int getsoulsLeft(){return m_soulsLeft;}
    void setsoulsLeft(int souls){m_soulsLeft = souls;}
    
    bool gethitPed(){return m_hitPed;}
    void sethitPed(bool hit){m_hitPed = hit;}
    
    int getBonus(){return m_bonus;}

    void addActor(Actor* add){m_actors.push_back(add);}
    
    
    bool sprayFirstAppropriateActor(Actor* a);
    
    void checkLane(Actor* actor, bool& worthy, double& colltopY, double& collbotY, int cur_lane);
    void checkFrontBack(Actor* Cab, Actor* actor, bool& worthyback, bool& worthyfront, double& collbackY, double& collfrontY, int cur_lane);
    void checkFrontBackLanes(Actor* a, bool& worthyback, bool& worthyfront, double& collbackY, double& collfrontY, int cur_lane);
    


private:
    std::vector<Actor*> m_actors;
    GhostRacer* m_GhostRacer;
    int last;
    
    int m_soulsLeft;
    bool m_hitPed;
    
    int m_bonus;
};

#endif // STUDENTWORLD_H_
