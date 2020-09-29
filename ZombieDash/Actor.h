#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;
class Goodie;
class Penelope;
class Actor :public GraphObject
{
public:
    Actor(int ID, double X, double Y, Direction dir, int depth, StudentWorld* sw);
    virtual void doSomething() = 0;
    bool isDead() const;
    StudentWorld* getWorld() const;
      void setDead();
    
    // If this is an activated object, perform its effect on a (e.g., for an
    // Exit have a use the exit).
    virtual void activateIfAppropriate(Actor* a);
  
    // If this object uses exits, use the exit.
    virtual void useExitIfAppropriate();
    // If this object can die by falling into a pit or burning, die.
    virtual void dieByFallOrBurnIfAppropriate();
    // If this object can be infected by vomit, get infected.
    virtual void beVomitedOnIfAppropriate();
    // If this object can pick up goodies, pick up g
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
     
     //Does this object block agent movement?
    virtual bool blocksMovement() const;
    
    // Does this object block flames?
    virtual bool blocksFlame() const;
    virtual bool blocksVomit() const;
    // Does this object trigger landmines only when they're active?
    virtual bool triggersOnlyActiveLandmines() const;
    // Can this object cause a zombie to vomit?
    virtual bool triggersZombieVomit() const;
    
    // Is this object a threat to citizens?
    virtual bool threatensCitizens() const;
    
    // Does this object trigger citizens to follow it or flee it?
    virtual bool triggersCitizens() const;
   
    
private:
    bool dead;
    StudentWorld* m_world;
};

class  Wall :public Actor
{
public:
    Wall(double X, double Y, StudentWorld* sw);
    virtual void doSomething();
    virtual bool blocksMovement() const;
    virtual bool blocksFlame() const;
     virtual bool blocksVomit() const;
};

class ActivatingObject : public Actor
{
public:
    ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir);
};

class Exit : public ActivatingObject
{
public:
    Exit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual bool blocksFlame() const;
};

class Pit : public ActivatingObject
{
public:
    Pit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
};

class Flame : public ActivatingObject
{
public:
    Flame(StudentWorld* w, double x, double y, int dir);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
private:
    int ticks;
};
class Vomit : public ActivatingObject
{
public:
    Vomit(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
private:
    int ticks;
};
class Landmine : public ActivatingObject
{
public:
    Landmine(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    void introduceAroundObjects();
private:
    bool active;
    int Safeticks;
};
class Goodie : public ActivatingObject
{
public:
    Goodie(StudentWorld* w, double x, double y,int ID);
    virtual void activateIfAppropriate(Actor* a);
    virtual void dieByFallOrBurnIfAppropriate();
    
    // Have p pick up this goodie.
    virtual void pickUp(Penelope* p) = 0;
};
class VaccineGoodie : public Goodie
{
public:
    VaccineGoodie(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void pickUp(Penelope* p);
};
class GasCanGoodie : public Goodie
{
public:
    GasCanGoodie(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void pickUp(Penelope* p);
};
class LandmineGoodie : public Goodie
{
public:
    LandmineGoodie(StudentWorld* w, double x, double y);
    virtual void doSomething();
    virtual void pickUp(Penelope* p);
};
class Agent : public Actor
{
public:
    Agent(StudentWorld* w, int imageID, double x, double y, int dir);
   virtual bool blocksMovement() const;
    virtual bool triggersOnlyActiveLandmines() const;
     virtual bool triggersCitizens() const;
    
};
class Human : public Agent
{
public:
    Human(StudentWorld* w, int imageID, double x, double y);
    virtual void beVomitedOnIfAppropriate();
   
   virtual bool triggersZombieVomit() const;
    
    // Make this human uninfected by vomit.
    void clearInfection();
    void increaseInfection();
    bool getStatus();
    // How many ticks since this human was infected by vomit?
    int getInfectionDuration() const;
private:
    int infectamount;
    bool infectionStatus;
};
class Citizen : public Human
{
public:
    Citizen(StudentWorld* w,  double x, double y);
    virtual bool triggersCitizens() const;
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
private:
   bool flag;
    bool flag2;
};
class Zombie : public Agent
{
public:
    Zombie(StudentWorld* w,  double x, double y);
      virtual bool threatensCitizens() const;

};

class DumbZombie : public Zombie
{
public:
    DumbZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
private:
    bool flag;
    int movePlan;
};
class SmartZombie : public Zombie
{
public:
    SmartZombie(StudentWorld* w,  double x, double y);
    virtual void doSomething();
    virtual void dieByFallOrBurnIfAppropriate();
private:
    bool flag;
    int movePlan;
};
class Penelope :public Human {
public:
    Penelope(double X, double Y, StudentWorld* sw);
    virtual void doSomething();
    virtual void useExitIfAppropriate();
    virtual void dieByFallOrBurnIfAppropriate();
    virtual void pickUpGoodieIfAppropriate(Goodie* g);
    // Increase the number of vaccines the object has.
    void increaseVaccines();
    
    // Increase the number of flame charges the object has.
    void increaseFlameCharges();
    
    // Increase the number of landmines the object has.
    void increaseLandmines();

    // How many vaccines does the object have?
    
    int getNumVaccines() const;
    
    // How many flame charges does the object have?
    int getNumFlameCharges() const;
    
    // How many landmines does the object have?
    int getNumLandmines() const;
private:
    int mine, flame, vaccine ;
};



#endif // ACTOR_H_
