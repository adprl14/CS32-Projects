#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor:public GraphObject{
public:
    Actor(StudentWorld* sWorld, int ImID, int x, int y, Direction dir, int depth):GraphObject(ImID,x,y, dir,depth ){
        m_sw = sWorld;
    }
    
    virtual void doSomething() = 0;
    void died(bool died){isDead = died;}
    void set_health(int hp){m_health=hp;}
    
    StudentWorld* getWorld() const{return m_sw;}
    int get_health() const{ return m_health;}
    bool AmIDead() const {return isDead;}
    
    
    virtual bool AmIBlocked(int x, int y) const;
    virtual bool DoIBlockInsects() const {return false;}
    virtual bool AmIInsect() const {return false;}
    virtual bool AmIFood() const{return false;}
    
    void DirecMoved(Direction Moved){
        MovedDirec = Moved;
    }
    
    Direction WheredidIMove() const{
        return MovedDirec;
    }
    
    void FaceRandDirec(){
        Direction newDirection = static_cast<Direction>((randInt(1,4)));
        setDirection(newDirection);
    }
    
private:
    StudentWorld* m_sw;
    int m_health;
    bool isDead;
    Direction MovedDirec;
    
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                Pebble                    ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


class Pebble:public Actor{
public:
    Pebble(StudentWorld* sWorld, int x, int y):Actor(sWorld, IID_ROCK, x, y, right, 1){
        died(false); //keep pebbles alive so don't delete
        set_health(0);
        DirecMoved(none);
        //        std::cerr << "(" << getX() <<"," << getY()<< ") ";
    }
    
    virtual void doSomething(){
        //pebbles are boring
    }
    
    virtual bool DoIBlockInsects() const{
        return true;
    }
    
    
private:
    
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                GRASSHOPPERS              ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
class Insect: public Actor{
public:
    Insect(StudentWorld* sWorld, int ImID, int x, int y, Direction direc, int depth):Actor(sWorld, ImID,x,y, direc, depth)
    {
        sleep_ticks = 0;
    }
    
    virtual void doSomething() = 0;
    void eat(){ /*DUMMY*/}
    void doInsectStuff();
    virtual void sleep(){sleep_ticks--;}
    void stun(){sleep_ticks = 2;}
    void die();
    virtual void Move()=0;

    bool AmISleep() const {return !(sleep_ticks==0);}
    virtual bool AmIInsect() const{
        return true;
    }
    
private:
    int sleep_ticks;
};

//Still Abstract base
class Grasshopper: public Insect{
public:
    Grasshopper(StudentWorld* sWorld, int ImID, int x, int y, Direction direc, int depth):Insect(sWorld, ImID,x,y, direc, depth){
        DistToMove = randInt(2, 10);
    }
    virtual void doSomething() = 0;
    void doGHstuff(); //Grasshopper common actions
    virtual void Move();
    
    
    void Moved1Spot(){DistToMove--;}
    void SetDistToZero(){DistToMove = 0;}
    int DistRemaining() const{ return DistToMove;}
    void PickDirection(){
        if(DistToMove <= 0){
            FaceRandDirec();
            DistToMove = randInt(2, 10);
        }
    }
private:
    int DistToMove;
};



class BabyGrasshopper:public Grasshopper{
public:
    BabyGrasshopper(StudentWorld* sWorld, int x, int y):Grasshopper(sWorld, IID_BABY_GRASSHOPPER, x, y, static_cast<Direction>((randInt(1,4))), 0)
    {
        DirecMoved(none);
        set_health(500);
        died(false);
    }
    
    virtual void doSomething();
    void Evolve(); //NOT IMPLEMENTED YET
   
private:
  
};

class AdultGrasshopper:public Grasshopper{
    AdultGrasshopper(StudentWorld* sWorld, int x, int y):Grasshopper(sWorld, IID_ADULT_GRASSHOPPER, x, y, static_cast<Direction>((randInt(1,4))), 0)
    {
        
    }
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                ANTS                      ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////









//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                OBJECTS                   ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class Food: public Actor{
public:
    Food(StudentWorld* sWorld,int x, int y, int food):Actor( sWorld, IID_FOOD, x, y, right, 2){
        set_health(food);
        died(false);
    }
    
    virtual void doSomething(){}
    virtual bool AmIFood() const{return true;}

    void addFood(int i){
        int newFood = get_health() + i;
        set_health(newFood);
    }
    
private:
    
};

#endif // ACTOR_H_
