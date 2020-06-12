#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
using namespace std;

class Actor;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
:GameWorld(assetPath)
{}

int StudentWorld::init()
{
    m_nOfBacRemaining = 10 * getLevel();
    //create pits
    for(int i = 0; i < getLevel(); i++)
    {
        double startX = randInt(0, VIEW_WIDTH);
        double startY = randInt(0, VIEW_HEIGHT);
        double distFromCtr = sqrt(pow(startX - VIEW_WIDTH/2, 2) + pow(startY - VIEW_HEIGHT/2, 2));
        if(distFromCtr > 120) // remove those distance over 120 pixel from center
        {
            i--;
            continue;
        }
        Pit* newPit = new Pit(IID_PIT,startX,startY,this,0,1);
        bool noOverlapPits = true;
        vector<Actor*>::iterator it;
        for(it = m_actors.begin(); it!=m_actors.end(); it++)
        {
            if((!(*it)->isOverlappable()) && isOverlap(*it, newPit, SPRITE_WIDTH))
            {
                noOverlapPits = false;
                delete newPit;
                break;
            }
        }
        if(noOverlapPits)
            m_actors.push_back(newPit);
        else
            i--;
    }
    //create food
    for(int i = 0; i < min(5*getLevel(), 25); i++)
    {
        double startX = randInt(0, VIEW_WIDTH);
        double startY = randInt(0, VIEW_HEIGHT);
        double distFromCtr = sqrt(pow(startX - VIEW_WIDTH/2, 2) + pow(startY - VIEW_HEIGHT/2, 2));
        if(distFromCtr > 120) // remove those distance over 120 pixel from center
        {
            i--;
            continue;
        }
        Food* newFood = new Food(IID_FOOD,startX, startY, this, 90, 1);
        bool noOverlap = true;
        vector<Actor*>::iterator it;
        for(it = m_actors.begin(); it!=m_actors.end(); it++)
        {
            if((!(*it)->isOverlappable()) && isOverlap(*it, newFood, 2.0*SPRITE_RADIUS))
            {
                noOverlap = false;
                delete newFood;
                break;
            }
        }
        if(noOverlap)
            m_actors.push_back(newFood);
        else
            i--;
    }
    
    // create dirt
    for(int i = 0; i < max(180-20* getLevel(), 20); i++)
    {
        double startX = randInt(0, VIEW_WIDTH);
        double startY = randInt(0, VIEW_HEIGHT);
        double distFromCtr = sqrt(pow(startX - VIEW_WIDTH/2, 2) + pow(startY - VIEW_HEIGHT/2, 2));
        if(distFromCtr > 120) // remove those distance over 120 pixel from center
        {
            i--;
            continue;
        }
        Dirt* newDirt = new Dirt(IID_DIRT,startX, startY, this, 0, 1);
        bool noOverlapPits = true;
        vector<Actor*>::iterator it;
        for(it = m_actors.begin(); it!=m_actors.end(); it++)
        {
            if((!(*it)->isOverlappable()) && isOverlap(*it, newDirt, 2.0*SPRITE_RADIUS))
            {
                noOverlapPits = false;
                delete newDirt;
                break;
            }
        }
        if(noOverlapPits)
            m_actors.push_back(newDirt);
        else
        {
            i--;
        }
    }
    
    // create Socrate player
    m_player = new Socrates(0,0,VIEW_HEIGHT/2,this,0,0);
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    //player move
    if(m_player->getLiveOrDeath())
    {
        m_player->doSomething();
        if(!m_player->getLiveOrDeath())
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if(m_nOfBacRemaining <= 0)
            return GWSTATUS_FINISHED_LEVEL;
    }
    //actor move(dirt/bacteria)
    vector<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->getLiveOrDeath())
        {
            (*it)->doSomething();
            if(!m_player->getLiveOrDeath())
            {
                decLives();
                return GWSTATUS_PLAYER_DIED;
            }
            if(m_nOfBacRemaining <= 0)
            {
                playSound(SOUND_FINISHED_LEVEL);
                return GWSTATUS_FINISHED_LEVEL;
            }
        }
    }

    //delete dead actors
    for (it = m_actors.begin(); it != m_actors.end(); )
    {
        if(!(*it)->getLiveOrDeath()) //if actor is dead
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }

    //potentially add fungi
    int chanceFungus = max(510- 10*getLevel(), 200);
    int randNum = randInt(0, chanceFungus-1);
    int randAngle = randInt(0, 359);
    double startX, startY;
    const double PI = 4 * atan(1);
    startX = VIEW_WIDTH/2 + cos(randAngle*1.0/180 * PI)*VIEW_RADIUS;
    startY = VIEW_HEIGHT/2 + sin(randAngle*1.0/180 * PI)*VIEW_RADIUS;
    if(randNum == 0)
    {
        Fungus* newFungus = new Fungus(IID_FUNGUS,startX,startY,this,0,1);
        m_actors.push_back(newFungus);
    }
    //potentially add goodies
    int chanceGoodie = max(510- 10*getLevel(), 250);
    int randNum1 = randInt(0, chanceGoodie-1);
    int randAngle1 = randInt(0, 359);
    double startX1, startY1;
    startX1 = VIEW_WIDTH/2 + cos(randAngle1*1.0/180 * PI)*VIEW_RADIUS;
    startY1 = VIEW_HEIGHT/2 + sin(randAngle1*1.0/180 * PI)*VIEW_RADIUS;
    if(randNum1 == 0)
    {
        int randNum2 = randInt(1, 10);
        switch (randNum2) {
            case 1:{
                ExtraLifeGoodie* newELG = new ExtraLifeGoodie(IID_EXTRA_LIFE_GOODIE, startX1, startY1, this, 0, 1);
                m_actors.push_back(newELG);
                break;}
            case 2:
            case 3:
            case 4:{
                FlameGoodie* newFTG = new FlameGoodie(IID_FLAME_THROWER_GOODIE, startX1, startY1, this, 0, 1);
                m_actors.push_back(newFTG);
                break;}
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:{
                RestoreHpGoodie* newRHG = new RestoreHpGoodie(IID_RESTORE_HEALTH_GOODIE, startX1, startY1, this, 0, 1);
                m_actors.push_back(newRHG);
                break;}
        }
    }
    
    //display game status
    ostringstream oss;
    oss << "Score: ";
    oss.fill('0');
    if(getScore() >= 0)
        oss << setw(6) << getScore();
    else
        oss << "-" << setw(5) << abs(getScore());
    oss << "  Level: " << getLevel() << "  Lives: " << getLives() << "  health: " << getPlayer()->getHealth() << "  Sprays: " << getPlayer()->getSpray() << "  Flames: " << getPlayer()->getFlame();
    setGameStatText(oss.str());
    
    return GWSTATUS_CONTINUE_GAME;
}

