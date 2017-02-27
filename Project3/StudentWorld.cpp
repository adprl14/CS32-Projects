#include "StudentWorld.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "Compiler.h"
#include "Actor.h"
#include "Field.h"
#include <iostream>
#include <iomanip>  // defines the manipulator setwf
#include <sstream>  // defines the type std::ostringstream
#include <string>
#include<list>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

StudentWorld::~StudentWorld(){
    cleanUp();
    for(int i = 0; i<4; i++){
        if(compilerForEntrant[i] != nullptr){
            delete compilerForEntrant[i];
        }

    }
}

 int StudentWorld::init()
{
    Field f;
    string fieldFile = getFieldFilename();
    string error;

    AntHill *ah0, *ah1, *ah2, *ah3;
    
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    std::string errorComp;
    cerr<<fileNames.size()<<endl;
//Compile all Bugs Files
    for (int i = 0; i < fileNames.size();i++)
    {
        compilerForEntrant[i] = new Compiler;
            if ( ! compilerForEntrant[i]->compile(fileNames[i], errorComp) )
            {
                setError(fileNames[i] + " " + error);
            }
    }
    
    //MANUALLY
//    compilerForEntrant[0] = new Compiler;
//    if ( ! compilerForEntrant[0]->compile(fileNames[0], errorComp) )
//    {
//        setError(fileNames[0] + " " + error);
//    }
//    compilerForEntrant[1] = new Compiler;
//    if ( ! compilerForEntrant[1]->compile(fileNames[1], errorComp) )
//    {
//        setError(fileNames[1] + " " + error);
//    }
//    compilerForEntrant[2] = new Compiler;
//    if ( ! compilerForEntrant[2]->compile(fileNames[2], errorComp) )
//    {
//        setError(fileNames[2] + " " + error);
//    }
//    compilerForEntrant[3] = new Compiler;
//    if ( ! compilerForEntrant[3]->compile(fileNames[3], errorComp) )
//    {
//        setError(fileNames[3] + " " + error);
//    }

    
//Load Field
    if (f.loadField(fieldFile, error) != Field::LoadResult::load_success) {
        setError(fieldFile + " " + error);
        cerr<< error;
        return GWSTATUS_LEVEL_ERROR; // something bad happened!
    }
//Allocate all members in field
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
                
                case Field::anthill0:
                {
                    ah0 = new AntHill(this ,0,i,j, compilerForEntrant[0]);
                    ActorGrid[i][j].push_back(ah0);
                    ColonyCoord[0].first=i;
                    ColonyCoord[0].second=j;
                }
                    break;
                    
                case Field::anthill1:
                {
                    ah1 = new AntHill(this ,1,i,j, compilerForEntrant[1]);
                    ActorGrid[i][j].push_back(ah1);
                    ColonyCoord[1].first=i;
                    ColonyCoord[1].second=j;
                }
                    break;
                
                case Field::anthill2:
                {
                    ah2 = new AntHill(this ,2,i,j, compilerForEntrant[2]);
                    ActorGrid[i][j].push_back(ah2);
                    ColonyCoord[2].first=i;
                    ColonyCoord[2].second=j;
                }
                    break;
                
                case Field::anthill3:
                {
                    ah3 = new AntHill(this ,3,i,j, compilerForEntrant[3]);
                    ActorGrid[i][j].push_back(ah3);
                    ColonyCoord[3].first=i;
                    ColonyCoord[3].second=j;
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
  
    if(Ticks==MAX_TICKS){
        if(isThereAWinningAnt()){
            setWinner(GetWinningAntName());
            return GWSTATUS_PLAYER_WON;
        }
        return GWSTATUS_NO_WINNER;
    }
    
    setGameStatText(getDisplayText());
    
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


bool StudentWorld::isThereType(const int &x, const int &y, type Type) const{
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->get_type() == Type && !((*it)->AmIDead()) ){
            return true;
        }
    }
    return false;
}

bool StudentWorld::isDangerHere(const int &x, const int &y, const Ant* ptrToAnt) const{
        
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if(((*it)->AmIInsect() && ((*it)->getColony() != ptrToAnt->getColony()) ) || (*it)->AmIPuddle()  )
            return true;
    }
    return false;
}


bool StudentWorld::isPherInFront(const Actor* ptrToActor) const{
    Actor::Direction Facing = ptrToActor->getDirection();
    int checkX=ptrToActor->getX();
    int checkY=ptrToActor->getY();
    switch(Facing){
        case GraphObject::up:
            checkY++;
            break;
        case GraphObject::right:
            checkX++;
            break;
        case GraphObject::left:
            checkX--;
            break;
        case GraphObject::down:
            checkY--;
            break;
        default:
            return false;
    }
    
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[checkX][checkY].begin(); it != ActorGrid[checkX][checkY].end();it++){
        if((*it)->AmIPher() )
            return true;
    }
    return false;
}

