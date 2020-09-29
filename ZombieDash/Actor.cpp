#include "Actor.h"
#include "StudentWorld.h"
#include <cmath>
// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(int ID, double X, double Y, Direction dir, int depth, StudentWorld* sw)
:GraphObject(ID, X, Y, dir, depth), dead(false), m_world(sw)
{
    
}
void Actor::activateIfAppropriate(Actor *a){
    
}
void Actor::useExitIfAppropriate(){
    
}
bool Actor::isDead() const {
    return dead;
}
void Actor::setDead(){
    dead=true;
}
StudentWorld* Actor::getWorld() const{
    return m_world;
}

bool Actor::blocksMovement() const{
    return false;
}
bool Actor::blocksFlame() const{
    return false;
}
bool Actor::blocksVomit()const{
    return false;
}
void Actor::dieByFallOrBurnIfAppropriate(){
    
}
void Actor::beVomitedOnIfAppropriate(){
    
}
bool Actor::triggersOnlyActiveLandmines() const{
    return false;
}
void Actor::pickUpGoodieIfAppropriate(Goodie *g){
    
}
bool Actor::triggersZombieVomit()const{
    return false;
}
bool Actor::threatensCitizens()const{
    return false;
}
bool Actor::triggersCitizens()const{
    return false;
}
Wall::Wall(double X, double Y, StudentWorld* sw)
:Actor(IID_WALL, X, Y, right, 0, sw)
{
    
}

void Wall::doSomething() {
    return;
}
bool Wall::blocksFlame() const{
    return true;
}
bool Wall::blocksVomit()const{
    return true;
}
bool Wall::blocksMovement() const{
    return true;
}

ActivatingObject::ActivatingObject(StudentWorld* w, int imageID, double x, double y, int depth, int dir)
:Actor(imageID,x,y,dir,depth,w)
{
}

Exit::Exit(StudentWorld* w, double x, double y)
:ActivatingObject(w,IID_EXIT,x,y,1,right)
{
}

void Exit::doSomething(){
}

void Exit::activateIfAppropriate(Actor* a){
    if (!isDead())
        a->useExitIfAppropriate();
}
bool Exit::blocksFlame() const{
    return true;
}

Pit::Pit(StudentWorld* w, double x, double y)
:ActivatingObject(w,IID_PIT,x,y,0,right)
{
}
void Pit::doSomething(){
    
}
void Pit::activateIfAppropriate(Actor *a){
    if (!isDead())
        a->dieByFallOrBurnIfAppropriate();
}

