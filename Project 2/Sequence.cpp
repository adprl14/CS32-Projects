//
//  Sequence.cpp
//  CS32_Project2
//
//  Created by Andrew Perley on 1/28/17.
//  Copyright © 2017 Andrew Perley. All rights reserved.
//

#include "Sequence.h"

//constructor creates an empty Sequence with 0 size
Sequence::Sequence(){
    head = nullptr;
    tail = nullptr;
    seq_size = 0;
}

//Copy Constructor
Sequence::Sequence(const Sequence &oldseq){
    //initialize Sequence to empty
    head = nullptr;
    tail = nullptr;
    seq_size = 0;
   
   //if the old sequence is not empty then copy elements over to new sequence
    if( !oldseq.empty()){
        Node *current_old = oldseq.head;
    
        for (int i = 0; i<oldseq.size(); i++){
            insert(i, current_old->value);
            current_old = current_old->next;
        }
    }
    
}

//assignment operator;
Sequence& Sequence::operator=(const Sequence& otherseq);



Sequence::~Sequence(){
    Node *p;
    p=head;
    while (p!=nullptr){
        Node *n = p->next;
        delete p;
        p=n;
    }
}


bool Sequence::empty() const{
    if(head == nullptr){
        return true;
    }
    
    return false;
}


int Sequence::size() const{
    return seq_size;
}


bool Sequence::insert(int pos, const ItemType &value){
    if(pos>= 0 && pos <= seq_size){
        Node *temp = head;
        
        //adding to empty list
        if( head == nullptr){
            Node *n = new Node;
            n->value = value;
            n->next = nullptr;
            n->prev = nullptr;
            head = n;
            tail = n;
            seq_size++;
            return true;
        }
        
        //adding to front of list with 1+ nodes
        if(pos == 0 && head != nullptr){
            Node *n = new Node;
            n->value = value;
            n->next = head;
            n->prev = nullptr;
            head->prev = n;
            head = n;
            seq_size++;
            return true;
        }
        
        //adding to end of list
        if( pos == seq_size){
            Node *n = new Node;
            n->value = value;
            n->next = nullptr;
            n->prev = tail;
            tail->next = n;
            tail = n;
            seq_size++;
            return true;
        }
        
        //adding to middle of list
        
        for( int i = 0; i < pos; i++){
            if (temp != nullptr)
                temp = temp->next;
        }
        
        Node *n = new Node;
        n-> value = value;
        n->next = temp;
        n->prev = temp->prev;
        (temp->prev)->next = n;
        temp -> prev = n;
        seq_size++;
        return true;
    }
    
    return false;
}

int Sequence::insert(const ItemType& value){
    int p = -1;                     //int representing position where value is inserted, -1 == failure
    Node* temp = head;
    
    //inserting if list is empty
    if(head == nullptr) {
        p=0;
        insert(p, value);
        return p;
    }
    
   //insert to front of list
    if( value <= head->value) {
        p=0;
        insert(p, value);
        return p;
    }
    
    //insert to middle or end
    for (int i=0; i<seq_size; i++){
        if(value <= temp->value){
            p=i;
            insert(p, value);
            return p;
        }
        temp = temp->next;
        if(temp == nullptr){
            p = seq_size;
            insert(p, value);
            return p;
        }
    }
    return p;
}


bool Sequence::erase(int pos){
    if( pos>= 0 && pos < seq_size){
        if(empty())
            return false;              //immediately return false if Sequence is empty
        //erase if only one node in the list
        else if (head->next == nullptr && pos == 0){
            Node *temp;
            temp = head;
            delete temp;
            head = nullptr;
            tail = nullptr;
            seq_size--;
            return true;
        }
        //deleting the first node in a list with multiple nodes
        else if(pos==0){
            Node *temp;
            temp = head;
            head = temp->next;
            temp->next->prev = nullptr;
            delete temp;
            seq_size--;
            return true;
        }
        //deleting last node in a list with multiple nodes
        else if(pos == seq_size-1){
            Node *temp;
            temp = head;
           
            while(temp->next != nullptr)    //advance temp to last node
                temp = temp->next;
            
            temp->prev->next = nullptr;
            tail = temp -> prev;
            delete temp;
            
            seq_size--;
            return true;
        }
        //deleting a node from the middle of the list
        else{
            Node* temp;
            temp = head;
            
            for (int i=0; i<pos; i++){
                temp = temp->next;  //point temp at node to be deleted in the middle
            }
            temp->next->prev = temp->prev;  //set next node's prev to node before temp
            temp->prev->next = temp->next;  //set previous node's next to node after temp
            delete temp;
            seq_size--;
            return true;
        }
    }
    
    return false;
}


int Sequence::remove(const ItemType& value){
    int num_rem = 0;
    Node *temp = head;
    
    if(empty()){            //nothing to remove if Sequence is empty
        return num_rem;
    }
   
    int count = 0;          //counter for node position
    
    while(temp!= nullptr){
        if(temp->value == value){       //check if current node value == value
            temp = temp->next;          //move temp up one node to avoid temp pointing to a deleted node
            erase(count);               //erase the node == value (i.e. the node before what temp is now)
            num_rem++;                  //increment num_rem since you erased one
            continue;                   /*reiterate while loop without incrementing count since one node has
                                        been deleted*/
        }
        temp = temp->next;              /*if current node value != value then move to next node and              
                                         increment count*/
        count++;
    }
    
    
    return num_rem;
}


bool Sequence::get(int pos, ItemType& value) const{
    if( pos>=0 && pos < seq_size){
        Node *p = head;
       
        for (int i = 0; i<pos; i++){        //loop through list until you find node at position pos
            p=p->next;
        }
        value = p->value;                   //store value at p's node into value
        return true;
    }
    
    return false;
}

bool Sequence::set(int pos, const ItemType& value){
    if( pos>=0 && pos<seq_size){
        Node *temp = head;
        
        for(int i=0; i<pos; i++)
        {
            temp= temp->next;
        }
        
        temp->value = value;
        return true;
    }
    return false;
}

int Sequence::find(const ItemType& value) const{
    int p = -1;             //integer to represent smallest position where value was found, -1 means failure
    Node *temp = head;      //temporary pointer to current node in list
    
    if(empty())             //if Sequence is empty, immediately return failure to find value
        return p;
    
    for ( int i = 0; i< seq_size; i++){
        if (temp->value == value){
            p = i;                     //if value at temp == value then set p = position of temp
            return p;
        }
        temp=temp->next;                //move to next node
        if(temp ==nullptr)              //quit if you went past the end of the Sequence
            break;
    }
    
    //otherwise if no node contains value return p=-1 to signify failure
    return p;
}

void Sequence::swap(Sequence& other){
    Node *their_head = other.head;
    Node *their_tail = other.tail;
    
    other.head = head;
    other.tail = tail;
    
    head = their_head;
    tail = their_tail;
    
}




