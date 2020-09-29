#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <cmath>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
//initialize datamembers
: GameWorld(assetPath), playercount(0),citizencount(0),zombiecount(0),usedexit(false)
{
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}
void StudentWorld::addActor(Actor *a){
    ActorList.push_back(a);
}
void StudentWorld::ExitUsed(){
    usedexit=true;
}
 
int StudentWorld::init()
{
    ostringstream levelFile;
    levelFile.fill('0');//load with 0 for empty space
    levelFile << "level" << setw(2)<<getLevel() << ".txt";
    string currentLevel = levelFile.str();
    Level lev(assetPath());
    Level::LoadResult result = lev.loadLevel(currentLevel);
    if(result==Level::load_fail_file_not_found){
        return GWSTATUS_PLAYER_WON;
    }
    if(result==Level::load_fail_bad_format){
        return GWSTATUS_LEVEL_ERROR;
    }
    for (int x = 0;x < LEVEL_WIDTH;x++) {
        for (int y = 0;y < LEVEL_HEIGHT;y++) {
            Level::MazeEntry ge = lev.getContentsOf(x, y);
            switch (ge) {
                case Level::empty:
                    break;
                case Level::player:
                    player = new Penelope(x * 16, y * 16, this);
                    playercount++;//keep tracking the number
                    break;
                case Level::dumb_zombie:
                    ActorList.push_back(new DumbZombie(this,x*16,y*16));
                    zombiecount++;//keep tracking the number
                    break;
                case Level::smart_zombie:
                    ActorList.push_back(new SmartZombie(this,x*16,y*16));
                    zombiecount++;//keep tracking the number
                    break;
                case Level::citizen:
                    ActorList.push_back(new Citizen(this,x*16,y*16));
                    citizencount++;//keep tracking the number
                    break;
                case Level::wall:
                    ActorList.push_back(new Wall(x * 16, y * 16, this));
                    break;
                case Level::exit:
                    ActorList.push_back(new Exit(this,x*16,y*16));
                    break;
                case Level::pit:
                    ActorList.push_back(new Pit(this,x*16,y*16));
                    break;
                case Level::vaccine_goodie:
                    ActorList.push_back(new VaccineGoodie(this,x*16,y*16));
                    break;
                case Level::gas_can_goodie:
                    ActorList.push_back(new GasCanGoodie(this,x*16,y*16));
                    break;
                case Level::landmine_goodie:
                    ActorList.push_back(new LandmineGoodie(this,x*16,y*16));
                    break;
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}
bool StudentWorld::isblock(Agent* a, double x, double y) const{
    
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if (a != *p) {//make sure that it won't be blocked by itself
            if ((abs((*p)->getX() - x) < SPRITE_WIDTH - 1 && abs((*p)->getY() - y) < SPRITE_HEIGHT - 1)&&(*p)->blocksMovement()) {
                return true;
            }
        }
    }
    if (a != player) {//take the player into consideration
        if ((abs((player)->getX() - x) < SPRITE_WIDTH - 1 && abs((player)->getY() - y) < SPRITE_HEIGHT - 1)&&(player)->blocksMovement()) {
            return true;
        }
    }
   
     return false;
}



int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    //keep updating the stats;
    ostringstream Stats;
    Stats<<"Score: ";
    Stats.fill('0');
    
    if(getScore()>=0){
        Stats<<setw(6);
        Stats<<getScore();
    }else{
        Stats<<'-'<<setw(5);
        Stats<<(-getScore());
    }
    
    
    
    Stats<<"  Level: "<<getLevel()<<"  Lives: "<<getLives();
    Stats<<"  Vaccines: "<<player->getNumVaccines()<<"  Flames: "<<player->getNumFlameCharges()<<"  Mines: "<<player->getNumLandmines()<<" Infected: "<<player->getInfectionDuration();
    setGameStatText(Stats.str());
    if(!player->isDead()){
    player->doSomething();
    }else  {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    //activate every alive actors
    for (list<Actor*>::iterator p = ActorList.begin();p != ActorList.end(); p++) {
        if(!(*p)->isDead()){
        activateOnAppropriateActors(*p);
        (*p)->doSomething();
             
        if(usedexit&&citizencount==0){
            usedexit=false;
            playSound(SOUND_LEVEL_FINISHED);
            return GWSTATUS_FINISHED_LEVEL;
        }
        }
    }
    //clean up dead players
    for (list<Actor*>::iterator p = ActorList.begin();p != ActorList.end(); ) {
        
        if((*p)->isDead()){
            delete (*p);
            p = ActorList.erase(p);
           
        }else{
            p++;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
    
}

void StudentWorld::cleanUp()
{
    //make sure that there are no memory leaks and we won't follow a nullptr or dangling pointer
    if (playercount == 1) {
        delete player;
        playercount--;
    }
   
    playercount=0;
    zombiecount=0;
    citizencount=0;
    for (list<Actor*>::iterator p = ActorList.begin();p != ActorList.end(); ) {
        delete *p;
        p = ActorList.erase(p);
    }
}
void StudentWorld::activateOnAppropriateActors(Actor* a)
{
    for(list<Actor*>::iterator p=ActorList.begin();p!=ActorList.end();p++){
        if(pow((*p)->getX()-a->getX(),2)+pow((*p)->getY()-a->getY(),2)<=100){
            if(a!=(*p))//make sure it won't be overlapped by itself
            a->activateIfAppropriate(*p);
        }
    }
    if(a!=player){//take player into consideration
    if(pow((player)->getX()-a->getX(),2)+pow((player)->getY()-a->getY(),2)<=100)
    a->activateIfAppropriate(player);
    }
}
//helper function
bool StudentWorld::isFlameBlockedAt(double x, double y) const {
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if ((pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2))<=100&&(*p)->blocksFlame()) {
            return true;
        }
    }
    return false;
}
//helper function
bool StudentWorld::isVomitBlockedAt(double x, double y) const{
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if ((pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2))<=100&&(*p)->blocksVomit()) {
            return true;
        }
    }
    return false;
}
//consider every actor including penelope
bool StudentWorld::isZombieVomitTriggerAt(double x, double y) const{
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if ((pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2))<=100&&(*p)->triggersZombieVomit()) {
            return true;
        }
    }
    if ((pow(player->getX()-x,2)+pow(player->getY()-y,2))<=100&&player->triggersZombieVomit()) {
        return true;
    }
    return false;
}
void StudentWorld::increaseZ(int i){
    zombiecount+=i;
}
void StudentWorld::increaseC(int i){
    citizencount+=i;
}
// Return true if there is a living zombie or Penelope, otherwise false.
// If true, otherX, otherY, and distance will be set to the location and
// distance of the one nearest to (x,y), and isThreat will be set to true
// if it's a zombie, false if a Penelope.
bool StudentWorld::locateNearestCitizenTrigger(double x, double y, double& otherX, double& otherY, double& distance, bool& isThreat) const{
    
  Penelope*  const i = player;
    if(i->isDead()&&zombiecount==0) {
        return false;
    }
    double min = sqrt(pow(i->getX()-x,2)+pow(i->getY()-y,2));
    double coordX=i->getX();
    double coordY=i->getY();
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if ((*p)->triggersCitizens()) {
            if(min >sqrt(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2))){
                min=sqrt(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2));
                coordX=(*p)->getX();
                coordY=(*p)->getY();
            }
        }
    }
    if(min==sqrt(pow(i->getX()-x,2)+pow(i->getY()-y,2))){
       
        isThreat=false;
    }else{
      
        isThreat=true;
    }
    otherX=coordX;
    otherY=coordY;
    distance=min;
        
    return true;
    
    
}
// Return true if there is a living zombie, false otherwise.  If true,
// otherX, otherY and distance will be set to the location and distance
// of the one nearest to (x,y).
bool StudentWorld:: locateNearestCitizenThreat(double x, double y, double& otherX, double& otherY, double& distance) const{
    if(zombiecount==0){
        return false;
    }
    double min=VIEW_WIDTH;
    double coordX=0;
    double coordY=0;
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if ((*p)->threatensCitizens()) {
            if(min >sqrt(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2))){
                min=sqrt(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2));
                coordX=(*p)->getX();
                coordY=(*p)->getY();
            }
        }
    }
    distance=min;
    otherX=coordX;
    otherY=coordY;

    return true;
}

