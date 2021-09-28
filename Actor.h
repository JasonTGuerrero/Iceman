#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <utility>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;


class Actor : public GraphObject {
  
public:
    Actor(StudentWorld* world, int imageID, int x, int y, GraphObject::Direction dir, double size, unsigned int depth) :
    GraphObject(imageID, x, y, dir, size, depth)
    {
        setVisible(true);
        m_world = world;
        m_alive = true;
    }
    
    virtual void doSomething() = 0;
    
    bool isAlive() { return m_alive; }
    void died() { m_alive = false; }
    
    virtual ~Actor() { }
    StudentWorld* get_world();
    
private:
    StudentWorld* m_world;
protected:
    bool m_alive;
};


class Ice : public Actor {
  
public:
    Ice(StudentWorld* world, int x, int y)
    : Actor(world, IID_ICE, x, y, Direction::right, .25, 3)
    { }
    
    virtual void doSomething() { }
    
    ~Ice() { }
private:
    
};


class Iceman : public Actor {
  
public:
    Iceman(StudentWorld* world)
    : Actor(world, IID_PLAYER, 30, 60, Direction::right, 1.0, 0)
    {
        m_health = 10;
        m_squirts = 5;
        m_nuggets = 0;
        m_sonars = 1;
        m_alive = true;
    }
    
    void update_health(int health) {
        m_health = health;
    }
    
    int health() {
        return m_health;
    }
    
    void update_squirts(int squirts) {
        m_squirts += squirts;
    }
    
    void update_nuggets(int nuggets) {
        m_nuggets += nuggets;
    }
    
    void update_sonars(int sonars) {
        m_sonars += sonars;
    }
    
    int getHealth() { return m_health; }
    int getSquirts() { return m_squirts; }
    int getNuggets() { return m_nuggets; }
    int getSonars() { return m_sonars; }
    
    virtual void doSomething();
    
    virtual ~Iceman() { }
    
private:
    int m_health;
    int m_squirts;
    int m_nuggets;
    int m_sonars;
//    bool m_alive;
};


class Boulder : public Actor {
  
public:
    
    enum State { stable, waiting, falling };
    
    Boulder(int x, int y, StudentWorld* world) : Actor (world, IID_BOULDER, x, y, Direction::down, 1.0, 1)
    {
        m_state = stable;
        m_tick_count = 30;
        m_alive = true;
        
    };
    virtual void doSomething();
    bool ice_under_boulder();
    void killActors(const int& x, const int& y);
    
    virtual ~Boulder() { }
    
private:
    int m_state;
//    bool m_alive;
    int m_tick_count;
};

class Squirt : public Actor {
  
public:
//    Squirt(int x, int y, StudentWorld* world, Direction dir);
    Squirt(int x, int y, StudentWorld* world, Direction dir) :
    Actor(world, IID_WATER_SPURT, x, y, dir, 1.0, 1)
    {
        setVisible(true);
        m_squirt_distance = 4;
        
    }
    virtual void doSomething();
    virtual ~Squirt() { }

private:
    int m_squirt_distance;
    
};


class Barrel : public Actor {
    
public:
    
    Barrel(int x, int y, StudentWorld* world) : Actor(world, IID_BARREL, x, y, Direction::right, 1.0, 2) {
        m_alive = true;
        setVisible(false);
    }
    virtual void doSomething();
    virtual ~Barrel() { }
    
private:
//    bool m_alive;
};

class GoldNugget : public Actor
{
public:
    GoldNugget(int x, int y, StudentWorld* world, bool temporary) : Actor(world, IID_GOLD, x, y, Direction::right, 1.0, 2)
    {
        setVisible(false);
        m_temporary = temporary;
    }
    virtual void doSomething();
    
private:
    bool m_temporary;
    
    
};

class Sonar : public Actor {
    
public:
    Sonar(StudentWorld* world) : Actor(world, IID_SONAR, 0, 60, Direction::right, 1.0, 2)
    {
        setVisible(true);
        m_alive = true;
    }
    virtual void doSomething();
    void setTicksForSonar(int T) { m_T = T; }
    void decrementT() { m_T--; }
    virtual ~Sonar() { }
    
private:
//    bool m_alive;
    int m_T;
    
};

class WaterPool :public Actor {
public:
    WaterPool(int x, int y, StudentWorld* world);
    virtual void doSomething();
    void setAbilityToSpawn() { spawnable = true; }
    virtual ~WaterPool() { }
private:
    int m_numTicks;
    bool spawnable;
};

class Protester : public Actor {
public:
    
    Protester(int image, int HP, StudentWorld* world);
    virtual void doSomething();
    int getWaitingTicks() { return m_ticksToWaitBetweenMoves; }
    virtual ~Protester() { }
    
private:
    int m_HP;
    int m_numSquaresToMoveInCurrentDirection;
    int m_ticksToWaitBetweenMoves;
    bool is_resting;
    bool is_leaving;
    bool is_stunned;
    int m_resting_ticks;
    int m_ticks_stunned;
};

#endif // ACTOR_H_
