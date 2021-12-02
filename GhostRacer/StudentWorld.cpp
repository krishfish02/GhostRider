#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_GhostRacer = nullptr;
    last = 0;
    m_soulsLeft = 2 * getLevel() + 5;
    m_hitPed = false;
    m_bonus = 5000;
}

int StudentWorld::init()
{
    m_GhostRacer =  new GhostRacer(this);
    m_soulsLeft = 2 * getLevel() + 5;
    m_hitPed = false;
    m_bonus = 5000;
    
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    
    for(int j = 0; j<N; j++)
    {
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, j*SPRITE_HEIGHT));
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j*SPRITE_HEIGHT));
    }
    
    
    int M = VIEW_HEIGHT / (4*SPRITE_HEIGHT);
    
    for(int j = 0; j<M; j++)
    {
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, j* (4*SPRITE_HEIGHT)));
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, j*(4*SPRITE_HEIGHT)));
        last = j*(4*SPRITE_HEIGHT);
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    if(m_bonus > 0)
        m_bonus--;
    
    m_GhostRacer->doSomething();

    vector<Actor*>::iterator iter;
    for (iter = m_actors.begin(); iter != m_actors.end(); iter++)
    {
            (*iter)->doSomething();
        if(!m_GhostRacer->getisAlive())
        {
            return GWSTATUS_PLAYER_DIED;
        }
        
        if(gethitPed())
        {
            return GWSTATUS_PLAYER_DIED;
        }
        
        if(getsoulsLeft() == 0 )
        {
            increaseScore(getBonus());
            return GWSTATUS_FINISHED_LEVEL;
        }
    }
    
    
   
    for (iter = m_actors.begin(); iter != m_actors.end();)
    {
        bool alive = (*iter)->getisAlive();
        if (!alive)
        {
            delete *iter;
            iter = m_actors.erase(iter);
        }
        else
            iter++;
    }
    
    
    
    
    
    last = last - 4 - getGhostRacer()->getySpeed();
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    int delta_y = new_border_y - last;
    
    
    
    if (delta_y >= SPRITE_HEIGHT)
    {
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y));
        m_actors.push_back(new BorderLine(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y));
    }
    
    if (delta_y >= 4*SPRITE_HEIGHT)
    {
        
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH/3, new_border_y));
        m_actors.push_back(new BorderLine(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH/3, new_border_y));
        last = new_border_y;
    }
    
    
    
    
    int ChanceOilSlick = max(150-getLevel() * 10, 40);
    int rand = randInt(0,ChanceOilSlick);
    int size = randInt(2,5);
    int randx = randInt(LEFT_EDGE,RIGHT_EDGE);
    if(rand == 0)
    {
        m_actors.push_back(new OilSlick(this, randx , VIEW_HEIGHT, size));
    }
    
    
    
    
    int ChanceOfHolyWater = 100+10*getLevel();
    rand = randInt(0,ChanceOfHolyWater);
    randx = randInt(LEFT_EDGE,RIGHT_EDGE);
    if(rand == 0)
    {
        m_actors.push_back(new HolyWaterGoodie(this, randx , VIEW_HEIGHT));
    }
    
    
    int ChanceOfLostSoul = 100;
    rand = randInt(0,ChanceOfLostSoul);
    randx = randInt(LEFT_EDGE,RIGHT_EDGE);
    if(rand == 0)
    {
        m_actors.push_back(new SoulGoodie(this, randx , VIEW_HEIGHT));
    }
    
    
    
    int ChanceZombiePed = max(100-getLevel()*10,20);
    rand = randInt(0,ChanceZombiePed);
    randx = randInt(0,VIEW_WIDTH);
    if(rand == 0)
    {
        m_actors.push_back(new ZombiePed(this, randx , VIEW_HEIGHT));
    }
    
    
    int ChanceHumanPed = max(200-getLevel()*10,30);
    rand = randInt(0,ChanceHumanPed);
    randx = randInt(0,VIEW_WIDTH);
    if(rand == 0)
    {
        m_actors.push_back(new HumanPed(this, randx , VIEW_HEIGHT));
    }

        
    int ChanceVehicle = max(100-getLevel()*10, 20);
    rand = randInt(0, ChanceVehicle);
    int x =0;
    if(rand == 0)
    {
        
        int cur_lane = randInt(1,3);
        for(int i = 0; i<3; i++)
        {
            
            bool worthy = false;
            double collbotY = 257;
            double colltopY = -1;
            
            
            if(cur_lane == 1)
                x= ROAD_CENTER - ROAD_WIDTH/3;
            if(cur_lane == 2)
                x= ROAD_CENTER;
            if(cur_lane == 3)
                x= ROAD_CENTER + ROAD_WIDTH/3;

            
            for(int i = 0; i<m_actors.size(); i++)
            {
                
                if(m_actors[i]->getworthy())
                {
                    
                    switch(cur_lane)
                    {
                        case 1:
                        {
                            checkLane(m_actors[i], worthy, colltopY, collbotY, 1);
                            break;
                        }
                            
                        case 2:
                        {
                            checkLane(m_actors[i], worthy, colltopY, collbotY, 2);
                            break;
                        }
                            
                        case 3:
                        {
                            checkLane(m_actors[i], worthy, colltopY, collbotY, 3);
                            break;
                        }
                            
                        default:
                            break;
                    }
                    
                }
            }
            
            
            switch(cur_lane)
            {
                case 1:
                {
                    checkLane(m_GhostRacer, worthy, colltopY, collbotY, 1);
                    break;
                }
                    
                case 2:
                {
                    checkLane(m_GhostRacer, worthy, colltopY, collbotY, 2);
                    break;
                }
                    
                case 3:
                {
                    checkLane(m_GhostRacer, worthy, colltopY, collbotY, 3);
                    break;
                }
                    
                default:
                    break;
            }
            
            
          if(!worthy || collbotY > (VIEW_HEIGHT/3))
            {
                double yspeed = m_GhostRacer->getySpeed() + randInt(2,4);
                m_actors.push_back(new ZombieCab(this, x, SPRITE_HEIGHT/2,yspeed, cur_lane));
                break;
            }
            else if(!worthy || colltopY < (VIEW_HEIGHT*2/3))
            {
                double yspeed = m_GhostRacer->getySpeed() - randInt(2,4);
                m_actors.push_back(new ZombieCab(this, x, VIEW_HEIGHT - SPRITE_HEIGHT/2,yspeed, cur_lane));
                break;
            }
            else
            {
                cur_lane++;
                if(cur_lane == 4)
                    cur_lane = 1;
            }
                
    
        }
        
    }
    
    
    
    ostringstream oss;

    oss << "Score: ";
    oss << getScore() << "  ";
    
    oss << "Lvl: ";
    oss << getLevel() << "  ";
    
    oss << "Souls2Save: ";
    oss << getsoulsLeft() << "  ";
    
    oss << "Lives: ";
    oss << getLives() << "  ";
    
    oss << "Health: ";
    oss << getGhostRacer()->gethitPoints() << "  ";
    
    oss << "Sprays: ";
    oss << getGhostRacer()->getholyWater() << "  ";
    
    oss << "Bonus: ";
    oss << getBonus() << endl;
    
    setGameStatText(oss.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete m_GhostRacer;
    vector<Actor*>::iterator iter;
    for (iter = m_actors.begin(); iter != m_actors.end(); iter++)
    {
        delete *iter;
    }
    m_actors.clear();
    
}