// Return true if there is a living human, otherwise false.  If true,
// otherX, otherY, and distance will be set to the location and distance
// of the human nearest to (x,y).
bool StudentWorld:: locateNearestVomitTrigger(double x, double y, double& otherX, double& otherY, double& distance)
    {
    Penelope*  const i = player;
    if(i->isDead()&&citizencount==0) {
        return false;
    }
    double min = sqrt(pow(i->getX()-x,2)+pow(i->getY()-y,2));
    double coordX=i->getX();
    double coordY=i->getY();
    for (list<Actor*>::const_iterator p = ActorList.begin();p != ActorList.end();p++) {
        if ((*p)->triggersZombieVomit()) {
            if(min >sqrt(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2))){
                min=sqrt(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2));
                coordX=(*p)->getX();
                coordY=(*p)->getY();
            }
        }
    }
   
    otherX=coordX;
    otherY=coordY;
    distance=min;
    
    return true;
}

int StudentWorld::getcitizenamount() const{
    return citizencount;
}
//helper function to detect overlapping
bool StudentWorld::isoverlap(Agent *a, double x, double y)const{
  
    for(list<Actor*>::const_iterator p=ActorList.begin();p!=ActorList.end();p++){
        if(pow((*p)->getX()-x,2)+pow((*p)->getY()-y,2)<=100){
            if(a!=(*p))
                return true;
        }
    }
    if(a!=player){
        if(pow((player)->getX()-x,2)+pow((player)->getY()-y,2)<=100)
            return true;
    }
    return false;
}
//help the citizen run away
double StudentWorld::getLargest(double a, double b, double c, double d){
    double arr[4];
    arr[0]=a;  arr[1]=b;  arr[2]=c;  arr[3]=d;
    double max=a;
    for(int i=0;i<4;i++){
       if(arr[i]>max)
           max=arr[i];
    }
    return max;
    
}
