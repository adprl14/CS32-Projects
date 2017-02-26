#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include <algorithm>
#include <utility>
#include  <cmath>
using namespace std;


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp





////////////////////////////////////////////////////////////////////////////////////
///////////////////////        HELPER CODE         /////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

bool Actor::AmIBlocked(int x, int y) const{
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
    Actor* target = getWorld()->PickRandomInsect(getX(), getY());
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
                    cerr<< "(" << target->getX() << "," << target->getY()<< ") Bite-back " << "(" << getX() << "," << getY()<< ")" <<endl;
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
        //ADD FOOD HERE
        //Insect dies :(
        die();
        return;
    }
    
    if(AmISleep()){
        sleep();
        return;
    }
    
    if(WillIBite()){
        Bite(50);
    }
    else if (WillIJump() && Jump()){
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

//Jumps an Adult Grasshopper to a random location within a circular radius of 10
//If successful returns true, otherwise returns false(i.e all positions within radius of 10 are blocked)
bool AdultGrasshopper::Jump(){
    double m_x = getX();
    double m_y = getY();
    std::vector<pair<double,double>> JumpVec;
    
    for (double i = 1; i<VIEW_WIDTH; i+=1){
        for (double j = 1; j<VIEW_HEIGHT; j+=1){
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
        return true;

    }
    return false;

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
