#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Field.h"
#include <string>
#include<list>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld(){
    cleanUp();
}

int StudentWorld::init()
{
    Field f;
    string fieldFile = getFieldFilename();
    string error;
    // cerr << "im starting";
    if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) {
        setError(fieldFile + " " + error);
        cerr<< error;
        return GWSTATUS_LEVEL_ERROR; // something bad happened!
    }
    
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            
            Field::FieldItem item =  f.getContentsOf(i, j);
            switch (item) {
                    
                case Field::rock:
                {
                    Actor* p= new Pebble(this, i, j );
                    ActorGrid[i][j].push_back(p);
                }
                    break;
                
                case Field::food:
                {
                    Actor *p = new Food(this, i, j, 6000);
                    ActorGrid[i][j].push_back(p);
                }
                    break;
                    
                case Field::grasshopper:
                {
                    Actor* p= new BabyGrasshopper(this, i, j );
                    ActorGrid[i][j].push_back(p);
                    //  cerr<< "G: (" << i << "," << j << ")" << endl;
                }
                    break;
                    
                    
                default:
                    break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}


int StudentWorld::move()
{
    Ticks++;
    
    ResetMove();
    
    MakeActorsDoSomething();
    
    killDeadActors();
  
    return GWSTATUS_CONTINUE_GAME;

    }

    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.
    //    ActorGrid[(*it)->getX()][(*it)->getY()].push_back((*it));
    //    it = find(ActorGrid[(*it)->getX()-1][(*it)->getY()].begin(), ActorGrid[(*it)->getX()-1][(*it)->getY()].end(), (*it));
    //
    //    ActorGrid[(*it)->getX()-1][(*it)->getY()].erase(it);



void StudentWorld::cleanUp(){
    
//    for(int i=0; i<VIEW_WIDTH; i++){
//        for(int j = 0; j<VIEW_HEIGHT; j++){
//            if(ActorGrid[i][j].empty())
//                continue;
//            
//            for(list<Actor*>::iterator it = ActorGrid[i][j].begin(); it != ActorGrid[i][j].end();){
//                delete *it;
//                it = ActorGrid[i][j].erase(it);
//            }
//            
//        }
//    }
}

///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////      Other Functions       //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

bool StudentWorld::ActorListEmpty (int x, int y) const
{
    return ActorGrid[x][y].empty();
}

bool StudentWorld::Blocked(int x, int y) const{
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->DoIBlockInsects()){
            return true;
        }
    }
    return false;
}

bool StudentWorld::isThereFood(int x, int y) const{
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->AmIFood()){
            return true;
        }
    }
    return false;
}

void StudentWorld::AddFood(int x, int y, int amt){
    if(isThereFood(x,y)){
        std::list<Actor*>::const_iterator it;
        for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
            if((*it)->AmIFood()){
                break;
            }
        }
        (*it)->set_health((*it)->get_health() + amt);
    }
    
    else{
        Actor *p = new Food (this, x, y, amt);
        ActorGrid[x][y].push_back(p);
    }
}

void StudentWorld::ResetMove(){
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            if(ActorGrid[i][j].empty())
                continue;
            for(list<Actor*>::iterator it = ActorGrid[i][j].begin(); it != ActorGrid[i][j].end();it++)
            {
                (*it)->DirecMoved(Actor::Direction::none);
            }
            
        }
    }

}

void StudentWorld::MakeActorsDoSomething(){
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            if(ActorGrid[i][j].empty())
                continue;
            for(list<Actor*>::iterator it2 = ActorGrid[i][j].begin(); it2 != ActorGrid[i][j].end();it2++)
            {
                if((*it2)->WheredidIMove()!= Actor::Direction::none)
                    continue;
                
                if(!((*it2)->AmIDead()) && ((*it2)->get_health())> 0){
                    cerr<< i <<" " <<j << " ";
                    cerr<< (*it2)->getX()<<" " <<(*it2)->getY() << endl;
                    
                    (*it2)->doSomething();
                    MoveActorTo(it2, (*it2)->WheredidIMove());
                }
                
                
            }
        }
    }
    
}

void StudentWorld::killDeadActors(){
    
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            if(ActorGrid[i][j].empty())
                continue;
            for(list<Actor*>::iterator it = ActorGrid[i][j].begin(); it != ActorGrid[i][j].end();it++)
            {
                if((*it)->get_health() <=0 && (*it)->AmIDead()){
                    delete (*it);
                    ActorGrid[i][j].erase(it);
                }
            }
            
        }
    }
}


void StudentWorld::MoveActorTo (list<Actor*>::iterator it, Actor::Direction direc){
    int x = (*it)->getX();
    int y = (*it)->getY();
    switch (direc) {
        case GraphObject::up:
            ActorGrid[x][y].push_back(*it);
            ActorGrid[x][y-1].erase(it);
            break;
            
        case GraphObject::down:
            ActorGrid[x][y].push_back(*it);
            ActorGrid[x][y+1].erase(it);

            break;
            
        case GraphObject::left:
            ActorGrid[x][y].push_back(*it);
            ActorGrid[x+1][y].erase(it);

            break;
            
        case GraphObject::right:
            ActorGrid[x][y].push_back(*it);
            ActorGrid[x-1][y].erase(it);

            break;
            
        default:
            break;
    }
}



// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


