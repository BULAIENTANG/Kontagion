#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
using namespace std;

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor : public GraphObject
{
public:
    Actor(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    StudentWorld* getWorld() const;
    bool getLiveOrDeath() const;
    virtual void takeDamage(int amt);
    void setDeath();
    virtual bool isBlocker();
    virtual bool isConsumable();
    virtual bool isDamageable();
    virtual bool isOverlappable();
    virtual void doSomething() = 0;
    virtual ~Actor();
    virtual void getPositionInThisDirection(Direction angle, int units, double &dx, double &dy) const;
private:
    bool          m_isAlive;
    StudentWorld* m_world;
};


/* ********* Player ********** */

class Socrates : public Actor
{
public:
    Socrates(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    void setHealth(int health);
    int  getHealth() const;
    void setSpray(int amt);
    int  getSpray() const;
    void setFlame(int amt);
    int  getFlame() const;
    Direction getPosAngle() const;
    void setPosAngle(Direction angle);
    virtual void takeDamage(int amt);
    virtual ~Socrates();
    virtual void doSomething();
private:
    int m_health;
    int m_nspray;
    int m_nflame;
    Direction m_posAngle;
};




/* ********* Other Items ********** */

class Dirt : public Actor
{
public:
    Dirt(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Dirt();
    virtual void takeDamage(int amt);
    virtual bool isBlocker();
    virtual bool isDamageable();
    virtual void doSomething();    
};

class Food : public Actor
{
public:
    Food(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Food();
    virtual bool isConsumable();
    virtual bool isOverlappable();
    virtual void doSomething();
};

class Pit : public Actor
{
public:
    Pit(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Pit();
    virtual bool isOverlappable();
    virtual void doSomething();
private:
    int m_nRegSal;
    int m_nAggSal;
    int m_nEcoli;
};



/* ********* Projectiles ********** */

class Projectile : public Actor
{
public:
    Projectile(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Projectile();
    double getMaxDist() const;
    double getDistMoved() const;
    void setMaxDist(int MaxDist);
    void setDistMoved(int distMoved);
    virtual void doSomething() = 0;
    virtual void takeDamage(int amt);
private:
    double m_maxDist;
    double m_distMoved;
};

class Flame : public Projectile
{
public:
    Flame(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Flame();
    virtual void doSomething();
};

class Spray : public Projectile
{
public:
    Spray(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Spray();
    virtual void doSomething();
};








/* ********* Goodies ********** */

class Goodie : public Actor
{
public:
    Goodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Goodie();
    void decLifetime();
    int getLifetime() const;
    virtual bool isDamageable();
    virtual void doSomething() = 0;
    virtual void takeDamage(int amt);
private:
    int lifetime;
};

class RestoreHpGoodie : public Goodie
{
public:
    RestoreHpGoodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~RestoreHpGoodie();
    virtual void doSomething();
};

class FlameGoodie : public Goodie
{
public:
    FlameGoodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~FlameGoodie();
    virtual void doSomething();
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~ExtraLifeGoodie();
    virtual void doSomething();
};

class Fungus : public Goodie
{
public:
    Fungus(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Fungus();
    virtual void doSomething();
};









/* ********* Bacteria ********** */

class Bacteria : public Actor
{
public:
    Bacteria(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    int getHealth() const;
    void setHealth(int health);
    int getFoodEaten() const;
    void setFoodEaten(int amt);
    int getMovePlanDist() const;
    void setMovePlanDist(int num);
    virtual void takeDamage(int amt);
    void findFood(Bacteria* b);
    void generateNewBac(Bacteria* b, string type);
    virtual ~Bacteria();
    virtual void doSomething() = 0;
    virtual bool isDamageable();

private:
    int m_health;
    int m_movePlanDist;
    int m_nFoodEaten;
};

class Salmonella : public Bacteria
{
public:
    Salmonella(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Salmonella();
    virtual void takeDamage(int amt);
    void nextMove(Salmonella* b); //step 5
    virtual void doSomething() = 0;
};

class RegSal : public Salmonella
{
public:
    RegSal(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~RegSal();
    virtual void doSomething();
};

class AggSal : public Salmonella
{
public:
    AggSal(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~AggSal();
    virtual void doSomething();
};

class Ecoli : public Bacteria
{
public:
    Ecoli(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth);
    virtual ~Ecoli();
    virtual void takeDamage(int amt);
    virtual void doSomething();
    void nextMove(Ecoli* b);
};
#endif // ACTOR_H_

