//
//  Sequence.cpp
//  CS32_Project2
//
//  Created by Andrew Perley on 1/28/17.
//  Copyright © 2017 Andrew Perley. All rights reserved.
//

#include "Sequence.h"
#include <iostream>

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


Sequence& Sequence::operator=(const Sequence& otherseq){
    Sequence temp(otherseq);            // create a temporary copy of the other sequence
    swap(temp);                         //swap current sequence with temp copy
                                        //self-assignment should be no problem
    return *this;
}



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
    int m_size = seq_size;
    
    other.head = head;
    other.tail = tail;
    
    head = their_head;
    tail = their_tail;
    
    seq_size = other.seq_size;
    other.seq_size = m_size;
    
}

//dump prints out all node values in Linked List (i.e. the "Sequence" class)
void Sequence::dump() const{
    Node *temp;
    temp = head;
    while (temp!=nullptr){
        std::cerr << temp->value << " ";
        temp = temp->next;
    }
    std::cerr << std::endl;
}


int subsequence(const Sequence& seq1, const Sequence& seq2){
    int k = -1;         //int to store position where seq2 is found as a subsequence of seq1 in seq1
    ItemType x;
    ItemType y;
    
    //no need to check for subsequence if seq2 is larger than seq 1, or is seq2 has no elements
    if(seq2.size() > seq1.size() || seq2.size()==0){
        return k;
    }
    
    seq2.get(0, x);              //store value of first element in seq2 in x
    
    //check as long as seq2 can fit in seq1 at position i in seq1
    for(int i = 0; i < seq1.size()-(seq2.size()-1) ; i++){
        seq1.get(i, y);         //store value of first element in seq1 in y
        if( y == x){            //if you find an element in seq1 with value == x
            //check every subsequent element in seq1 to see if they match subsequent elements in seq2
            for (int j = 1; j<seq2.size(); j++) {
                ItemType temp1;
                ItemType temp2;
                seq1.get(i+j, temp1);
                seq2.get(j, temp2);
                
                //if two elements are not equal retry
                if(temp1!=temp2)
                    break;
               
                //if all elements of seq2 are found sequentially in seq1 return the position in seq1 where the subsequence starts
                if(j==seq2.size()-1){
                    k=i;
                    return k;
                }
            }
        }
        
       
    }
    
    return k;           //otherwise if seq2 is not found as a subsequence of seq1 return -1
}


void interleave(const Sequence& seq1, const Sequence& seq2, Sequence& result){
    Sequence temp;                          //create a temp sequence to avoid aliasing (e.g result = seq_1)
    
    int curr1,curr2,curr_res;              //counters for current place in each repective sequence
    curr1=curr2=curr_res=0;                 //i.e. seq1,seq2, result
    
    //loop to weave sequences together. keep going as long as one of the sequences still has nodes left
    while (curr1 < seq1.size() || curr2 < seq2.size()){
        
        //insert node value at curr_res from seq1 so long as there are nodes left (i.e. a node exists as position curr1 in seq1)
        if (curr1 < seq1.size()) {
            ItemType x;
            seq1.get(curr1, x);
            temp.insert (curr_res,x);    //add value of x to temp at curr_res
            curr1++; curr_res++;        //increment counter for place in seq1 and temp
        }
        
        //analogous to above
        if (curr2 < seq2.size()) {
            ItemType y;
            seq2.get(curr2, y);
            temp.insert (curr_res,y);
            curr2++; curr_res++;
        }
    }
    
    //once temp has been created by weaving together seq1 and seq2, swap result and temp
    result.swap(temp);
}


