//
//  test.cpp
//  CS32_Project2
//
//  Created by Andrew Perley on 1/30/17.
//  Copyright © 2017 Andrew Perley. All rights reserved.
//


#include <iostream>
#include "Sequence.h"
#include <cassert>

void test_empty();
void test_size();
void test_insert1();
void test_insert2();
void test_erase();
void test_remove();
void test_get();
void test_set();
void test_find();
void test_swap();
void test_destructor();
void test_copy();
void test_assign();
void test_interleave();
void test_subsequence();



int main() {
    test_empty();
    test_size();
    test_insert1();
    test_insert2();
    test_erase();
    test_remove();
    test_get();
    test_set();
    test_find();
    test_swap();
    test_destructor();
    test_copy();
    test_assign();
    test_interleave();
    test_subsequence();

    
    std::cerr<< "YAY!!!!!!"<<std::endl;
}

void test_empty(){
    Sequence empty;         //construct empty sequence
    assert(empty.empty());  //make sure empty sequence is empty
}

void test_size(){
    Sequence nothing;
    assert(nothing.size() == 0);    //make sure empty sequence has 0 size
}

void test_insert1(){
    Sequence seq1;
    assert(seq1.empty() && seq1.size()==0 );    //make sure I am working with an empty sequence
    
    assert(!seq1.insert(1, 0) && !seq1.insert(-1, 0));  //make sure I cannot insert into invalid positions
    assert(seq1.insert(0, 1) && !seq1.empty() && seq1.size()==1);//make sure successful insert returns true, and seq1 is not empty and size has been incremented by 1
    
    //test multiple insertions and insertion into end works
    for(int i = 1; i<100; i++){
        assert(seq1.size() == i);
        seq1.insert(i, 2);
    }
    
    assert(seq1.insert(0, 1)); //make sure insertion into beginning of non-zero sequence works
    
}

void test_insert2(){
    Sequence seq2;
    assert(seq2.empty() && seq2.size()==0 );
    
    assert(seq2.insert(2)==0 && seq2.size() ==1);   //make sure insertion successful, in correct position, size incremented correctly
    assert(seq2.insert(3)==1 && seq2.insert(0)==0); //makes sure smallest number is added before 1st element it is <=
    assert(seq2.find(2) ==1);   //implicitly makes sure Seqeunce contents in correct order
    

}

void test_erase(){
    Sequence to_erase;
    assert(!to_erase.erase(0) && !to_erase.erase(1) );  //makes sure can't erase a zero sequence
    
    for( int i = 0; i<100; i++){
        to_erase.insert(0);
    }
    
    //test multiple erasures and erasing from end of Sequence
    for (int j = 100; j>0; j--) {
        assert(to_erase.size() == j);
        to_erase.erase(j-1);
    }
    
    to_erase.insert(1);to_erase.insert(7);to_erase.insert(12);
    assert(to_erase.erase(1));  //makes sure can erase from middle
    assert(to_erase.find(7) == -1 && to_erase.find(12) == 1); //makes sure contents moved correctly
    assert(to_erase.erase(0));  //makes sure can erase from end
    
    
}

void test_remove(){
    Sequence to_remove;
    for( int i = 0; i<100; i++){
        to_remove.insert(0);
    }
    to_remove.insert(50, 100);
    assert(to_remove.size()==101);
    
    assert(to_remove.remove(0)==100 && to_remove.size()==1);    //tests removal of multiple nodes
    assert(to_remove.remove(10)==0 && to_remove.remove(100)==1);    //test removal of 0 nodes and 1 node
    assert(to_remove.size()==0);
}

void test_get(){
    Sequence treasure;
    ItemType loot;
    //store treasure :)
    assert(!treasure.get(1, loot));//tests to make sure cannot get from invalid position
    treasure.insert(9);treasure.insert(0); treasure.insert(12);treasure.insert(7);

    treasure.insert(1, 60);
    
    assert(treasure.get(1, loot) && loot == 60); //tests to make sure value "got" is correct and get returns true
    
    
    
}

void test_set(){
    Sequence stuff;
    assert(!stuff.set(5, 6));   //cannot set to invalid position
    
    stuff.insert(0);stuff.insert(3);stuff.insert(7);stuff.insert(14);
    assert(stuff.set(1, 8) && stuff.find(8) ==1 );  //tests that set works and value actually changed
}

void test_find(){
    Sequence lost_found;
    assert(lost_found.find(0)==-1); //tests that cannot find in zero Sequence
    lost_found.insert(17);lost_found.insert(17);lost_found.insert(1);lost_found.insert(0);
    assert(lost_found.find(12)==-1);   //cannot find number that does not exist in Sequence
    assert(lost_found.find(17)==2); //finds 17 and reports pos where first found

}

