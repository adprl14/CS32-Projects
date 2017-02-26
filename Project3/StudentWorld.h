#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
        Ticks = 0;
    }
    
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    bool ActorListEmpty(int x, int y) const;
    bool Blocked(int x, int y) const;
    bool isThereFood(int x, int y) const;

    void AddFood(int x, int y, int amt);
    void ResetMove();
    void MakeActorsDoSomething();
    void killDeadActors();
    void MoveActorTo (std::list<Actor*>::iterator it, Actor::Direction direc);
    void MoveActorTo (int destx, int desty, Actor* act);

    void AllocateActor(int x, int y, std::string type);
    Actor* PickRandomInsect (int x, int y);
    
    int GiveInsectFood(int x, int y, int amt);
    
    void StunAllStunableActors (int x, int y, Actor* water);
    void PoisonAllPoisonableActors (int x, int y, Actor* poison);

    

    
private:
    std::list<Actor*> ActorGrid[VIEW_WIDTH][VIEW_HEIGHT];
    int Ticks;
    
};

#endif // STUDENTWORLD_H_
