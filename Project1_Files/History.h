//
//  History.h
//  CS32Project1
//
//  Created by Andrew Perley on 1/9/17.
//  Copyright © 2017 Andrew Perley. All rights reserved.
//

#ifndef History_h
#define History_h
#include "globals.h"

class Arena;

class History
{
public:
    History(int nRows, int nCols);
    ~History();
    bool record(int r, int c);
    void display() const;

private:
    int h_rows;
    int h_cols;
    char h_grid[MAXROWS][MAXCOLS];
    
};

#endif /* History_h */