Flame::Flame(StudentWorld* w, double x, double y, int dir)
:ActivatingObject(w,IID_FLAME,x,y,0,dir),ticks(0)
{
}
void Flame::doSomething(){
    //set dead after 2 ticks
    if(!isDead()){
        ticks++;
        if(ticks==2){
            this->setDead();
            ticks=0;
        }
    }
}
void Flame::activateIfAppropriate(Actor *a){
    if(!isDead())
        a->dieByFallOrBurnIfAppropriate();
}
Vomit::Vomit(StudentWorld* w, double x, double y)
:ActivatingObject(w,IID_VOMIT,x,y,0,right),ticks(0)
{
}
void Vomit::doSomething(){
    //setdead after 2 ticks
    if(!isDead()){
        ticks++;
        if(ticks==2){
            this->setDead();
            ticks=0;
        }
    }
}
void Vomit::activateIfAppropriate(Actor *a){
    if(!isDead())
        a->beVomitedOnIfAppropriate();
}
Landmine::Landmine(StudentWorld* w, double x, double y)
:ActivatingObject(w,IID_LANDMINE,x,y,1,right),active(false),Safeticks(30)
{
}
void Landmine::doSomething(){
    //set 30 safety ticks
    if(!isDead()){
        if(!active){
            Safeticks--;
            if(Safeticks==0){
                active=true;
                return;
            }
            
        }
    }
}
void Landmine::introduceAroundObjects(){
    //introduce 9 flames and 1 pit if not blocked
    
    Pit* a10=new Pit(getWorld(),getX(),getY());
    for(int i=-1;i<2;i++){
        for(int j=-1;j<2;j++){
            if(!getWorld()->isFlameBlockedAt(getX()+i*SPRITE_WIDTH, getY()+j*SPRITE_HEIGHT))
                getWorld()->addActor(new Flame(getWorld(),getX()+i*SPRITE_WIDTH,getY()+j*SPRITE_HEIGHT,up));
            
        }
    }
    
    
    getWorld()->addActor(a10);
    
}
void Landmine::activateIfAppropriate(Actor *a){
    //activate if it active and appropriate actors trigger it
    if(!isDead()&&active){
        if(a->triggersOnlyActiveLandmines()){
            this->setDead();
            getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
            introduceAroundObjects();
        }
    }
    
}
void Landmine::dieByFallOrBurnIfAppropriate(){
    if(!isDead()){
        setDead();
        getWorld()->playSound(SOUND_LANDMINE_EXPLODE);
        introduceAroundObjects();
    }
    
}
Goodie::Goodie(StudentWorld* w, double x, double y,int ID)
:ActivatingObject(w,ID,x,y,1,right)
{
}
void Goodie::activateIfAppropriate(Actor *a){
    if(!isDead())
        a->pickUpGoodieIfAppropriate(this);
    
}
void Goodie::dieByFallOrBurnIfAppropriate(){
    setDead();
}
VaccineGoodie::VaccineGoodie(StudentWorld* w, double x, double y)
:Goodie(w,x,y,IID_VACCINE_GOODIE)
{
}
void VaccineGoodie::doSomething(){
    
}
void VaccineGoodie::pickUp(Penelope *p){
    p->increaseVaccines();
}

