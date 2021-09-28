#include "StudentWorld.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

std::string StudentWorld::setHUD() {
    string level = to_string(getLevel());
    while (level.length() < 2) level.insert(0, " ");
    string lives = to_string(getLives());
    string health = to_string(m_player->getHealth() * 10);
    while (health.length() < 3) health.insert(0, " ");
    string squirts = to_string(m_player->getSquirts());
    while (squirts.length() < 2) squirts.insert(0, " ");
    string nuggets = to_string(m_player->getNuggets());
    while (nuggets.length() < 2) nuggets.insert(0, " ");
    string barrels = to_string(m_barrels);
    while (barrels.length() < 2) barrels.insert(0, " ");
    string sonars = to_string(m_player->getSonars());
    while (sonars.length() < 2) sonars.insert(0, " ");
    string score = to_string(getScore());
    while (score.length() < 6) score.insert(0, "0");
    
    return ("Lvl: " + level + " Lives: " + lives + " Hlth: " + health + "% Wtr: " + squirts + " Gld: "
            + nuggets + " Oil Left: " + barrels +" Sonar: " + sonars + " Scr: " + score);
}

void StudentWorld::mineIce(Iceman* player)
{
    bool can_dig = false;
    int x = player->getX();
    int y = player->getY();
    for (int i = x; i <= x + 3; i++) {
        for (int j = y; j <= y + 3; j++) {
            if (m_ice[i][j] != nullptr) {
                delete m_ice[i][j];
                m_ice[i][j] = nullptr;
                can_dig = true;
            }
        }
    }
    if (can_dig) playSound(SOUND_DIG);
}

bool StudentWorld::is_ice(const int& x, const int& y)
{
    if (m_ice[x][y] == nullptr) return false;
    else return true;
}

bool StudentWorld::isEmptyFourByFourSquare(int x, int y) {
    for (int i = x; i < x+4; i++) {
        for (int j = y; j < y+4; j++) {
            if (is_ice(i, j))
                return false;
        }
    }
    return true;
}


void StudentWorld::fireSquirtGun(const int& x, const int& y, Actor::Direction dir) {
    if (!is_ice(x, y))
        m_actors.push_back(new Squirt(x, y, this, dir));
}

double StudentWorld::EuclideanDistance(int x1, int x2, int y1, int y2)
{
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2) * 1.0);
}

bool StudentWorld::boulderInTheWay(int x, int y)
{
    for (auto p : m_actors) {
        if (p->getID() == IID_BOULDER) {
            if (EuclideanDistance(x, p->getX(), y, p->getY()) < 4) {
                return true;
            }
        }
    }
    return false;
}

bool StudentWorld::valid_spawn(int x, int y)
{

    for (auto p : m_actors) {
        if ((EuclideanDistance(x, p->getX(), y, p->getY()) <= 6.0)) {
            return false;
        }
    }
    return true;
}

int StudentWorld::get_numBarrels()
{
    return m_barrels;
}

void StudentWorld::update_numBarrels()
{
    m_barrels--;
}

void StudentWorld::activate_sonar(int x, int y)
{
    for (auto p : m_actors) {
        if (EuclideanDistance(x, p->getX(), y, p->getY()) <= 12) {
            if (p->isAlive())
                p->setVisible(true);
        }
    }
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