bool StudentWorld::isOverlap(const Actor* a, const Actor* b, double dist)
{
    double distApart = sqrt(pow(a->getX() - b->getX(),2) + pow(a->getY() - b->getY(),2));
    if(distApart <= dist)
        return true;
    return false;
}

void StudentWorld::addBacteria(string type, double x, double y)
{
    if(type == "RegSal")
    {
        RegSal* bacteria = new RegSal(IID_SALMONELLA, x, y, this, 90, 0);
        m_actors.push_back(bacteria);
        return;
    }
    if(type == "AggSal")
    {
        AggSal* bacteria = new AggSal(IID_SALMONELLA, x, y, this, 90, 0);
        m_actors.push_back(bacteria);
        return;
    }
    if(type == "Ecoli")
    {
        Ecoli* bacteria = new Ecoli(IID_ECOLI, x, y, this, 90, 0);
        m_actors.push_back(bacteria);
        return;
    }
}

void StudentWorld::addFlame(const Socrates* a)
{
    Direction d = a->getDirection();
    double newX, newY;
    
    for(int i = 0; i < 16; i++)
    {
        a->getPositionInThisDirection(d, 2*SPRITE_RADIUS, newX, newY);
        Flame* newFlame = new Flame(IID_FLAME,newX,newY,this,d,1);
        m_actors.push_back(newFlame);
        d = (d + 22) % 360;
    }
}

void StudentWorld::addSpray(const Socrates* a)
{
    Direction d = a->getDirection();
    double newX, newY;
    a->getPositionInThisDirection(d, 2*SPRITE_RADIUS, newX, newY);
//    RegSal* newSpray = new RegSal(IID_SALMONELLA,newX,newY,this,d,1);
    Spray* newSpray = new Spray(IID_SPRAY,newX,newY,this,d,1);
    m_actors.push_back(newSpray);
}

void StudentWorld::addFood(const Actor* a)
{
    Food* newFood = new Food(IID_FOOD,a->getX(),a->getY(),this,90,1);
    m_actors.push_back(newFood);
}

void StudentWorld::AnotherBacKilled()
{
    m_nOfBacRemaining--;
}

void StudentWorld::NewBacCreated()
{
    m_nOfBacRemaining++;
}

void StudentWorld::cleanUp()
{
    // deallocate the memory for player
    delete m_player;
    m_player = nullptr;
    // deallocate the memory for vector of Actor pointer
    vector<Actor*>::iterator it;
    for (it = m_actors.begin(); it != m_actors.end(); )
    {
        delete *it;
        it = m_actors.erase(it);
    }
    m_actors.clear();
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

Direction StudentWorld::getDirectionTowards(const Actor* a, const Actor *b)
{
    const double PI = 4 * atan(1);
    double deltaX = b->getX() - a->getX();
    double deltaY = b->getY() - a->getY();
    Direction dir = atan(deltaY/deltaX) * 180 / PI;
    if(deltaX == 0 && deltaY > 0)
        return 90;
    if(deltaX == 0 && deltaY < 0)
        return 270;
    if(deltaX > 0 && deltaY > 0)
        return dir;
    if((deltaX < 0 && deltaY > 0) || (deltaX < 0 && deltaY < 0))
        return (180 + dir);
    if(deltaX > 0 && deltaY < 0)
        return (360 + dir);
    else
        return a->getDirection();
}

Actor* StudentWorld::getDamageableObject(const Actor* a)
{
    vector<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if(isOverlap(a, (*it), SPRITE_WIDTH) && (*it)->isDamageable())
            return (*it);
    }
    return nullptr;
}

bool StudentWorld::isBlocked(const Actor* a, Direction dir, double dist)
{
    double distMoved = 1;
    while(distMoved <= dist)
    {
        double newX, newY;
        a->getPositionInThisDirection(dir, distMoved, newX, newY);
        
        vector<Actor*>::iterator it;
        for(it = m_actors.begin(); it != m_actors.end(); it++)
        {
            double distApart = sqrt(pow(newX - (*it)->getX(), 2) + pow(newY - (*it)->getY(), 2));
            if((*it)->isBlocker() && distApart <= SPRITE_WIDTH/2)
                return true;
        }
        distMoved++;
    }
    return false;
}

Actor* StudentWorld::getFoodAround(const Actor* a, double dist)
{
    vector<Actor*>::iterator it;
    for(it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if((*it)->isConsumable() && isOverlap(a, *it, dist))
           return (*it);
    }
    return nullptr;
}

Socrates* StudentWorld::getPlayer()
{
    return m_player;
}


