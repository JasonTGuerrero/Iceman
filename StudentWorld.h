#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
using std::min;
using std::max;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class StudentWorld : public GameWorld
{
    
private:
        
    Iceman* m_player;
    std::vector<Actor*> m_actors;
    Ice* m_ice[64][64];
    int m_boulders;
    int m_barrels;
    int m_nuggets;
    int m_sonar_ticks;
    
public:
    StudentWorld(std::string assetDir)
        : GameWorld(assetDir)
    {
    }
    
    bool is_ice(const int& x, const int& y);
    void mineIce(Iceman* player);
    void removeIce(const int& x, const int& y);
    double EuclideanDistance(int x1, int x2, int y1, int y2);
    bool boulderInTheWay(int x, int y);
    Iceman* get_player() { return m_player; }
    bool valid_spawn(int x, int y);
    int get_numBarrels();
    void update_numBarrels();
    void activate_sonar(int x, int y);
    void fireSquirtGun(const int& x, const int& y, Actor::Direction dir);
    bool isEmptyFourByFourSquare(int x, int y);
    std::string setHUD();
    
    
    virtual int init() {
        int current_level_number = getLevel();


        m_boulders = min(current_level_number / 2 + 2, 9);     // boulders
        m_nuggets = max(5-current_level_number / 2, 2);       // gold nuggets
        m_barrels = min(2 + current_level_number, 21);        // barrels

        m_player = new Iceman(this);


        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 64; y++) {
                if (((x >= 30 && x <= 33) && y >= 4) || y >= 60) {
                    m_ice[x][y] = nullptr;
                }
                else {
                    m_ice[x][y] = new Ice(this, x, y);
                }
            }
        }

        for (int i = 0; i < m_boulders; i++) {
                int x = 0;
                int y = 0;
                
                bool invalid_spawn = true;
                while (invalid_spawn)
                {
                    x = rand() % 61;
                    y = rand() % 37 + 20;
                    if ((x < 26 || x > 33))
                        invalid_spawn = false;
                    
                    if (!valid_spawn(x, y))
                        invalid_spawn = true;
                }
                Actor* p = new Boulder(x, y, this);
                for (int i = 0; i < 4; i++)
                {
                    for (int j = 0; j < 4; j++)
                    {
                        delete m_ice[p->getX()+i][p->getY()+j];
                        m_ice[p->getX()+i][p->getY()+j] = nullptr;
                    }
                }
                m_actors.push_back(p);
            }

        for (int i = 0; i < m_barrels; i++) {
            int x = 0;
            int y = 0;
            
            bool invalid_spawn = true;
            while (invalid_spawn)
            {
                x = rand() % 61;
                y = rand() % 57;
                if ((x < 26 || x > 33))
                    invalid_spawn = false;
                
                if (!valid_spawn(x, y))
                    invalid_spawn = true;
            }
            m_actors.push_back(new Barrel(x, y, this));
        }
        
        for (int i = 0; i < m_nuggets; i++) {
            int x = 0;
            int y = 0;
            
            bool invalid_spawn = true;
            while (invalid_spawn)
            {
                x = rand() % 61;
                y = rand() % 57;
                if ((x < 26 || x > 33))
                    invalid_spawn = false;
                
                if (!valid_spawn(x, y))
                    invalid_spawn = true;
            }
            m_actors.push_back(new GoldNugget(x, y, this, false));
        }

        m_sonar_ticks = getLevel() * 25 + 300;

        return GWSTATUS_CONTINUE_GAME;
    }

    
    virtual int move() {
        // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
        // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.


        m_player->doSomething();

        for (auto p : m_actors) {
            p->doSomething();
        }

        if (m_player->health() == 0) {
            playSound(SOUND_PLAYER_GIVE_UP);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }

        if (m_barrels == 0) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }

        int spawn = rand() % m_sonar_ticks;
        if (spawn == 1) {
            int sonar_spawn = rand() % 5 + 1;
            if (sonar_spawn == 1)
                m_actors.push_back(new Sonar(this));
            
            else {
                int x = 0;
                int y = 0;
                
                while (is_ice(x, y) && !isEmptyFourByFourSquare(x, y)) {
                    x = rand() % 61;
                    y = rand() % 57;
                    
                }
                m_actors.push_back(new WaterPool(x, y, this));
            }
        }
        
        setGameStatText(setHUD());
        
        return GWSTATUS_CONTINUE_GAME;
    }
    
    virtual void cleanUp() {
        delete m_player;
        for (int x = 0; x < 64; x++) {
            for (int y = 0; y < 64; y++) {
                delete m_ice[x][y];
            }
        }


        for (auto it = m_actors.begin(); it != m_actors.end(); it++) {
            delete *it;
        }

        while (!m_actors.empty()) {
            m_actors.pop_back();
        }

    }
};

#endif // STUDENTWORLD_H_
