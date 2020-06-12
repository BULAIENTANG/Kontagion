#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: GraphObject(imageID, startX, startY, dir, depth)
{
    m_isAlive = true;
    m_world = studWorld;
}

StudentWorld* Actor::getWorld() const
{return m_world;}

void Actor::setDeath()
{m_isAlive = false;}

void Actor::takeDamage(int amt)
{return;}

bool Actor::getLiveOrDeath() const
{return m_isAlive;}

bool Actor::isBlocker()
{return false;}

bool Actor::isConsumable()
{return false;}

bool Actor::isDamageable()
{return false;}

bool Actor::isOverlappable()
{return true;}

void Actor::getPositionInThisDirection(Direction angle, int units, double &dx, double &dy) const
{
    const double PI = 4 * atan(1);
    dx = (getX() + units * cos(angle*1.0 / 360 * 2 * PI));
    dy = (getY() + units * sin(angle*1.0 / 360 * 2 * PI));
}

Actor::~Actor(){}

/*implementation for Socrates*/

Socrates::Socrates(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth)
{
    setDirection(0);
    setHealth(100);
    m_nspray = 20;
    m_nflame = 5;
    m_posAngle = 180;
}

Socrates::~Socrates(){}

void Socrates::setHealth(int health)
{m_health = health;}

int Socrates::getHealth() const
{return m_health;}

void Socrates::setSpray(int amt)
{m_nspray += amt;}

int Socrates::getSpray() const
{return m_nspray;}

void Socrates::setFlame(int amt)
{m_nflame += amt;}

int Socrates::getFlame() const
{return m_nflame;}

Direction Socrates::getPosAngle() const
{return m_posAngle;}

void Socrates::setPosAngle(Direction angle)
{m_posAngle += angle;}

void Socrates::takeDamage(int amt)
{
    m_health += amt;
    if(m_health <= 0)
    {
        setDeath();
        getWorld()->playSound(SOUND_PLAYER_DIE);
    }
    else
        getWorld()->playSound(SOUND_PLAYER_HURT);
}

void Socrates::doSomething()
{
    if(!getLiveOrDeath()) // if player is dead
        return;
    
    int key;
    if(getWorld()->getKey(key))
    {
        const double PI = 4 * atan(1);
    // user hit a key during this tick!
        switch (key)
        {
            case KEY_PRESS_LEFT:
            {
                setPosAngle(5);
                Direction angle = getPosAngle();
                double newX = cos(angle*1.0/180 * PI)*VIEW_RADIUS + VIEW_RADIUS;
                double newY = sin(angle*1.0/180 * PI)*VIEW_RADIUS + VIEW_RADIUS;
                moveTo(newX, newY);
                setDirection(getPosAngle()+180);
                //... move Socrates counterclockwise ...;
                break;
            }
            case KEY_PRESS_RIGHT:
            {
                setPosAngle(-5);
                Direction angle = getPosAngle();
                double newX = cos(angle*1.0/180 * PI)*VIEW_RADIUS + VIEW_RADIUS;
                double newY = sin(angle*1.0/180 * PI)*VIEW_RADIUS + VIEW_RADIUS;
                moveTo(newX, newY);
                setDirection(getPosAngle()+180);
                //... move Socrates clockwise...;
                break;
            }
            case KEY_PRESS_SPACE:
            //... add spray in front of Socrates...;
            {
                if(m_nspray > 0)
                {
                    getWorld()->addSpray(this);
                    
                    setSpray(-1);
                getWorld()->playSound(SOUND_PLAYER_SPRAY);
                }
                break;
            }
            case KEY_PRESS_ENTER:
            //... add flame in front of Socrates...;
            {
                if(m_nflame > 0)
                {
                    getWorld()->addFlame(this);
                    
                    setFlame(-1);
                getWorld()->playSound(SOUND_PLAYER_FIRE);
                }
                break;
            }
        }
    }
    else if(getSpray() < 20)
        setSpray(1);
}

/*implementation for Pit*/

Pit::Pit(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth), m_nRegSal(5), m_nAggSal(3), m_nEcoli(2)
{}

