#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "Level.h"
#include <list>
#include <sstream>
#include <iomanip>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Agent;
class Penelope;
class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetPath);
    ~StudentWorld();
    void addActor(Actor* a);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    bool isblock(Agent* a, double x, double y ) const;
    bool isoverlap(Agent* a, double x, double y ) const;
    bool isZombieVomitTriggerAt(double x, double y) const;
    bool isFlameBlockedAt(double x, double y) const;
    bool isVomitBlockedAt(double x, double y) const;
    void ExitUsed();
    
    int getcitizenamount() const;
    void activateOnAppropriateActors(Actor* a);
    bool locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const;
    bool  locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance);
    bool  locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const;
    void increaseZ(int i);
    void increaseC(int i);
    double getLargest(double a,double b,double c,double d);
private:
    list< Actor*> ActorList;
    Penelope* player;
    int playercount;
    int citizencount;
    int zombiecount;
    bool usedexit;
    
};

#endif // STUDENTWORLD_H_
