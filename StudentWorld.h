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
    

    
private:
    std::list<Actor*> ActorGrid[VIEW_WIDTH][VIEW_HEIGHT];
    int Ticks;
    
    
    
    
    
    //    void ActorIter ( void (*f)(int i, int j)){
    //        for(int i=0; i<VIEW_WIDTH; i++)
    //        {
    //            for(int j = 0; i<VIEW_HEIGHT; j++)
    //            {
    //
    //                f(i,j);
    //
    //            }
    //
    //        }
    //    }
    //
    //    void CleanActors(int i,int j){
    //        if(!ActorContainer[i][j].empty())
    //        {
    //        for(std::list<Actor*>::iterator it = ActorContainer[i][j].begin(); it != ActorContainer[i][j].end();){
    //            delete *it;
    //            it = ActorContainer[i][j].erase(it);
    //        }
    //    }
    //    }
    
};

#endif // STUDENTWORLD_H_