void test_swap(){
    Sequence swap_me;
    Sequence swap_him;
    Sequence swap_her;
    
    swap_me.insert(0, 6);swap_me.insert(0, 12);
    
    for(int i = 0; i<10; i++) {
        swap_him.insert(5);
    }
    
    swap_me.swap(swap_me);
    
    assert(swap_me.size()==2 && swap_me.find(12)==0); //tests aliasing
    
    swap_me.swap(swap_him);
    assert(swap_him.size()==2 && swap_him.find(12)==0); //makes sure swap worked (i.e. contents and size swapped)
    assert(swap_me.size()==10 && swap_me.find(5)==0 && swap_me.remove(5)==10);
    
    swap_him.swap(swap_her);
    assert(swap_him.size()==0 && swap_him.find(12)==-1);    //tests swapping with 0 Sequence
    assert(swap_her.size()==2 && swap_her.find(12)==0);


    
}

void test_destructor(){
    Sequence destruct_nothing;
    Sequence destruct_something;
    
    //tested with cerr debug statement in code
    for (int i = 0; i<10; i++){
        destruct_something.insert(0, 0);
    }
    
}

void test_copy(){
    Sequence A;
    Sequence B(A);
    
    assert(A.size() == 0 && B.size() == 0); //tests copy constructor for 0 Sequence
    
    for (int i = 0; i<100; i++) {
        A.insert(1);
    }
    
    Sequence C(A);
    //test copy constructor for non-zero sequence
    assert(A.size() == 100 && C.size() == 100 && A.find(1) == 0 && C.find(1) == 0 );
    
}

void test_assign(){
    Sequence a;
    Sequence b;
    Sequence z;
    z.insert(5);z.insert(3);z.insert(62);
    
    a.insert(0);a.insert(1);a.insert(3);a.insert(7);a.insert(0, 12);
    
    b=a;
    a.dump();   //compare contents in a cerr dump
    b.dump();
    
    assert((a.find(12)==b.find(12))&& (a.find(7)==b.find(7)));  //tests assignment to 0 Sequence
    assert(a.size()==b.size());
    
    a=a;
    a.dump();
    assert((a.find(12)==b.find(12))&& (a.find(7)==b.find(7)));  //tests self assignment
    assert(a.size()==5);
    
    z=a;
    assert(z.find(62)==-1 && z.size() == 5);
}

void test_interleave(){
    Sequence c;
    Sequence d;
    Sequence e;
    
    c.insert(12);c.insert(2);c.insert(7);c.insert(6);
    d.insert(0, 5);
    d.insert(1, -3);
    
    c.dump(); d.dump(); e.dump();
    
    interleave(c, e, c);    //test interleave with aliasing
    c.dump();
    assert(c.size() == 4 && c.find(2 ==0));
    assert(e.size()==0);
    
    interleave(e, e, e);    //test 0 Sequence aliasing
    e.dump();
    assert(e.size()==0);
    
    interleave(c, d, e);       //test normal function
    assert(c.size()==4 && d.size()==2 && e.size()==6);
    assert(e.find(2)==0 && e.find(5)==1 && e.find(-3)==3 && e.find(12) == 5);
    
    interleave(d, d, d);    //test aliasing
    assert(d.size() == 4 && d.find(-3)==2);
    
    d.dump();
    e.dump();
}

void test_subsequence(){
    Sequence p;
    Sequence q;
    Sequence r;
    Sequence s;
    
    assert(subsequence(p, q)==-1);  //cannot find subsequence in 0 Sequences
    p.insert(32);
    assert(subsequence(p, q)==-1 && subsequence(q, p)==-1); //cannot find a subsequence of a trivial Sequence(q) in a nontrivial one (p)
    
    p.insert(0, 8);p.insert(0, 29);p.insert(0, 17);p.insert(0, 63);p.insert(0, 17);p.insert(0, 42);p.insert(0, 63);p.insert(0, 21);p.insert(0, 30);
    q.insert(0, 29);q.insert(0, 17);q.insert(0, 63);
    p.dump();
    q.dump();
    
    assert(subsequence(p, q)==5 && subsequence(q, p)==-1);  //tests normal function of subsequence and that a larger sequence cannot be found as a subsequence of a smaller sequence
    
    r.insert(0, 32);r.insert(0, 8);r.insert(0, 29);
    r.dump();
    assert(subsequence(p, r)==7); //tests subsequence that goes to the last element of the main Sequence
    
    s.insert(0, 63);s.insert(0, 21);s.insert(0, 30);
    s.dump();
    
    assert(subsequence(p, s)==0); //tests subsequence starts from first element of the main Sequence
    
}