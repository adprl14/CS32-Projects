//
//  main.cpp
//  CS32_Project2
//
//  Created by Andrew Perley on 1/28/17.
//  Copyright © 2017 Andrew Perley. All rights reserved.
//

#include <iostream>
#include "Sequence.h"
#include <cassert>

void test1();
void test2();

int main() {
    test1();
    test2();
    
    
    std::cerr<< "YAY!!!!!!"<<std::endl;
}

void test1(){
    Sequence s;
    ItemType x;
    
    assert(s.empty()&& s.size() == 0);
    assert(!s.insert(1, 6));
    assert(!s.insert(-1, 5));
    assert(!s.get(0, x));
    
    assert(s.insert(0, 5) && s.size()==1);
    assert(s.insert(0, 1) && s.get(0,x) && x==1 && s.find(1)==0 );
    assert(s.find(5)==1);
    s.insert(7);
    assert(s.size()==3 && s.find(7) == 2);
    s.insert(-1);
    s.insert(12);
    assert(s.size()==5);
    assert(s.find(-1) == 0 && s.find(12) == 4);
    
    assert(!s.erase(5) && s.erase(0));
    assert(s.size() ==4 && s.find(1)==0);
    assert(s.erase(3) && s.find(1) == 0 && s.size() ==3);
    
    Sequence s2;
    s2.insert(2);
    assert(s2.erase(0) && s2.size() ==0);
    s2.insert(1); s2.insert(-1);
    assert(s2.size() ==2 && s2.find(-1) == 0);
    s2.erase(1);
    assert(s2.size() == 1 && s2.find(-1) ==0);
    assert(s2.insert(1, -3) && s2.find(-3)==1);
    
    Sequence s3;
    s3.insert(1);s3.insert(1);s3.insert(1);s3.insert(1);s3.insert(0);
    s3.set(2, 5);
    assert(s3.size() ==5 && s3.find(5) == 2);
    assert(s3.remove(-5)==0);
    assert(s3.remove(1)==3 && s3.size()==2 && s3.find(5)==1);
    assert(!s3.set(2, 10) && !s3.set(-1, 12) && s3.set(0,12) && s3.find(12) ==0);
}

void test2(){
    Sequence s;
    s.insert(0);s.insert(5);s.insert(7);
    s.insert(0, 12);
    
    Sequence b(s);
    
    assert( b.size()==4 && b.find(12) ==0);
    
    
    
    
    
    
}