StudentWorld::~StudentWorld()
{
    cleanUp();
}

bool StudentWorld::sprayFirstAppropriateActor(Actor* a)
{
    for(int i = 0; i<m_actors.size(); i++)
    {
        if(a->overlap(m_actors[i]) && m_actors[i]->getisAffected())
        {
            m_actors[i]->beSprayedIfAppropriate();
            a->setisAlive(false);
            return true;
        }
    }

    return false;
    
}

void StudentWorld::checkLane(Actor* actor, bool& worthy, double& colltopY, double& collbotY, int cur_lane)
{
    if(actor->getX() >= ROAD_CENTER - ROAD_WIDTH/2 + ((cur_lane-1)*ROAD_WIDTH/3) && actor->getX() < ROAD_CENTER - ROAD_WIDTH/2 + (cur_lane* ROAD_WIDTH/3))
    {
        worthy = true;
        if( actor->getY()< collbotY)
            collbotY = actor->getY();
        
        if( actor->getY()> colltopY)
            colltopY = actor->getY();
    }
    
}






//to avoid returning a whole container list into actor.cpp


void StudentWorld::checkFrontBackLanes(Actor* a, bool& worthyback, bool& worthyfront, double& collbackY, double& collfrontY, int cur_lane)
{
    for(int i = 0; i<m_actors.size(); i++)
    {
        
        if(m_actors[i]->getworthy())
        {
            
            switch(cur_lane)
            {
                case 1:
                    checkFrontBack(a,m_actors[i], worthyback, worthyfront, collbackY, collfrontY, 1);
                    break;
                    
                case 2:
                    checkFrontBack(a,m_actors[i], worthyback, worthyfront, collbackY, collfrontY, 2);
                    break;
                    
                case 3:
                    checkFrontBack(a,m_actors[i], worthyback, worthyfront, collbackY, collfrontY, 3);
                    break;
                    
                default:
                    break;
            }
            
        }
    }
}

void StudentWorld::checkFrontBack(Actor* Cab, Actor* actor, bool& worthyback, bool& worthyfront, double& collbackY, double& collfrontY, int cur_lane)
{
    if(actor->getX() >= ROAD_CENTER - ROAD_WIDTH/2 + ((cur_lane-1)*ROAD_WIDTH/3) && actor->getX() < ROAD_CENTER - ROAD_WIDTH/2 + (cur_lane* ROAD_WIDTH/3) && actor->getY() > Cab->getY())
    {
        worthyfront = true;
        if( actor->getY()< collfrontY)
            collfrontY = actor->getY();
    }
    else if(actor->getX() >= ROAD_CENTER - ROAD_WIDTH/2 + ((cur_lane-1)*ROAD_WIDTH/3) && actor->getX() < ROAD_CENTER - ROAD_WIDTH/2 + (cur_lane* ROAD_WIDTH/3) && actor->getY() < Cab->getY())
    {
        worthyback = true;
        if( actor->getY()> collbackY)
            collbackY = actor->getY();
        
    }
}
