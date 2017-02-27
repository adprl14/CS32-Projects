#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include "Compiler.h"
#include <string>
#include <list>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
const int MAX_TICKS = 2000;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
        Ticks = 0;
        for(int i = 0; i<4; i++){
            antsProducedBy[i].first =0;
            antsProducedBy[i].second = 0;
            
        }
        for(int i = 0; i<4; i++){
            compilerForEntrant[i] = nullptr;
        }
    }
    
    virtual ~StudentWorld();
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    
    
    bool ActorListEmpty(int x, int y) const;
    bool Blocked(int x, int y) const;
    bool isThereFood(int x, int y) const;
    bool isThereType(const int& x, const int& y, type Type) const;
    bool isDangerHere(const int &x, const int &y,const Ant* ptrToAnt) const;
    bool isPherInFront(const Actor* ptrToActor) const;
    Actor* GetFirstType(const int &x, const int &y, type Type);


    void AddFood(int x, int y, int amt);
    void ResetMove();
    void MakeActorsDoSomething();
    void killDeadActors();
    void MoveActorTo (std::list<Actor*>::iterator it, Actor::Direction direc);
    void MoveActorTo (int destx, int desty, Actor* act);

    void AllocateActor(int x, int y, type Type);
    Actor* PickRandomInsect (int x, int y, Actor *self);
    
    int GiveInsectFood(int x, int y, int amt);
    
    void StunAllStunableActors (int x, int y, Actor* water);
    void PoisonAllPoisonableActors (int x, int y, Actor* poison);
    
    void MakeType(const int&x,const int&y, type Type);
    void addPherTo(const int&x,const int&y,type PherType);
    void MakeAnt(int x, int y,int Num, Compiler *comp);
    void AddAnt(int Num);
    Actor* GetWinningAnt();
    int GetWinningAntNumber();
    bool isThereAWinningAnt();
    std::string GetWinningAntName();
    std::string getDisplayText();

    

    
private:
    std::list<Actor*> ActorGrid[VIEW_WIDTH][VIEW_HEIGHT];
    int Ticks;
    std::pair<int,int> antsProducedBy[4];
    std::pair<int,int> ColonyCoord[4];
    Compiler *compilerForEntrant[4];
};

#endif // STUDENTWORLD_H_
