
#ifndef Screen_hpp
#define Screen_hpp

#include <stdio.h>
#include<curses.h>
#include <iostream>
#include <cstdlib>

#include "getChar.hpp"
#include "Cell.hpp"
#include "prototypes.hpp"


class Screen
{
public:
    Screen() {}
    ~Screen();
    
    void createBoard(int , int);

    void genMunchie(int , int , int);
    int getMunchieRow() {return munchieRow;}
    int getMunchieCol() {return munchieCol;}
    int getMunchieValue() {return munchieValue;}
    
    void addToWorm(int , int);
    void moveWorm(int , int);

    bool isWormEmpty() {return _isEmpty;}
    
    void screenCellTaken(int , int);
    void screenCellFree(int , int);
    
    int isTaken(int r , int c) {return board[r][c];}
        
private:
    
    int **board = nullptr;
    Cell *freePool = nullptr;
    Cell *worm = nullptr;
    
    bool _isEmpty = true;
    
    int munchieRow;
    int munchieCol;
    int munchieValue = 0;
    
    int head = 0;
    int tail = 0;
    
    int lastIdx = 0;
    
    int _numOfRows = 0;
    int _numOfCols = 0;
    
};

#endif /* Screen_hpp */
