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
                
                case Field::water:
                {
                    Actor* p= new PoolOfWater(this, i, j );
                    ActorGrid[i][j].push_back(p);
                }
                    break;
               
                case Field::poison:
                {
                    Actor* p= new Poison(this, i, j );
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
  
    if(Ticks==2000)
        return GWSTATUS_NO_WINNER;
    
    return GWSTATUS_CONTINUE_GAME;

}

    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_NO_WINNER will cause our framework to end the simulation.




void StudentWorld::cleanUp(){
    
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            if(ActorGrid[i][j].empty())
                continue;
            
            for(list<Actor*>::iterator it = ActorGrid[i][j].begin(); it != ActorGrid[i][j].end();){
                delete *it;
                it = ActorGrid[i][j].erase(it);
            }
            
        }
    }
}












///////////////////////////////////////////////////////////////////////////////////////
/////////////////////////      Other Functions       //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

//returns true if there are no actors at x,y
bool StudentWorld::ActorListEmpty (int x, int y) const
{
    return ActorGrid[x][y].empty();
}

//Returns true if insect is blocked by object at x,y
bool StudentWorld::Blocked(int x, int y) const{
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->DoIBlockInsects()){
            return true;
        }
    }
    return false;
}

//returns true if there is a food Object at (x,y) otherwise false
bool StudentWorld::isThereFood(int x, int y) const{
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->AmIFood()&& !((*it)->AmIDead()) ){
            return true;
        }
    }
    return false;
}

//Adds food of amt amount to the Food Object at (x,y). If no Food Object a new one is allocated with amt health
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

//Resets all Actors DirecMoved to none so that they can move again when doSomething is called
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

//calls doSomething() on all Actors in play, moves their position in grid if necessary
void StudentWorld::MakeActorsDoSomething(){
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            if(ActorGrid[i][j].empty())
                continue;
            for(list<Actor*>::iterator it2 = ActorGrid[i][j].begin(); it2 != ActorGrid[i][j].end();it2++)
            {
                if((*it2)->WheredidIMove()!= Actor::Direction::none)
                    continue;
                
                if(!((*it2)->AmIDead()) ){
//                    cerr<< i <<" " <<j << " ";
//                    cerr<< (*it2)->getX()<<" " <<(*it2)->getY() << endl;
                    
                    (*it2)->doSomething();
                    MoveActorTo(it2, (*it2)->WheredidIMove());
                }
                
                
            }
        }
    }
    
}

//deletes any actors with isDead = true
void StudentWorld::killDeadActors(){
    
    for(int i=0; i<VIEW_WIDTH; i++){
        for(int j = 0; j<VIEW_HEIGHT; j++){
            if(ActorGrid[i][j].empty())
                continue;
            for(list<Actor*>::iterator it = ActorGrid[i][j].begin(); it != ActorGrid[i][j].end();it++)
            {
                if((*it)->AmIDead()){
                    delete (*it);
                    ActorGrid[i][j].erase(it);
                }
            }
            
        }
    }
}

//If an actor moved its graphical position, this function moves the actors spot in the Grid to the spot corresponding its new graphical location
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

//MOVES ACTOR TO ORIGINAL SPOT IN GRID to new destination
//CAUTION: DO NOT USE THIS FUNCTION AFTER USING moveTo method
void StudentWorld::MoveActorTo (int destx, int desty, Actor* act){
    int m_x = act->getX();
    int m_y = act->getY();
    
    list<Actor*>::iterator it;
    for( it = ActorGrid[m_x][m_y].begin(); it != ActorGrid[m_x][m_y].end();it++)
    {
        if((*it) == act)
            break;
    }
    ActorGrid[destx][desty].push_back(*it);
    ActorGrid[m_x][m_y].erase(it);

    
}


//Dynamically allocates new Actor of class "type" into ActorGrid[x][y]
void StudentWorld::AllocateActor(int x, int y, string type){
    if(type == "AdultGrasshopper"){
        Actor *p = new AdultGrasshopper(this, x, y);
        ActorGrid[x][y].push_back(p);
    }
    
}


//PICKS RANDOM ALIVE INSECT, RETURN nullptr if no alive insects
Actor* StudentWorld::PickRandomInsect (int x, int y){
    std::vector<Actor*> InsectVec;
    
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->AmIInsect() && !((*it)->AmIDead()) ) {
            InsectVec.push_back(*it);
        }
    }
    if(InsectVec.size()>0){
    int InsectNum = randInt(0, InsectVec.size()-1);
    return InsectVec[InsectNum];
    }
    return nullptr;
}


//returns the amount of food to give to an insect (amt if amt>= food object's health, otherwise food object's total health)  and adjusts Food objects health(kills Food if no health left afterward)
//0 if no food available
int StudentWorld::GiveInsectFood(int x, int y, int amt){
    std::list<Actor*>::const_iterator it;
    if(isThereFood(x,y)){
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->AmIFood()){
            break;
        }
    }
    
    int food = (*it)->get_health();
    if(food>amt){
        (*it)->set_health(food - amt);
        return amt;
    }
    else{
        (*it)->set_health(0);
        (*it)->died(true);
        return food;
    }
    }
    return 0;
}


void StudentWorld::StunAllStunableActors(int x, int y, Actor* water){
    list<Actor*>::iterator it;
    for(it = ActorGrid[x][y].begin();it != ActorGrid[x][y].end(); it++) {
        if((*it)->IsStunable() && ((*it)->stun_er() != water)){
            (*it)->stun();
            (*it)->StunnedMe(water);
        }
    }
}

void StudentWorld::PoisonAllPoisonableActors(int x, int y, Actor* poison){
    list<Actor*>::iterator it;
    for(it = ActorGrid[x][y].begin();it != ActorGrid[x][y].end(); it++) {
        if((*it)->IsPoisonable()&& ((*it)->poison_er()!= poison)){
            cerr<<"poisoning " << (*it)->get_health();
            (*it)->poison();
            (*it)->PoisonedMe(poison);
            cerr<<" to " << (*it)->get_health() << endl;
            
            if((*it)->get_health()<=0){
                (*it)->die();
            }
        }
    }
    
}



// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


