#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isOverlap(const Actor* a, const Actor* b, double dist);
    void addFlame(const Socrates* a);
    void addSpray(const Socrates* a);
    void addBacteria(string type, double x, double y);
    void addFood(const Actor* a);
    void AnotherBacKilled();
    void NewBacCreated();
    Direction getDirectionTowards(const Actor* a, const Actor* b);
    Actor* getDamageableObject(const Actor* a);
    bool isBlocked(const Actor* a, Direction dir, double dist);
    Actor* getFoodAround(const Actor* a, double dist);
    Socrates* getPlayer();
    

private:
    vector<Actor*> m_actors;
    Socrates* m_player;
    int m_nOfBacRemaining;
};

#endif // STUDENTWORLD_H_