Actor* StudentWorld::GetFirstType(const int &x, const int &y, type Type){
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->get_type() == Type)
            return (*it);
    }
    return nullptr;
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
void StudentWorld::AllocateActor(int x, int y, type Type){
    if(Type == Adult){
        Actor *p = new AdultGrasshopper(this, x, y);
        ActorGrid[x][y].push_back(p);
    }
    if(Type == Pher0){
        Actor *p = new Pheromone(this,IID_PHEROMONE_TYPE0, x, y,Pher0);
        ActorGrid[x][y].push_back(p);
    }
    if(Type == Pher1){
        Actor *p = new Pheromone(this,IID_PHEROMONE_TYPE1, x, y,Pher1);
        ActorGrid[x][y].push_back(p);
    }
    if(Type == Pher2){
        Actor *p = new Pheromone(this,IID_PHEROMONE_TYPE2, x, y,Pher2);
        ActorGrid[x][y].push_back(p);
    }
    if(Type == Pher3){
        Actor *p = new Pheromone(this,IID_PHEROMONE_TYPE3, x, y,Pher3);
        ActorGrid[x][y].push_back(p);
    }
}



//PICKS RANDOM ALIVE INSECT not equal to self, RETURN nullptr if no alive insects
Actor* StudentWorld::PickRandomInsect (int x, int y, Actor* self){
    std::vector<Actor*> InsectVec;
    
    std::list<Actor*>::const_iterator it;
    for( it = ActorGrid[x][y].begin(); it != ActorGrid[x][y].end();it++){
        if((*it)->AmIInsect() && !((*it)->AmIDead())  && (*it)!= self) {
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
            cerr<< water << " stunned "<< (*it)<<endl;
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


///////////////////////////////////////////////////////////////////////////////////
////////////////////////         TRACKING ANTS            /////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void StudentWorld::MakeType(const int&x, const int &y, type Type){
    Actor *p = nullptr;
    switch(Type){
        case Pher0:
            p = new Pheromone(this, IID_PHEROMONE_TYPE0, x,y,Pher0);
            ActorGrid[x][y].push_back(p);
            break;
        case Pher1:
            p = new Pheromone(this, IID_PHEROMONE_TYPE1, x,y,Pher1);
            ActorGrid[x][y].push_back(p);
            break;

        case Pher2:
            p = new Pheromone(this, IID_PHEROMONE_TYPE2, x,y,Pher2);
            ActorGrid[x][y].push_back(p);
            break;

        case Pher3:
            p = new Pheromone(this, IID_PHEROMONE_TYPE3, x,y,Pher3);
            ActorGrid[x][y].push_back(p);
            break;
        default:
            break;
    }
}

void StudentWorld::addPherTo(const int&x,const int&y,type PherType){
    list<Actor*>::iterator it;
    for( it = ActorGrid[x][y].begin();it != ActorGrid[x][y].begin();it++){
        if((*it)->get_type()==PherType)
            break;
    }
    if((*it)->get_health()<768)
    {
        if((768-(*it)->get_health()) <256)
            (*it)->set_health(768);
        else
             (*it)->set_health((*it)->get_health() + 256);
    }
}



void StudentWorld::MakeAnt(int x, int y, int Num, Compiler *comp){
    int ID;
    switch(Num){
        case 0:
            ID = IID_ANT_TYPE0;
            break;
        case 1:
            ID = IID_ANT_TYPE1;
            break;
        case 2:
            ID = IID_ANT_TYPE2;
            break;
        case 3:
            ID = IID_ANT_TYPE3;
            break;
        default:
            return;
    }
    AddAnt(Num);
    Actor *ant = new Ant(this, ID,Num, x, y, comp );
    ActorGrid[x][y].push_back(ant);
}

void StudentWorld::AddAnt(int Num){
    antsProducedBy[Num].first++;
    antsProducedBy[Num].second = MAX_TICKS-Ticks;
}


bool StudentWorld::isThereAWinningAnt(){
    for (int i = 0; i<4; i++){
        if (antsProducedBy[i].first>=6) {
            return true;
        }
    }
    return false;
}

int StudentWorld::GetWinningAntNumber(){
    std::pair<int, int> temp[4];
    for(int i = 0; i<4; i++){
        temp[i] = antsProducedBy[i];
    }
    
    sort(begin(temp), end(temp));
    
    for(int j = 0; j<4; j++){
        if(temp[3] == antsProducedBy[j])
            return j;
    }
    return randInt(0,3);
}

Actor* StudentWorld::GetWinningAnt(){
    int AntNum = GetWinningAntNumber();
    int colonyX = ColonyCoord[AntNum].first;
    int colonyY = ColonyCoord[AntNum].second;
    std::list<Actor*> ColonySpot = ActorGrid[colonyX][colonyY];
    if( ColonySpot.empty())
        return nullptr;
    std::list<Actor*>::iterator it;
    for( it = ColonySpot.begin();it != ColonySpot.end();it++){
        if((*it)->AmIAnthill())
            return *it;
    }
    return nullptr;
}


std::string StudentWorld::GetWinningAntName(){
    AntHill* curWinner = dynamic_cast<AntHill*>(GetWinningAnt());
    if(curWinner!=nullptr){
        return curWinner->getCompiler()->getColonyName();
    }
    return "";
}

std::string StudentWorld::getDisplayText(){
    ostringstream oss;
    oss<< "Ticks:";
    oss << setw(5) << 2000-Ticks;
    for(int i = 0; i<4; i++){
        
    }
    return oss.str();
}




// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