GasCanGoodie::GasCanGoodie(StudentWorld* w, double x, double y)
:Goodie(w,x,y,IID_GAS_CAN_GOODIE)
{
}
void GasCanGoodie::doSomething(){
    
}
void GasCanGoodie::pickUp(Penelope *p){
    p->increaseFlameCharges();
}
LandmineGoodie::LandmineGoodie(StudentWorld* w, double x, double y)
:Goodie(w,x,y,IID_LANDMINE_GOODIE)
{
}
void LandmineGoodie::doSomething(){
    
}
void LandmineGoodie::pickUp(Penelope *p){
    p->increaseLandmines();
}
Agent::Agent(StudentWorld* w, int imageID, double x, double y, int dir)
:Actor(imageID,x,y,dir,0,w)
{
    
}
bool Agent::triggersOnlyActiveLandmines() const{
    return true;
}
bool Agent::blocksMovement()const{
    return true;
}
bool Agent::triggersCitizens()const{
    return true;
}
Human::Human(StudentWorld* w, int imageID, double x, double y)
:Agent(w,imageID,x,y,right),infectamount(0),infectionStatus(false)
{
}
int Human::getInfectionDuration() const{
    return infectamount;
}
void Human::beVomitedOnIfAppropriate(){
    if(!isDead()){
        this->infectionStatus=true;
        
    }
    
}
bool Human::getStatus(){
    return infectionStatus;
}
bool Human::triggersZombieVomit() const{
    return true;
}
void Human::clearInfection(){
    infectionStatus=false;
    infectamount=0;
}
void Human::increaseInfection(){
    if(infectionStatus==true)
        infectamount++;
}
Citizen::Citizen(StudentWorld* w,  double x, double y)
:Human(w,IID_CITIZEN,x,y),flag(true),flag2(true)
{
}
void Citizen::doSomething(){
    if(!isDead()){
        increaseInfection();
        //make sure the sound is only played on the first vomit
        if(getStatus()&&flag2){
            
            getWorld()->playSound(SOUND_CITIZEN_INFECTED);
            flag2=false;
        }
        
        if(getInfectionDuration()==500){
            getWorld()->increaseC(-1);
            setDead();
            getWorld()->playSound(SOUND_ZOMBIE_BORN);
            getWorld()->increaseScore(-1000);
            //randomly create zombies after death
            int r=randInt(1, 10);
            if( 1<=r&&r<=3){
                getWorld()->addActor(new SmartZombie(getWorld(),getX(),getY()));
                getWorld()->increaseZ(1);
            }else{
                getWorld()->addActor(new DumbZombie(getWorld(),getX(),getY()));
                getWorld()->increaseZ(1);
            }
            return;
        }
        // paralyzed flag
        if(flag){
            flag=false;
            return;
        }
        flag=true;
        double coordX=0;
        double coordY=0;
        double distance=0;
        bool isZombie=true;
        
        if( getWorld()->locateNearestCitizenTrigger(getX(), getY(), coordX, coordY, distance, isZombie)){
            int r=randInt(0, 1);
            //randomly pick a direction if blocked then choose another one
            if(!isZombie&&distance<=80){
                if( r==0){
                    if(coordX>getX()){
                        if(!getWorld()->isblock(this, getX()+2, getY())){
                            setDirection(right);
                            moveTo(getX()+2, getY());
                            return;
                        }
                        r=1;
                    }
                    if(coordX<getX()){
                        if(!getWorld()->isblock(this, getX()-2, getY())){
                            setDirection(left);
                            moveTo(getX()-2, getY());
                            return;
                        }
                        r=1;
                    }
                }
                
                
                if( r==1){
                    if(coordY>getY()){
                        if(!getWorld()->isblock(this, getX(), getY()+2)){
                            setDirection(up);
                            moveTo(getX(), getY()+2);
                            return;
                        }
                    }
                    if(coordY<getY()){
                        if(!getWorld()->isblock(this, getX(), getY()-2)){
                            setDirection(down);
                            moveTo(getX(), getY()-2);
                            return;
                        }
                    }
                    if(coordX>getX()){
                        if(!getWorld()->isblock(this, getX()+2, getY())){
                            setDirection(right);
                            moveTo(getX()+2, getY());
                            return;
                        }
                    }
                    if(coordX<getX()){
                        if(!getWorld()->isblock(this, getX()-2, getY())){
                            setDirection(left);
                            moveTo(getX()-2, getY());
                            return;
                        }
                    }
                }
            }
            if(isZombie&&distance<=80){
                //pick a direction if blocked, ignore that direction
                double Zx=0;double Zy=0;double distanceZ=VIEW_HEIGHT;
                getWorld()->locateNearestCitizenThreat(getX()-2, getY(), Zx, Zy, distanceZ);
                double d1=distanceZ;
                getWorld()->locateNearestCitizenThreat(getX()+2, getY(), Zx, Zy, distanceZ);
                double d2=distanceZ;
                getWorld()->locateNearestCitizenThreat(getX(), getY()+2, Zx, Zy, distanceZ);
                double d3=distanceZ;
                getWorld()->locateNearestCitizenThreat(getX(), getY()-2, Zx, Zy, distanceZ);
                double d4=distanceZ;
                
                if(getWorld()->isblock(this, getX()-2, getY()))
                    d1=0;
                if(getWorld()->isblock(this, getX()+2, getY()))
                    d2=0;
                if(getWorld()->isblock(this, getX(), getY()-2))
                    d4=0;
                if(getWorld()->isblock(this, getX(), getY()+2))
                    d3=0;
                //pick the direction with greatest  distance to run away
                double max=getWorld()->getLargest(d1, d2, d3, d4);
                if(max!=0){
                    if(max==d1){
                        
                        setDirection(left);
                        moveTo(getX()-2, getY());
                        return;
                    }
                    if(max==d2){
                        
                        setDirection(right);
                        moveTo(getX()+2, getY());
                        return;
                    }
                    
                    if(max==d4){
                        setDirection(down);
                        moveTo(getX(), getY()-2);
                        return;
                    }
                    if(max==d3){
                        setDirection(up);
                        moveTo(getX(), getY()+2);
                        return;
                    }
                }
                
                
            }
        }
        
        
    }
}
void Citizen::useExitIfAppropriate(){
    if(!isDead()){
        getWorld()->increaseScore(500);
        getWorld()->increaseC(-1);
        setDead();
        getWorld()->playSound(SOUND_CITIZEN_SAVED);
        
    }
    
}
void Citizen::dieByFallOrBurnIfAppropriate(){
    if(!isDead()){
        getWorld()->increaseC(-1);
        getWorld()->playSound(SOUND_CITIZEN_DIE);
        getWorld()->increaseScore(-1000);
        setDead();
    }
}
bool Citizen::triggersCitizens()const{
    return false;
}
Zombie::Zombie(StudentWorld* w,  double x, double y)
:Agent(w,IID_ZOMBIE,x,y,right)
{
}
bool Zombie::threatensCitizens()const{
    return true;
}
DumbZombie::DumbZombie(StudentWorld* w,  double x, double y)
:Zombie(w,x,y),flag(true),movePlan(0)
{
}
void DumbZombie::doSomething(){
    if(!isDead()){
        if(flag){
            flag=false;
            return;
        }
        flag=true;
        double Vx=0; double Vy=0;
        double destX=0;double destY=0;
        //compute vomit coordinate
        switch (getDirection()) {
            case right:
                Vx=getX()+SPRITE_WIDTH;
                Vy=getY();
                break;
            case left:
                Vx=getX()-SPRITE_WIDTH;
                Vy=getY();
                break;
            case up:
                Vx=getX();
                Vy=getY()+SPRITE_HEIGHT;
                break;
            case down:
                Vx=getX();
                Vy=getY()-SPRITE_HEIGHT;
                break;
        }
        double x=0,y=0,distance=VIEW_HEIGHT;
        
        if(getWorld()->locateNearestVomitTrigger(Vx, Vy, x, y, distance)){
            if(distance<=10){
                // vomit on 1/3 chance if vomit is not blocked
                int r=randInt(1, 3);
                if(r==1){
                    if(!getWorld()->isVomitBlockedAt(Vx, Vy)){
                        getWorld()->addActor(new Vomit(getWorld(),Vx,Vy));
                        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                        return;
                    }
                }
            }
        }
        if(movePlan==0){
            movePlan=randInt(3, 10);
            int dir=randInt(1, 4);
            if(dir==1)
                setDirection(right);
            if(dir==2)
                setDirection(left);
            if(dir==3)
                setDirection(up);
            if(dir==4)
                setDirection(down);
        }
        switch (getDirection()) {
            case right:
                destX=getX()+1;
                destY=getY();
                break;
            case left:
                destX=getX()-1;
                destY=getY();
                break;
            case up:
                destX=getX();
                destY=getY()+1;
                break;
            case down:
                destX=getX();
                destY=getY()-1;
                break;
        }
        //if not blocked ,move on the new movement plan assigned
        if(!getWorld()->isblock(this, destX, destY)){
            moveTo(destX, destY);
            movePlan--;
        }
        movePlan=0;
    }
}
void DumbZombie::dieByFallOrBurnIfAppropriate(){
    if(!isDead()){
        setDead();
        getWorld()->increaseZ(-1);
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
        getWorld()->increaseScore(1000);
        int i=randInt(1, 10);
        if(i==7){
            //drop a vaccine at appropriate position if not blocked on a 10% probability
            int dir=randInt(1, 4);
            if(dir==1){
                if(!getWorld()->isoverlap(this, getX()+SPRITE_WIDTH,getY()))
                    getWorld()->addActor(new VaccineGoodie(getWorld(),getX()+SPRITE_WIDTH,getY()));
            }
            
            if(dir==2){
                if(!getWorld()->isoverlap(this, getX()-SPRITE_WIDTH,getY()))
                    getWorld()->addActor(new VaccineGoodie(getWorld(),getX()-SPRITE_WIDTH,getY()));
            }
            
            if(dir==3){
                if(!getWorld()->isoverlap(this, getX(),getY()+SPRITE_HEIGHT))
                    getWorld()->addActor(new VaccineGoodie(getWorld(),getX(),getY()+SPRITE_HEIGHT));
            }
            
            if(dir==4){
                if(!getWorld()->isoverlap(this, getX(),getY()-SPRITE_HEIGHT))
                    getWorld()->addActor(new VaccineGoodie(getWorld(),getX(),getY()-SPRITE_HEIGHT));
            }
            
        }
    }
}
SmartZombie::SmartZombie(StudentWorld* w,  double x, double y)
:Zombie(w,x,y),flag(true),movePlan(0)
{
}
void SmartZombie::doSomething(){
    if(!isDead()){
        if(flag){
            flag=false;
            return;
        }
        flag=true;
        double Vx=0; double Vy=0;
        double destX=0;double destY=0;
        switch (getDirection()) {
            case right:
                Vx=getX()+SPRITE_WIDTH;
                Vy=getY();
                break;
            case left:
                Vx=getX()-SPRITE_WIDTH;
                Vy=getY();
                break;
            case up:
                Vx=getX();
                Vy=getY()+SPRITE_HEIGHT;
                break;
            case down:
                Vx=getX();
                Vy=getY()-SPRITE_HEIGHT;
                break;
        }
        double x=0,y=0,distance=VIEW_HEIGHT;
        
        if(getWorld()->locateNearestVomitTrigger(Vx, Vy, x, y, distance)){
            if(distance<=10){
                int r=randInt(1, 3);
                if(r==1){
                    if(!getWorld()->isVomitBlockedAt(Vx, Vy)){
                        getWorld()->addActor(new Vomit(getWorld(),Vx,Vy));
                        getWorld()->playSound(SOUND_ZOMBIE_VOMIT);
                        return;
                    }
                }
            }
        }
        if(movePlan==0){
            movePlan=randInt(3, 10);
            
            
            double px=0;double py=0;double Pdistance=VIEW_WIDTH;
            if(getWorld()->locateNearestVomitTrigger(getX(), getY(), px, py, Pdistance)){
                if(Pdistance>=80){
                    int dir=randInt(1, 4);
                    if(dir==1)
                        setDirection(right);
                    if(dir==2)
                        setDirection(left);
                    if(dir==3)
                        setDirection(up);
                    if(dir==4)
                        setDirection(down);
                }else{
                    int r2=randInt(0, 1);
                    //randomly picked a direction that get closer
                    if(py==getY()||r2==0){
                        if(px>getX())
                            setDirection(right);
                        else if(px<getX())
                            setDirection(left);
                    }
                    if(px==getX()||r2==1){
                        if(py>getY())
                            setDirection(up);
                        else if(py<getY())
                            setDirection(down);
                    }
                    
                    
                }
            }
        }
        switch (getDirection()) {
            case right:
                destX=getX()+1;
                destY=getY();
                break;
            case left:
                destX=getX()-1;
                destY=getY();
                break;
            case up:
                destX=getX();
                destY=getY()+1;
                break;
            case down:
                destX=getX();
                destY=getY()-1;
                break;
        }
        // move if not blocked
        if(!getWorld()->isblock(this, destX, destY)){
            moveTo(destX, destY);
            movePlan--;
        }
        
        movePlan=0;
    }
    
}
void SmartZombie::dieByFallOrBurnIfAppropriate(){
    if(!isDead()){
        setDead();
        getWorld()->increaseZ(-1);
        getWorld()->playSound(SOUND_ZOMBIE_DIE);
        getWorld()->increaseScore(2000);
    }
    
}
Penelope::Penelope(double x, double y, StudentWorld* sw)
:Human (sw,IID_PLAYER,x,y), mine(0), flame(0), vaccine(0)
{
    
}

