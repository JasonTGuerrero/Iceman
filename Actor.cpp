#include <cstdlib>
#include <queue>
#include "Actor.h"
#include "StudentWorld.h"

bool valid_move(const int& x, const int& y) {
    if (x >= 0 && x <= 60 && y >= 0 && y <= 60) return true;
    else return false;
}

StudentWorld* Actor::get_world() { return m_world; }

void Iceman::doSomething() {
    if (m_health == 0) return;
    
    int ch;
    if (get_world()->getKey(ch)) {
    
        switch (ch) {
                
            case KEY_PRESS_LEFT:
                if (getDirection() != left) setDirection(left);
                if (valid_move(getX()-1, getY()) && !get_world()->boulderInTheWay(getX()-1, getY())) {
                    moveTo(getX()-1, getY());
                    get_world()->mineIce(this);
                    }
                break;
                
            case KEY_PRESS_UP:
                if (getDirection() != up) setDirection(up);
                if (valid_move(getX(), getY()+1) && !get_world()->boulderInTheWay(getX(), getY()+1)) {
                    moveTo(getX(), getY()+1);
                    get_world()->mineIce(this);
                }
                break;
                
            case KEY_PRESS_DOWN:
                if (getDirection() != down) setDirection(down);
                if (valid_move(getX(), getY()-1) && !get_world()->boulderInTheWay(getX(), getY()-1)) {
                    moveTo(getX(), getY()-1);
                    get_world()->mineIce(this);
                }
                break;
                
            case KEY_PRESS_RIGHT:
                if (getDirection() != right) setDirection(right);
                if (valid_move(getX()+1, getY()) && !get_world()->boulderInTheWay(getX()+1, getY())) {
                    moveTo(getX()+1, getY());
                    get_world()->mineIce(this);
                }
                break;
                
            case KEY_PRESS_SPACE:
                if (m_squirts > 0) {
                    update_squirts(-1);
                    get_world()->playSound(SOUND_PLAYER_SQUIRT);
                    int x = Iceman::getX();
                    int y = Iceman::getY();
                    if (Iceman::getDirection() == up)
                        y++;
                    if (Iceman::getDirection() == down)
                        y--;
                    if (Iceman::getDirection() == right)
                        x++;
                    if (Iceman::getDirection() == left)
                        x--;

                    if (!get_world()->boulderInTheWay(x, y) && valid_move(x, y)) {
                        get_world()->fireSquirtGun(x, y, Iceman::getDirection());
                    }
                }
                break;
                
            case KEY_PRESS_ESCAPE:
                update_health(0);
                break;
                
            case 'q':
                exit(0);
                break;
                
            case 'z':
            case 'Z':
                if (m_sonars > 0) {
                    update_sonars(-1);
                    get_world()->activate_sonar(getX(), getY());
                    get_world()->playSound(SOUND_SONAR);
                }
                break;

        }
    }
}


bool Boulder::ice_under_boulder() {
    for (int i = 0; i <= 3; i++) {
        if (get_world()->is_ice(Boulder::getX()+i, Boulder::getY()-1))
            return true;
    }
    return false;
}

// will modify later to include killing protesters as well
void Boulder::killActors(const int& x, const int& y) {
    
    if (get_world()->EuclideanDistance(x, get_world()->get_player()->getX(), y, get_world()->get_player()->getY()) < 4)
    {
        setVisible(false);
        get_world()->get_player()->update_health(0);
        died();
    }
    
}

void Boulder::doSomething() {
    
    if (!isAlive()) {
        setVisible(false);
        return;
    }
    
    int x = getX();
    int y = getY();
    
    if (m_state == stable) {
        bool no_ice = true;
        for (int i = 0; i <= 3; i++) {
            if (get_world()->is_ice(x+i, y-1)) {
                no_ice = false;
            }
        }
        if (no_ice) {
            m_state = waiting;
        }
    }
    
    if (m_state == waiting) {
        
        m_tick_count--;
        if (m_tick_count == 0) {
            m_state = falling;
            get_world()->playSound(SOUND_FALLING_ROCK);
        }
        
    }
    
    if (m_state == falling) {
        if (getY() >= 0 && !ice_under_boulder()) {
            moveTo(getX(), getY()-1);
            killActors(getX(), getY());
        }
        else
            died();
    }
}


