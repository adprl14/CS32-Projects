//
//  History.cpp
//  CS32Project1
//
//  Created by Andrew Perley on 1/9/17.
//  Copyright © 2017 Andrew Perley. All rights reserved.
//

#include <iostream>
#include "History.h"
#include "Arena.h"
using namespace std;


History::History(int nRows, int nCols)
{
    h_rows = nRows;
    h_cols = nCols;
    for (int i = 0; i < nRows; i++)
    {
        for (int j = 0; j < nCols; j++)
        {
            h_grid[i][j] = '.';
        }
    }
}

History::~History()
{
}

bool History::record(int r, int c)
{
    bool result = true;
    
    if ( r < 1 || r > h_rows || c < 1 || c > h_cols)
    {
        result = false;
        return result;
    }
    
    
    if( (r>=1 && r <= h_rows) && (c>=1 && c <= h_cols) )
    {
        if (h_grid[r-1][c-1] >= 'A' && h_grid[r-1][c-1] < 'Z' )
        {
            h_grid[r-1][c-1]++;
        }
        else if (h_grid[r-1][c-1] == '.')
        {
            h_grid[r-1][c-1] = 'A';
        }
            
    }
        
    return result;
}

void History::display() const
{
    int r,c;
    
    clearScreen();
    for (r = 1; r <= h_rows; r++)
    {
        for (c = 1; c <= h_cols; c++)
            cout << h_grid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
}