Pit::~Pit(){}

bool Pit::isOverlappable()
{return false;}

void Pit::doSomething()
{
    if(m_nRegSal == 0 && m_nEcoli == 0 && m_nAggSal == 0)
        setDeath();
    else
    {
        if(randInt(1, 50) == 50)
        {
            if(m_nRegSal > 0 && m_nAggSal == 0 && m_nEcoli == 0)
            {getWorld()->addBacteria("RegSal", getX(), getY());
                m_nRegSal--;}
            if(m_nRegSal == 0 && m_nAggSal > 0 && m_nEcoli == 0)
            {getWorld()->addBacteria("AggSal",  getX(), getY());
                m_nAggSal--;}
            if(m_nRegSal == 0 && m_nAggSal == 0 && m_nEcoli > 0)
            {getWorld()->addBacteria("Ecoli",  getX(), getY());
                m_nEcoli--;}
            if(m_nRegSal > 0 && m_nAggSal > 0 && m_nEcoli == 0)
            {
                int num = randInt(1, 2);
                if(num == 1)
                {getWorld()->addBacteria("RegSal", getX(), getY());
                    m_nRegSal--;}
                if(num == 2)
                {getWorld()->addBacteria("AggSal",  getX(), getY());
                    m_nAggSal--;}
            }
            if(m_nRegSal > 0 && m_nAggSal == 0 && m_nEcoli > 0)
            {
                int num = randInt(1, 2);
                if(num == 1)
                {getWorld()->addBacteria("RegSal", getX(), getY());
                    m_nRegSal--;}
                if(num == 2)
                {getWorld()->addBacteria("Ecoli",  getX(), getY());
                    m_nEcoli--;}
            }
            if(m_nRegSal == 0 && m_nAggSal > 0 && m_nEcoli > 0)
            {
                int num = randInt(2, 3);
                if(num == 2)
                {getWorld()->addBacteria("AggSal",  getX(), getY());
                    m_nAggSal--;}
                if(num == 3)
                {getWorld()->addBacteria("Ecoli",  getX(), getY());
                    m_nEcoli--;}
            }
            if(m_nRegSal > 0 && m_nAggSal > 0 && m_nEcoli > 0)
            {
                int num = randInt(1, 3);
                if(num == 1){
                    getWorld()->addBacteria("RegSal", getX(), getY());
                    m_nRegSal--;}
                if(num == 2)
                {getWorld()->addBacteria("AggSal",  getX(), getY());
                    m_nAggSal--;}
                if(num == 3)
                {getWorld()->addBacteria("Ecoli",  getX(), getY());
                    m_nEcoli--;}
            }
            getWorld()->playSound(SOUND_BACTERIUM_BORN);
        }
    }
}

/*implementation for Dirt*/

Dirt::Dirt(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth)
{}

void Dirt::doSomething(){}

void Dirt::takeDamage(int amt)
{setDeath();}

bool Dirt::isBlocker()
{return true;}

bool Dirt::isDamageable()
{return true;}

Dirt::~Dirt(){}


/*implementation for Food*/

Food::Food(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth)
{}

Food::~Food(){}

void Food::doSomething(){};

bool Food::isConsumable()
{return true;}

bool Food::isOverlappable()
{return false;}

/*implementation for Projectile*/

Projectile::Projectile(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth), m_maxDist(0), m_distMoved(0)
{}

void Projectile::setMaxDist(int MaxDist)
{m_maxDist = MaxDist;}

void Projectile::setDistMoved(int distMoved)
{
    m_distMoved += distMoved;
}

double Projectile::getDistMoved() const
{return m_distMoved;}

double Projectile::getMaxDist() const
{return m_maxDist;}

void Projectile::takeDamage(int amt)
{return;}

Projectile::~Projectile(){}

/*implementation for Flame*/

