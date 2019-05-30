
#ifndef Cell_hpp
#define Cell_hpp

#include <stdio.h>


class Cell
{
public:
    
    void setRow(int r) {row = r;}
    void setCol(int c) {col = c;}
    void setFreePoolIdx(int idx) {freePoolIdx = idx;}
    
    int getRow() {return row;}
    int getCol() {return col;}
    int getFreePoolIdx() {return freePoolIdx;}
    
private:
    int row;
    int col;
    int freePoolIdx;
    
};



#endif /* Cell_hpp */
