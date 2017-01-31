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
    Sequence empty;
    assert(empty.empty());
}

void test_size(){
    Sequence nothing;
    assert(nothing.size() == 0);
}

void test_insert1(){
    Sequence seq1;
    assert(seq1.empty() && seq1.size()==0 );
    
    assert(!seq1.insert(1, 0) && !seq1.insert(-1, 0));
    assert(seq1.insert(0, 1) && !seq1.empty() && seq1.size()==1);
    
    for(int i = 1; i<100; i++){
        assert(seq1.size() == i);
        seq1.insert(i, 2);
    }
    
}

void test_insert2(){
    Sequence seq2;
    assert(seq2.empty() && seq2.size()==0 );
    
    assert(seq2.insert(2)==0 && seq2.size() ==1);
    assert(seq2.insert(3)==1 && seq2.insert(0)==0);
    assert(seq2.find(2) ==1);
    

}

void test_erase(){
    Sequence to_erase;
    assert(!to_erase.erase(0) && !to_erase.erase(1) );
    
    for( int i = 0; i<100; i++){
        to_erase.insert(0);
    }
    
    for (int j = 100; j>0; j--) {
        assert(to_erase.size() == j);
        to_erase.erase(j-1);
    }
    
    to_erase.insert(1);to_erase.insert(7);to_erase.insert(12);
    to_erase.erase(1);
    assert(to_erase.size() == 2 && to_erase.find(12) == 1);
    
    
}

void test_remove(){
    Sequence to_remove;
    for( int i = 0; i<100; i++){
        to_remove.insert(0);
    }
    to_remove.insert(50, 100);
    assert(to_remove.size()==101);
    
    assert(to_remove.remove(0)==100 && to_remove.size()==1);
    assert(to_remove.remove(10)==0 && to_remove.remove(100)==1);
    assert(to_remove.size()==0);
}

void test_get(){
    Sequence treasure;
    ItemType loot;
    //store treasure :)
    assert(!treasure.get(1, loot));
    treasure.insert(9);treasure.insert(0); treasure.insert(12);treasure.insert(7);

    treasure.insert(1, 60);
    
    assert(treasure.get(1, loot) && loot == 60);
    
    
    
}

void test_set(){
    Sequence stuff;
    assert(!stuff.set(5, 6));
    
    stuff.insert(0);stuff.insert(3);stuff.insert(7);stuff.insert(14);
    assert(stuff.set(1, 8) && stuff.find(8) ==1 );
}

void test_find(){
    Sequence lost_found;
    assert(lost_found.find(0)==-1);
    lost_found.insert(17);lost_found.insert(17);lost_found.insert(1);lost_found.insert(0);
    assert(lost_found.find(12)==-1);
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
    
    assert(swap_me.size()==2 && swap_me.find(12)==0);
    
    swap_me.swap(swap_him);
    assert(swap_him.size()==2 && swap_him.find(12)==0);
    assert(swap_me.size()==10 && swap_me.find(5)==0 && swap_me.remove(5)==10);
    
    swap_him.swap(swap_her);
    assert(swap_him.size()==0 && swap_him.find(12)==-1);
    assert(swap_her.size()==2 && swap_her.find(12)==0);


    
}

void test_destructor(){
    Sequence destruct_nothing;
    Sequence destruct_something;
    
    for (int i = 0; i<10; i++){
        destruct_something.insert(0, 0);
    }
    
}

void test_copy(){
    Sequence A;
    Sequence B(A);
    
    assert(A.size() == 0 && B.size() == 0);
    
    for (int i = 0; i<100; i++) {
        A.insert(1);
    }
    
    Sequence C(A);
    
    assert(A.size() == 100 && C.size() == 100 && A.find(1) == 0 && C.find(1) == 0 );
    
}

void test_assign(){
    Sequence a;
    Sequence b;
    
    a.insert(0);a.insert(1);a.insert(3);a.insert(7);a.insert(0, 12);
    
    b=a;
    
    a.dump();
    b.dump();
    
    assert((a.find(12)==b.find(12))&& (a.find(7)==b.find(7)));
    
    a=a;
    a.dump();
    assert((a.find(12)==b.find(12))&& (a.find(7)==b.find(7)));
    
}

void test_interleave(){
    Sequence c;
    Sequence d;
    Sequence e;
    
    c.insert(12);c.insert(2);c.insert(7);c.insert(6);
    d.insert(0, 5);
    d.insert(1, -3);
    
    c.dump(); d.dump(); e.dump();
    
    interleave(c, e, c);
    c.dump();
    
    interleave(e, e, e);
    e.dump();
    
    interleave(c, d, e);       //test normal function
    interleave(d, d, d);    //test aliasing
    
    d.dump();
    e.dump();
}

void test_subsequence(){
    Sequence p;
    Sequence q;
    Sequence r;
    Sequence s;
    
    assert(subsequence(p, q)==-1);
    p.insert(32);
    assert(subsequence(p, q)==-1 && subsequence(q, p)==-1);
    
    p.insert(0, 8);p.insert(0, 29);p.insert(0, 17);p.insert(0, 63);p.insert(0, 17);p.insert(0, 42);p.insert(0, 63);p.insert(0, 21);p.insert(0, 30);
    q.insert(0, 29);q.insert(0, 17);q.insert(0, 63);
    p.dump();
    q.dump();
    
    assert(subsequence(p, q)==5 && subsequence(q, p)==-1);
    
    r.insert(0, 32);r.insert(0, 8);r.insert(0, 29);
    r.dump();
    assert(subsequence(p, r)==7);
    
    s.insert(0, 63);s.insert(0, 21);s.insert(0, 30);
    s.dump();
    
    assert(subsequence(p, s)==0);
    
}