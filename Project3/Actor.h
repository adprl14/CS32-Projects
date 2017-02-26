#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include "Compiler.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

enum type {Baby, Adult,FoodType,Water,PoisonType,pebble,Pher0,Pher1,Pher2,Pher3,Ant0,Ant1,Ant2,Ant3 };
enum ColonyNum {zero, one, two, three};

class Actor:public GraphObject{
public:
    Actor(StudentWorld* sWorld, int ImID, int x, int y, Direction dir, int depth):GraphObject(ImID,x,y, dir,depth ){
        m_sw = sWorld;
        StunnedBy = nullptr;
        PoisonedBy = nullptr;
        DirecMoved(none);
        died(false);

    }
    
    virtual ~Actor(){};
    
    virtual void doSomething() = 0;
    void died(bool died){isDead = died;}
    void set_health(int hp){m_health=hp;}
    
    StudentWorld* getWorld() const{return m_sw;}
    int get_health() const{ return m_health;}
    bool AmIDead() const {return isDead;}
    virtual void stun(){}
    virtual void poison(){}
    virtual void die(){};
    
    virtual bool AmIBlocked(int x, int y) const;
    virtual bool DoIBlockInsects() const {return false;}
    virtual bool AmIInsect() const {return false;}
    virtual bool AmIFood() const{return false;}
    virtual bool IsStunable() const{return false;}
    virtual bool IsPoisonable() const{return false;}
    Actor* poison_er() const {return PoisonedBy;}
    Actor* stun_er() const {return StunnedBy;}

    void StunnedMe(Actor* water){StunnedBy = water;}
    void PoisonedMe(Actor* poison){PoisonedBy = poison;}

    void DirecMoved(Direction Moved){MovedDirec = Moved;}
    Direction WheredidIMove() const{return MovedDirec;}

protected:
    void FaceRandDirec(){
        Direction newDirection = static_cast<Direction>((randInt(1,4)));
        setDirection(newDirection);
    }
    
private:
    StudentWorld* m_sw;
    Actor* StunnedBy;
    Actor* PoisonedBy;
    int m_health;
    bool isDead;
    type m_type;
    Direction MovedDirec;
    
};


//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////              Insect BASE                 ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
class Insect: public Actor{
public:
    Insect(StudentWorld* sWorld, int ImID, int x, int y, Direction direc, int depth):Actor(sWorld, ImID,x,y, direc, depth)
    {
        sleep_ticks = 0;
    }
    
    virtual void doSomething() = 0;
    virtual void stun(){sleep_ticks += 2;}
    void die();
    virtual void poison();
    
    virtual bool IsStunable() const{return true;}
    virtual bool IsPoisonable() const{return true;}

    bool AmISleep() const {return !(sleep_ticks<=0);}
    virtual bool AmIInsect() const{return true;}
    
protected:
    void doInsectStuff();
    virtual void sleep(){sleep_ticks--;}
    virtual bool eat(int amt);
    virtual bool Bite(int amt);
    virtual void Move(){}


    
private:
    int sleep_ticks;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                GRASSHOPPERS              ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

//Still Abstract base
class Grasshopper: public Insect{
public:
    Grasshopper(StudentWorld* sWorld, int ImID, int x, int y, Direction direc, int depth):Insect(sWorld, ImID,x,y, direc, depth){
        DistToMove = randInt(2, 10);
    }
    
    void Moved1Spot(){DistToMove--;}
    void SetDistToZero(){DistToMove = 0;}
    int DistRemaining() const{ return DistToMove;}
    void PickDirection();
    
protected:
    virtual void Move();


private:
    int DistToMove;
    
};



class BabyGrasshopper:public Grasshopper{
public:
    BabyGrasshopper(StudentWorld* sWorld, int x, int y):Grasshopper(sWorld, IID_BABY_GRASSHOPPER, x, y, static_cast<Direction>((randInt(1,4))), 1)
    {
        set_health(500);
    }
    
    virtual bool Bite(int amt){return false;}
    virtual void doSomething();
    void Evolve();
   
private:
  
};

class AdultGrasshopper:public Grasshopper{
public:
    AdultGrasshopper(StudentWorld* sWorld, int x, int y):Grasshopper(sWorld, IID_ADULT_GRASSHOPPER, x, y, static_cast<Direction>((randInt(1,4))), 1)
    {
        set_health(1600);

    }
    
    
    virtual void doSomething();
    virtual bool IsStunnable() {return false;}
    virtual bool IsPoisonable() const{return false;}
    virtual void poison(){}

private:
    void Jump();
    bool WillIBite();
    bool WillIJump();
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                ANTS                      ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class Ant:public Insect{
public:
    Ant(StudentWorld* sWorld, int AntID, ColonyNum Num, int x, int y):Insect(sWorld, AntID, x, y,static_cast<Direction>((randInt(1,4))), 1){
        set_health(1500);
        WasBitten=false;
        WasBlocked=false;
        lastRandomNumber = 0;
        startInstructCount = 0;
        m_colony=Num;
    }
    
private:
    bool WasBitten;
    bool WasBlocked;
    int lastRandomNumber;
    int startInstructCount;
    ColonyNum m_colony;
};

class AntHill:public Insect{
public:
    AntHill(StudentWorld *sWorld, ColonyNum Num, int x, int y, Compiler *comp):Insect(sWorld, IID_ANT_HILL, x, y,right, 2){
        set_health(8999);
        m_compiler = comp;
        m_colony=Num;
    }
    
    ~AntHill(){
        delete m_compiler;
    }
    
    virtual void doSomething(){}
    virtual bool IsStunnable() {return false;}
    virtual bool IsPoisonable() const{return false;}
    virtual bool AmIInsect() const{return false;}


private:
    Compiler *m_compiler;
    ColonyNum m_colony;
};





//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                OBJECTS                   ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class Food: public Actor{
public:
    Food(StudentWorld* sWorld,int x, int y, int food):Actor( sWorld, IID_FOOD, x, y, right, 2){
        set_health(food);

    }

    virtual void doSomething(){}
    virtual bool AmIFood() const{return true;}

    void addFood(int i){
        int newFood = get_health() + i;
        set_health(newFood);
    }
    
private:
    
};

class Pheromone: public Actor{
public:
    Pheromone(StudentWorld* sWorld, int PheromoneID,ColonyNum Num, int x, int y):Actor(sWorld, PheromoneID, x, y,right,2){
        set_health(256);
        m_colony=Num;
    }
    
    virtual void doSomething();
private:
    ColonyNum m_colony;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                Puddle OBJ                   ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

class PuddleBase: public Actor{
public:
    PuddleBase(StudentWorld *sWorld,int ImId, int x, int y ):Actor(sWorld, ImId, x, y, right, 2){
        set_health(0);

    }
private:
    
};

class PoolOfWater:public PuddleBase{
public:
    PoolOfWater(StudentWorld *sWorld, int x, int y):PuddleBase(sWorld, IID_WATER_POOL, x, y){

    }
    
    virtual void doSomething();
    
private:
};

class Poison:public PuddleBase{
public:
    Poison(StudentWorld *sWorld, int x, int y):PuddleBase(sWorld, IID_POISON, x, y){

    }
    virtual void doSomething();

private:
};

//////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                Pebble                    ////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////


class Pebble:public Actor{
public:
    Pebble(StudentWorld* sWorld, int x, int y):Actor(sWorld, IID_ROCK, x, y, right, 1){
        set_health(0);

    }
    
    virtual void doSomething(){    }
    
    virtual bool DoIBlockInsects() const{
        return true;
    }
    
    
private:
    
};



#endif // ACTOR_H_
