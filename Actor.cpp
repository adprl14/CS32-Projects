#include "Actor.h"
#include "StudentWorld.h"
#include "GraphObject.h"
#include "GameConstants.h"
#include <algorithm>


// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp




void Insect::doInsectStuff(){
    
    set_health(get_health() - 1);
    
    
    
  
    
}

void BabyGrasshopper::doSomething(){
    
    Insect::doInsectStuff();
    
    if(get_health() <= 0){
        //must ADD FOOD HERE
        //Insect dies :(
        die();
        return;
    }
    
    if(AmISleep()){
        sleep();
        return;
    }
    
    if(WheredidIMove()!=Actor::Direction::none)
        return;
    
    PickDirection();
    
    //SKIP TO MOVEMENT FOR PART #1
    Move();
    
    stun();
}


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

void Insect::die(){
    died(true);
    getWorld()->AddFood(getX(), getY(), 100);
}

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