Flame::Flame(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Projectile(imageID, startX, startY, studWorld, dir, depth)
{
    setMaxDist(32);
}

Flame::~Flame(){}

void Flame::doSomething()
{
    if(!getLiveOrDeath())
        return;
    Actor* a = getWorld()->getDamageableObject(this);
    if(a != nullptr)
    {
        if(!a->getLiveOrDeath()) // if killed by previous flame, break
            return;
        a->takeDamage(-5);
        setDeath();
    }
    else
    {
        if(getDistMoved() < getMaxDist())
            //else move forward 2*sprite_radius in current dir
        {
            moveForward(2*SPRITE_RADIUS);
            setDistMoved(2*SPRITE_RADIUS);
        }
        else
            this->setDeath();
    }
}

/*implementation for Spray*/

Spray::Spray(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Projectile(imageID, startX, startY, studWorld, dir, depth)
{
    setMaxDist(112);
}

Spray::~Spray(){}

void Spray::doSomething()
{
    if(!getLiveOrDeath())
        return;
    Actor* a = getWorld()->getDamageableObject(this);
    if(a != nullptr)
    {
        a->takeDamage(-2);
        setDeath();
    }
    else
    {
        if(getDistMoved() < getMaxDist())
            //else move forward 2*sprite_radius in current dir
        {
            moveForward(2*SPRITE_RADIUS);
            setDistMoved(2*SPRITE_RADIUS);
        }
        else
            this->setDeath();
    }
    
}

/*implementation for Goodie*/

Goodie::Goodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth)
{
    lifetime = max(rand() % (300 - 10 * getWorld()->getLevel()), 50);
}

Goodie::~Goodie(){}

void Goodie::decLifetime()
{lifetime--;}

int Goodie::getLifetime() const
{return lifetime;}

void Goodie::takeDamage(int amt)
{setDeath();}

bool Goodie::isDamageable()
{return true;}

/*implementation for RHG*/

RestoreHpGoodie::RestoreHpGoodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Goodie(imageID, startX, startY, studWorld, dir, depth)
{}

RestoreHpGoodie::~RestoreHpGoodie(){}

void RestoreHpGoodie::doSomething()
{
    if(!getLiveOrDeath())
        return;
    if(getWorld()->isOverlap(this, getWorld()->getPlayer(), 2.0*SPRITE_RADIUS))
    {
        getWorld()->increaseScore(250);
        setDeath();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->setHealth(100);
        return;
    }
    decLifetime();
    if(getLifetime() == 0)
        setDeath();
}

/*implementation for FTG*/

FlameGoodie::FlameGoodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Goodie(imageID, startX, startY, studWorld, dir, depth)
{}

FlameGoodie::~FlameGoodie(){}

void FlameGoodie::doSomething()
{
    if(!getLiveOrDeath())
        return;
    if(getWorld()->isOverlap(this, getWorld()->getPlayer(),2.0 *SPRITE_RADIUS))
    {
        getWorld()->increaseScore(300);
        setDeath();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->getPlayer()->setFlame(5);
        return;
    }
    decLifetime();
    if(getLifetime() == 0)
        setDeath();
}

/*implementation for ELG*/

ExtraLifeGoodie::ExtraLifeGoodie(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Goodie(imageID, startX, startY, studWorld, dir, depth)
{}

ExtraLifeGoodie::~ExtraLifeGoodie(){}

void ExtraLifeGoodie::doSomething()
{
    if(!getLiveOrDeath())
        return;
    if(getWorld()->isOverlap(this, getWorld()->getPlayer(), 2*SPRITE_RADIUS))
    {
        getWorld()->increaseScore(500);
        setDeath();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->incLives();
        return;
    }
    decLifetime();
    if(getLifetime() == 0)
        setDeath();
}

/*implementation for Fungus*/

Fungus::Fungus(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Goodie(imageID, startX, startY, studWorld, dir, depth)
{}

Fungus::~Fungus(){}

void Fungus::doSomething()
{
    if(!getLiveOrDeath())
        return;
    if(getWorld()->isOverlap(this, getWorld()->getPlayer(),2.0 *SPRITE_RADIUS))
    {
        getWorld()->increaseScore(-50);
        setDeath();
        
        getWorld()->getPlayer()->takeDamage(-20);
        return;
    }
    decLifetime();
    if(getLifetime() == 0)
        setDeath();
}


/*implementation for Bacteria*/

Bacteria::Bacteria(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Actor(imageID, startX, startY, studWorld, dir, depth), m_health(0),m_movePlanDist(0),m_nFoodEaten(0)
{}

Bacteria::~Bacteria(){}

int Bacteria::getHealth() const
{return m_health;}

void Bacteria::setHealth(int health)
{m_health = health;}

void Bacteria::takeDamage(int amt)
{}

int Bacteria::getFoodEaten() const
{return m_nFoodEaten;}

void Bacteria::setFoodEaten(int amt)
{m_nFoodEaten = amt;}

void Bacteria::setMovePlanDist(int num)
{m_movePlanDist += num;}

int Bacteria::getMovePlanDist() const
{return m_movePlanDist;}

bool Bacteria::isDamageable()
{return true;}

void Bacteria::findFood(Bacteria* b)
{
    Actor* a = getWorld()->getFoodAround(b, 2*SPRITE_RADIUS);
    if(a != nullptr)
    {
        setFoodEaten(getFoodEaten()+1);
        a->setDeath();
    }
}

void Bacteria::generateNewBac(Bacteria* b, string type)
{
    int newX = b->getX(), newY = b->getY();
    if(b->getX()<VIEW_WIDTH/2)
        newX += SPRITE_RADIUS;
    if(b->getX()>VIEW_WIDTH/2)
        newX -= SPRITE_RADIUS;
    if(b->getY()<VIEW_HEIGHT/2)
        newY += SPRITE_RADIUS;
    if(b->getY()>VIEW_HEIGHT/2)
        newY -= SPRITE_RADIUS;
    b->getWorld()->addBacteria(type, newX, newY);
    b->setFoodEaten(0);
    getWorld()->NewBacCreated();
    getWorld()->playSound(SOUND_BACTERIUM_BORN);
}

/*implementation for Salmonella*/

Salmonella::Salmonella(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Bacteria(imageID, startX, startY, studWorld, dir, depth)
{}

Salmonella::~Salmonella(){}

void Salmonella::takeDamage(int amt)
{
   setHealth(getHealth()+amt);
    if(getHealth() <= 0)
    {
        setDeath();
        getWorld()->playSound(SOUND_SALMONELLA_DIE);
        getWorld()->AnotherBacKilled();
        getWorld()->increaseScore(100);
        if(randInt(0, 1) == 0)
            getWorld()->addFood(this);
    }
    else
        getWorld()->playSound(SOUND_SALMONELLA_HURT);
        
}

void Salmonella::nextMove(Salmonella* b)
{
    if(getMovePlanDist() > 0) //step 5
    {
        setMovePlanDist(-1);
        Direction dir = getDirection();
        double newX, newY;
        b->getPositionInThisDirection(dir, 3, newX, newY);
        double distToCtr = sqrt(pow(newX - VIEW_WIDTH/2, 2) + pow(newY - VIEW_HEIGHT/2, 2));
        if(distToCtr <= VIEW_RADIUS && (!getWorld()->isBlocked(b,dir,3)))
            moveTo(newX, newY);
        else
        {
            int randDir = randInt(0, 359);
            b->setDirection(randDir);
            b->setMovePlanDist(10-getMovePlanDist());
        }
        return;
    }
    else //step 6 : find food if no movement plan
    {
        Actor* a = getWorld()->getFoodAround(b,VIEW_RADIUS);
        if(a != nullptr)
        {
            Direction angleToFood = getWorld()->getDirectionTowards(b, a);
            b->setDirection(angleToFood);
            
            if(getWorld()->isBlocked(this, angleToFood, 3))
            {
                int randDir = randInt(0, 359);
                b->setDirection(randDir);
                b->setMovePlanDist(10-getMovePlanDist());
            }
            else
                moveForward(3);
            return;
        }
        else // no food, go random direction
        {
            int randDir = randInt(0, 359);
            b->setDirection(randDir);
            b->setMovePlanDist(10-getMovePlanDist());
            return;
        }
    }
}

/*implementation for RegSal*/

RegSal::RegSal(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Salmonella(imageID, startX, startY, studWorld, dir, depth)
{
    setHealth(4);
}

RegSal::~RegSal(){}

void RegSal::doSomething()
{
    if(!getLiveOrDeath()) //step 1 : check status
        return;
    if(getWorld()->isOverlap(this, getWorld()->getPlayer(), 2*SPRITE_RADIUS)) // step 2 : find overlap with player
    {
        getWorld()->getPlayer()->takeDamage(-1);
        nextMove(this); //step 5/6
    }
    if(getFoodEaten() == 3)
    {
        generateNewBac(this, "RegSal"); //step 3
        nextMove(this); //skip to step 5 : move forward
    }
    // step 4 : find food
    findFood(this);
    nextMove(this);
}


/*implementation for AggSal*/

AggSal::AggSal(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Salmonella(imageID, startX, startY, studWorld, dir, depth)
{
    setHealth(10);
}

AggSal::~AggSal(){}

void AggSal::doSomething()
{
    if(!getLiveOrDeath()) // step 1
        return;
    
    Socrates* player = getWorld()->getPlayer();
    
    if(getWorld()->isOverlap(this, player,72)) // step 2
    {
        Direction dir = getWorld()->getDirectionTowards(this, player);
        setDirection(dir);

        if(!getWorld()->isBlocked(this, dir, 3))
            moveForward(3);

        if(getWorld()->isOverlap(this, player, 2*SPRITE_RADIUS)) // step 3
        {
            getWorld()->getPlayer()->takeDamage(-2);
            return;
        }
        if(getFoodEaten() == 3) // step 4 : split
        {
            generateNewBac(this, "AggSal");
            return;
        }
        findFood(this); // step 5 : eat food
        
        return; // immediately return
    }
    else // if DistToPlayer > 72
    {
        if(getWorld()->isOverlap(this, player, 2.0*SPRITE_RADIUS)) // step 3
        {
            getWorld()->getPlayer()->takeDamage(-2);
            nextMove(this); //skip to step 6
        }
        if(getFoodEaten() == 3) // step 4 : split
        {
            generateNewBac(this, "AggSal"); //step 3
            nextMove(this); //skip to step 6
        }
        // step 5 : find food
        findFood(this);
        // step 6 / 7
        nextMove(this);
    }
}


/*implementation for Ecoli*/

Ecoli::Ecoli(int imageID, double startX, double startY, StudentWorld* studWorld, int dir, int depth)
: Bacteria(imageID, startX, startY, studWorld, dir, depth)
{
    setHealth(5);
}

Ecoli::~Ecoli(){}

void Ecoli::takeDamage(int amt)
{
   setHealth(getHealth()+amt);
    if(getHealth() <= 0)
    {
        setDeath();
        getWorld()->playSound(SOUND_ECOLI_DIE);
        getWorld()->increaseScore(100);
        getWorld()->AnotherBacKilled();
        if(randInt(0, 1) == 0)
            getWorld()->addFood(this);
    }
    else
        getWorld()->playSound(SOUND_ECOLI_HURT);
        
}

void Ecoli::nextMove(Ecoli* b)
{
    Socrates* player = getWorld()->getPlayer();
    if(getWorld()->isOverlap(b, player, 2*VIEW_RADIUS))
    {
        Direction dir = getWorld()->getDirectionTowards(b, player);
        b->setDirection(dir);
        for(int i = 0; i < 10; i++)
        {
            if(!getWorld()->isBlocked(b, dir, 2))
            {
                moveForward(2);
                return;
            }
            else
            {
                dir += 50;
                dir = dir % 360;
            }
        }
        return;
    }
}

void Ecoli::doSomething()
{
    if(!getLiveOrDeath())
        return;
    Socrates* player  = getWorld()->getPlayer();
    if(getWorld()->isOverlap(this, player, SPRITE_WIDTH))
    {
        player->takeDamage(-4);
        nextMove(this);
    }
    if(getFoodEaten() == 3)
    {
        //step 3
        generateNewBac(this, "Ecoli");
        nextMove(this); //skip to step 5
    }
    findFood(this); // step 4
    Ecoli::nextMove(this);
}