void Squirt::doSomething() {
    if (!isAlive()) {
        setVisible(false);
        return;
    }
    
    if (m_squirt_distance > 0)
        m_squirt_distance--;
    else
        died();
    
    switch (getDirection()) {
        case up:
            if (!get_world()->is_ice(getX(), getY()+1) && !get_world()->boulderInTheWay(getX(), getY()))
                moveTo(getX(), getY()+1);
            break;
            
        case down:
            if (!get_world()->is_ice(getX(), getY()-1) && !get_world()->boulderInTheWay(getX(), getY()))
                moveTo(getX(), getY()-1);
            break;
            
        case left:
            if (!get_world()->is_ice(getX()-1, getY()) && !get_world()->boulderInTheWay(getX(), getY()))
                moveTo(getX()-1, getY());
            break;
            
        case right:
            if (!get_world()->is_ice(getX()+1, getY()) && !get_world()->boulderInTheWay(getX(), getY()))
                moveTo(getX()+1, getY());
            break;
            
        case none:
            break;
    }
    
}

void Barrel::doSomething() {
    
    if (!isAlive())
        return;
    
    else if (!isVisible() && get_world()->EuclideanDistance(getX(), get_world()->get_player()->getX(), getY(), get_world()->get_player()->getY()) <= 4) {
        setVisible(true);
        return;
    }
    
    else if (get_world()->EuclideanDistance(getX(), get_world()->get_player()->getX(), getY(), get_world()->get_player()->getY()) <= 3) {
        died();
        get_world()->increaseScore(1000);
        get_world()->update_numBarrels();
        get_world()->playSound(SOUND_FOUND_OIL);
        setVisible(false);
    }
    
}

void GoldNugget::doSomething() {
    if (!isAlive())
        return;
    
    else if (!isVisible() && get_world()->EuclideanDistance(getX(), get_world()->get_player()->getX(), getY(), get_world()->get_player()->getY()) <= 4) {
        setVisible(true);
        return;
    }
    
    else if (get_world()->EuclideanDistance(getX(), get_world()->get_player()->getX(), getY(), get_world()->get_player()->getY()) <= 3) {
        died();
        get_world()->increaseScore(10);
        get_world()->get_player()->update_nuggets(1);
        get_world()->playSound(SOUND_GOT_GOODIE);
        setVisible(false);
    }
}

void Sonar::doSomething() {
    if (!isAlive())
        return;
    
    else {
        m_T--;
        if (get_world()->EuclideanDistance(getX(), get_world()->get_player()->getX(), getY(), get_world()->get_player()->getY()) <= 3) {
            
            died();
            setVisible(false);
            get_world()->playSound(SOUND_GOT_GOODIE);
            get_world()->increaseScore(75);
            get_world()->get_player()->update_sonars(1);
            
        }
    }
    if (m_T == 0) {
        died();
        setVisible(false);
    }
}

WaterPool::WaterPool(int x, int y, StudentWorld* world) : Actor(world, IID_WATER_POOL, x, y, Direction::right, 1.0, 2)
{
    m_numTicks = std::max(100, 300 - 10*((int)get_world()->getLevel()));
    setVisible(true);
    spawnable = false;
}

void WaterPool::doSomething() {
    if (!isAlive()) return;
    
    else {
        m_numTicks--;
        if (get_world()->EuclideanDistance(getX(), get_world()->get_player()->getX(), getY(), get_world()->get_player()->getY()) <= 3) {
            
            died();
            setVisible(false);
            get_world()->playSound(SOUND_GOT_GOODIE);
            get_world()->increaseScore(100);
            get_world()->get_player()->update_squirts(5);
            
        }
        if (m_numTicks == 0){
            died();
            setVisible(false);
        }
    }
}

Protester::Protester(int image, int HP, StudentWorld* world) : Actor(world, image, 60, 60, Direction::left, 1.0, 0)
{
    setVisible(true);
    m_numSquaresToMoveInCurrentDirection = rand() % 53 + 8;
    m_HP = 5;
    m_ticksToWaitBetweenMoves = 3 - ((int)get_world()->getLevel() / 4);
    is_resting = false;
    is_leaving = false;
    is_stunned = false;
}

void Protester::doSomething() {
    if (!isAlive()) return;
    
    if (is_resting) {
        m_resting_ticks++;
        return;
    }
    
    if (is_leaving) {
        if (getX() == 60 && getY() == 60) {
            died();
        }
    }
    
}


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