void Penelope::doSomething() {
    if(!isDead()){
        increaseInfection();
        if(getInfectionDuration()==500){
            setDead();
            
            getWorld()->playSound(SOUND_PLAYER_DIE);
            return;
        }
        int ch;
        if (getWorld()->getKey(ch)) {
            switch (ch)
            {
                case KEY_PRESS_LEFT:
                    setDirection(left);
                    if (!(getWorld()->isblock(this, getX() - 4, getY()))) {
                        
                        moveTo(getX() - 4, getY());
                    }
                    break;
                case KEY_PRESS_RIGHT:
                    setDirection(right);
                    if (!(getWorld()->isblock(this, getX() + 4, getY()))) {
                        
                        moveTo(getX() + 4, getY());
                    }
                    break;
                case KEY_PRESS_DOWN:
                    setDirection(down);
                    if (!(getWorld()->isblock(this, getX(), getY() - 4))) {
                        
                        moveTo(getX(), getY() - 4);
                    }
                    break;
                case KEY_PRESS_UP:
                    setDirection(up);
                    if (!(getWorld()->isblock(this, getX(), getY() + 4))) {
                        
                        moveTo(getX(), getY() + 4);
                    }
                    break;
                case KEY_PRESS_ENTER:
                    if(getNumVaccines()>0){
                        clearInfection();
                        vaccine--;
                    }
                    break;
                case KEY_PRESS_SPACE:
                    if(getNumFlameCharges()>0){
                        //introduce 3 flame objects if not blocked
                        flame--;
                        getWorld()->playSound(SOUND_PLAYER_FIRE);
                        for(int i=1;i<4;i++){
                            
                            if(getDirection()==up){
                                if(getWorld()->isFlameBlockedAt(getX(), getY()+i*SPRITE_HEIGHT))
                                    break;
                                getWorld()->addActor(new Flame(getWorld(),getX(),getY()+i*SPRITE_HEIGHT,right));
                                
                            }
                            if(getDirection()==down){
                                if(getWorld()->isFlameBlockedAt(getX(), getY()-i*SPRITE_HEIGHT))
                                    break;
                                getWorld()->addActor(new Flame(getWorld(),getX(),getY()-i*SPRITE_HEIGHT,right));
                                
                            }
                            if(getDirection()==left){
                                if(getWorld()->isFlameBlockedAt(getX()-i*SPRITE_WIDTH, getY()))
                                    break;
                                getWorld()->addActor(new Flame(getWorld(),getX()-i*SPRITE_WIDTH,getY() ,right));
                                
                            }
                            if(getDirection()==right){
                                if(getWorld()->isFlameBlockedAt(getX()+i*SPRITE_WIDTH, getY()))
                                    break;
                                getWorld()->addActor(new Flame(getWorld(),getX()+i*SPRITE_WIDTH,getY() ,right));
                                
                            }
                            
                        }
                    }
                    break;
                case KEY_PRESS_TAB:
                    if(getNumLandmines()>0){
                        getWorld()->addActor(new Landmine(getWorld(),getX(),getY()));
                        mine--;
                    }
                    break;
                    
            }
            
        }
    }
    
    
    
}
void Penelope::useExitIfAppropriate(){
    if(!isDead()){
        if(getWorld()->getcitizenamount()==0){
            getWorld()->ExitUsed();
        }
    }
    
    
    
}
void Penelope::increaseVaccines(){
    vaccine++;
}
void Penelope::increaseLandmines(){
    mine+=2;
}
void Penelope::increaseFlameCharges(){
    flame+=5;
}
int Penelope::getNumVaccines() const{
    return vaccine;
}
int Penelope::getNumLandmines() const{
    return mine;
}
int Penelope::getNumFlameCharges() const{
    return flame;
}
void Penelope::dieByFallOrBurnIfAppropriate(){
    if(!isDead()){
        setDead();
        getWorld()->playSound(SOUND_PLAYER_DIE);
        
    }
}
void Penelope::pickUpGoodieIfAppropriate(Goodie *g){
    if(!isDead()){
        getWorld()->increaseScore(50);
        g->setDead();
        getWorld()->playSound(SOUND_GOT_GOODIE);
        g->pickUp(this);
    }
}

