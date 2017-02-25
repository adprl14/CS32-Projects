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

bool Insect::Bite(int amt){
    Actor* target = getWorld()->PickRandomInsect(getX(), getY());
    if(target!=nullptr){
        target->set_health(target->get_health() - amt);
        
        if(target->get_health() <=0)
            target->died(true);
        return true;
    }
    
    return false;
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
        return;
    }
    
    if(eat(200)){
        if(randInt(0, 1) == 0){
            stun();
            return;
        }
    }
    
    PickDirection();
    
    //SKIP TO MOVEMENT FOR PART #1
    Move();
    
    stun();
}

void BabyGrasshopper::Evolve(){
    getWorld()->AllocateActor(getX(), getY(), "AdultGrasshopper");
    die();
}


////////////////////////////////////////////////////////////////////////////////////
///////////////////////     Adult Grasshoppers CODE         ////////////////////////
////////////////////////////////////////////////////////////////////////////////////



//JUST A TEMP COPY OF BABY GRASSHOPPER
void AdultGrasshopper::doSomething(){
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
        cerr<< getX() << " " << getY() << " ";
        getWorld()->MoveActorTo(destX, destY, this);
        moveTo(destX, destY);
        cerr<< getX() << " " << getY() << endl;
        return true;

    }
    return false;

}








