#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"
#include <algorithm>
#include <utility>
#include  <cmath>
using namespace std;


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp





////////////////////////////////////////////////////////////////////////////////////
///////////////////////        HELPER CODE         /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

bool Actor::AmIBlocked(const int& x,const int& y) const{
    if(m_sw->ActorListEmpty(x, y)){
        return false;
    }
    //only insects will ever need this function
    if(m_sw->Blocked(x,y))
    {
        return true;
    }
    
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////        Insects CODE         /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
void Insect::doInsectStuff(){
    
    set_health(get_health() - 1);
    
}

//Set isDead = true and add food to current spot
void Insect::die(){
    died(true);
    getWorld()->AddFood(getX(), getY(), 100);
}

//If Alive Food exists eat it and return true
bool Insect::eat(int amt){
    if(getWorld()->isThereFood(getX(), getY())){
        int meal = getWorld()->GiveInsectFood(getX(), getY(), amt);
        set_health(get_health() + meal);
        return true;
    }
    return false;
}

//Bites a random target at same location
bool Insect::Bite(int amt){
    Actor* target = getWorld()->PickRandomInsect(getX(), getY(), this);
    if(target!=nullptr){
        target->set_health(target->get_health() - amt);
        
      //  cerr<< "I bit " << "(" << target->getX() << "," << target->getY()<< ") with " << target->get_health() << endl;
        if(target->get_health() <=0){
            target->die();
            cerr << "target killed" << endl;
        }
        if(!target->AmIDead()){
            AdultGrasshopper* ap =dynamic_cast<AdultGrasshopper*>(target);
            if(ap!=nullptr){
                if(randInt(0, 1)==0){
                    cerr<< target<< " (" << target->getX() << "," << target->getY()<< ") Bite-back " <<  this<< " (" << getX() << "," << getY()<< ")" <<endl;
                    ap->Bite(50);
                }
            }
        }
        
        return true;
    }
    
    return false;
}

void Insect::poison(){
    set_health(get_health()-150);
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////     BASE Grasshoppers CODE         /////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void Grasshopper::Move(){
    Direction curDirec = getDirection();
    switch (curDirec) {
        case GraphObject::right:
            if(AmIBlocked(getX()+1, getY())){
                SetDistToZero();
                stun();
                return;
            }
            DirecMoved(right);
            Moved1Spot();
            moveTo(getX()+1, getY());
            break;
            
        case GraphObject::left:
            if(AmIBlocked(getX()-1, getY())){
                SetDistToZero();
                stun();
                return;
            }
            DirecMoved(left);
            Moved1Spot();
            
            moveTo(getX()-1, getY());
            break;
            
        case GraphObject::up:
            if(AmIBlocked(getX(), getY()+1)){
                SetDistToZero();
                stun();
                return;
            }
            DirecMoved(up);
            Moved1Spot();
            moveTo(getX(), getY()+1);
            break;
            
        case GraphObject::down:
            if(AmIBlocked(getX(), getY()-1)){
                SetDistToZero();
                stun();
                return;
            }
            DirecMoved(down);
            Moved1Spot();
            
            moveTo(getX(), getY()-1);
            break;
            
        default:
            break;
    }
    StunnedMe(nullptr);
    PoisonedMe(nullptr);

}

void Grasshopper::PickDirection(){
    if(DistToMove <= 0){
        FaceRandDirec();
        DistToMove = randInt(2, 10);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////     Baby Grasshoppers CODE         /////////////////////////
////////////////////////////////////////////////////////////////////////////////////

void BabyGrasshopper::doSomething(){
    if(WheredidIMove()!=Actor::Direction::none)
        return;
    
    Insect::doInsectStuff();
    
    if(get_health() <= 0){
        //ADD FOOD HERE
        //Insect dies :(
        die();
        return;
    }
    
    if(AmISleep()){
        sleep();
        return;
    }
    
    if(get_health()>=1600){
        Evolve();
        cerr << "I Evolved!" << endl;
        return;
    }
    
    if(eat(200)){
        if(randInt(0, 1) == 0){
            stun();
            return;
        }
    }
    
    PickDirection();
    
    Move();
    
    stun();
}

//makes BabyGrasshopper die off and allocates new AdultGrasshopper object in its place
void BabyGrasshopper::Evolve(){
    getWorld()->AllocateActor(getX(), getY(), "AdultGrasshopper");
    die();
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////     Adult Grasshoppers CODE         ////////////////////////
////////////////////////////////////////////////////////////////////////////////////



//Adult Grasshopper AI
void AdultGrasshopper::doSomething(){
    //if not true then the grasshopper already moved this turn
    if(WheredidIMove()!=Actor::Direction::none)
        return;
    
    Insect::doInsectStuff();
    
    if(get_health() <= 0){
        die();
        return;
    }
    
    if(AmISleep()){
        sleep();
        return;
    }
    
    if(WillIBite()){
        GHbite();
    }
    else if (WillIJump()){
        Jump();
    }
    else{
        if(eat(200)){
            if(randInt(0, 1)==0)
                goto sleep;
        }
        PickDirection();
        Move();
    }

    sleep:
        stun();
    
}

//Jumps an Adult Grasshopper to a random location within a circular radius of 10 not including current location
void AdultGrasshopper::Jump(){
    double m_x = getX();
    double m_y = getY();
    std::vector<pair<double,double>> JumpVec;
    
    for (double i = 0; i<VIEW_WIDTH; i+=1){
        for (double j = 0; j<VIEW_HEIGHT; j+=1){
           if(i== m_x || j == m_y)
               continue;
            double x_dist = abs(m_x - i);
            double y_dist = abs(m_y - j);
            double radius = sqrt(pow(x_dist, 2) + pow(y_dist, 2));
            if(radius <=10 && !(getWorld()->Blocked(i, j))){
                JumpVec.push_back(make_pair(i, j));
            }
        }
    }
    if(JumpVec.size() > 0){
        int JumpSpot = randInt(0, JumpVec.size()-1);
        double destX = JumpVec[JumpSpot].first;
        double destY = JumpVec[JumpSpot].second;
      //  cerr<< "(" << getX() << " " << getY() << ")" << " to ";
        getWorld()->MoveActorTo(destX, destY, this);
       moveTo(destX, destY);
       // cerr<< "(" <<getX() << " " << getY() << ")" << endl;

    }

}
bool AdultGrasshopper::WillIBite(){
    bool BiteChance = (randInt(0, 2)==0);
    bool OtherInsect = (getWorld()->PickRandomInsect(getX(), getY(), this)!=nullptr);
    return (BiteChance&&OtherInsect);
}

//If there is another place to jump to return true 1/10 of the time otherwise false
bool AdultGrasshopper::WillIJump(){
    double m_x = getX();
    double m_y = getY();
    
    for (double i = 0; i<VIEW_WIDTH; i+=1){
        for (double j = 0; j<VIEW_HEIGHT; j+=1){
            if(i== m_x || j == m_y)
                continue;
            double x_dist = abs(m_x - i);
            double y_dist = abs(m_y - j);
            double radius = sqrt(pow(x_dist, 2) + pow(y_dist, 2));
            if(radius <=10 && !(getWorld()->Blocked(i, j))){
                return (randInt(0, 9)==0);
            }
        }
    }
    return false;
    
}

////////////////////////////////////////////////////////////////////////////////////
///////////////////////                 ANT CODE                    ////////////////
////////////////////////////////////////////////////////////////////////////////////
void Ant::doSomething(){
    doInsectStuff();
    
    if(get_health() <= 0){
        die();
        return;
    }
    
    if(AmISleep()){
        sleep();
        return;
    }
    FaceRandDirec();
    Move();
    stun();
}

void Ant::Move(){
    Direction curDirec = getDirection();
    switch (curDirec) {
        case GraphObject::right:
            if(AmIBlocked(getX()+1, getY())){
                                stun();
                return;
            }
            DirecMoved(right);
            moveTo(getX()+1, getY());
            break;
            
        case GraphObject::left:
            if(AmIBlocked(getX()-1, getY())){
                stun();
                return;
            }
            DirecMoved(left);
            moveTo(getX()-1, getY());
            break;
            
        case GraphObject::up:
            if(AmIBlocked(getX(), getY()+1)){
                stun();
                return;
            }
            DirecMoved(up);
            moveTo(getX(), getY()+1);
            break;
            
        case GraphObject::down:
            if(AmIBlocked(getX(), getY()-1)){
                stun();
                return;
            }
            DirecMoved(down);            
            moveTo(getX(), getY()-1);
            break;
            
        default:
            break;
    }
    StunnedMe(nullptr);
    PoisonedMe(nullptr);
    
}

bool Ant::RunCommand(const Compiler::Command& c){
    
    if (c.opcode == Compiler::goto_command){
        
    }
    else if (c.opcode == Compiler::if_command){
            
    }
    else if (c.opcode == Compiler::emitPheromone){
            
    }
    else if (c.opcode == Compiler::faceRandomDirection){
            
    }
    else if (c.opcode == Compiler::rotateClockwise){
        rotateTheAntClockwise();
    }
    else if (c.opcode == Compiler::rotateCounterClockwise){
        rotateTheAntCounterClockwise();
    }
    
    else if (c.opcode == Compiler::moveForward){
        Move();
    }
    else if (c.opcode == Compiler::bite){
        AntBite();
    }
    else if (c.opcode == Compiler::pickupFood){
        FoodPickUp();
    }
    else if (c.opcode == Compiler::dropFood){
        FoodDrop();
    }
    else if (c.opcode == Compiler::eatFood){
        
    }
    else if (c.opcode == Compiler::generateRandomNumber){
        
    }
    
    return false;
}

void Ant::rotateTheAntClockwise(){
    switch (getDirection()) {
        case GraphObject::up:
            setDirection(right);
            break;
        case GraphObject::right:
            setDirection(down);
            break;
        case GraphObject::down:
            setDirection(left);
            break;
        case GraphObject::left:
            setDirection(up);
            break;
        default:
            break;
    }
}

void Ant::rotateTheAntCounterClockwise(){
    switch (getDirection()) {
        case GraphObject::up:
            setDirection(left);
            break;
        case GraphObject::left:
            setDirection(down);
            break;
        case GraphObject::down:
            setDirection(right);
            break;
        case GraphObject::right:
            setDirection(up);
            break;
        default:
            break;
    }
}

void Ant::FoodPickUp(){
    int PickUpAmt(0);
    if(getFood()<MAX_ANT_FOOD){
        if(MAX_ANT_FOOD-getFood()>=400)
            PickUpAmt = 400;
        else
            PickUpAmt = MAX_ANT_FOOD-getFood();
    }
    
    int MoreFood = getWorld()->GiveInsectFood(getX(), getY(), PickUpAmt);
    setFood(getFood()+MoreFood);
}


void Ant::FoodDrop(){
    getWorld()->AddFood(getX(), getY(), getFood());
    setFood(0);
}



////////////////////////////////////////////////////////////////////////////////////
///////////////////////             ANTHILL CODE                    ////////////////
////////////////////////////////////////////////////////////////////////////////////

void AntHill::doSomething(){
    set_health(get_health()-1);
   
    if(get_health() <= 0){
        died(true);
        return;
    }
    
    if(getWorld()->isThereFood(getX(), getY())){
        eat(10000);
        return;
    }
    
    if(get_health()>=2000){
        ProduceAnt();
    }

    
}

void AntHill::ProduceAnt(){
    getWorld()->MakeAnt(getX(), getY(), getColony(), getCompiler());
    set_health(get_health()-1500);
    antsProduced++;
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////     All PuddleBase and derived CODE         ////////////////
////////////////////////////////////////////////////////////////////////////////////

void PoolOfWater::doSomething(){
    getWorld()->StunAllStunableActors(getX(), getY(), this);
}

void Poison::doSomething(){
    getWorld()->PoisonAllPoisonableActors(getX(), getY(), this);
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////         OBJECTS CODE                        ////////////////
////////////////////////////////////////////////////////////////////////////////////

void Pheromone::doSomething(){
    set_health(get_health() -1);
    
    if(get_health() <= 0){
        died(true);
    }
}
