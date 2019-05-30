
#include "Screen.hpp"
#include "prototypes.hpp"
#include <queue>
#include <string>
#include <ctime>

//**************************************************************
//
// Creates and initializes 2d board array
// Creates and initializes the freePool array
// Creates the worm array
//
//**************************************************************
void Screen::createBoard(int row , int col )
{
    _numOfRows = row;
    _numOfCols = col;
    
    // create board
    board = new int *[row];
    for (int x = 0; x < row ; x++)
        board[x] = new int [col];
    
    
    // display Name of Game
    mvaddstr(0, 0, "Worm");
    
    // display Score
    mvaddstr(0, (_numOfCols - 5), "Score: ");
    mvaddch(0, (_numOfCols + 1), '0');

    
    // creates TOP boarder
    for (int x = 0 ; x < col ; x ++)
    {
        move(1,x);
        addch('*');
        board[1][x] = -1;
    }
    
    // creates Bottom boarder
    for (int x = 0 ; x < col ; x ++)
    {
        move(row - 1 ,x);
        addch('*');
        board[row-1][x] = -1;
    }
    
    // creates LEFT boarder
    for (int x = 1 ; x < row ; x++)
    {
        move(x,0);
        addch('*');
        board[x][0] = -1;
    }
    
    // creates RIGHT boarder
    for (int x = 1 ; x < row ; x++)
    {
        move(x,col-1);
        addch('*');
        board[x][col-1] = -1;
    
    }
    
    // create freePool and worm arrays
    int playableCells = (row - 3) * (col - 2);
    freePool = new Cell[playableCells];
    worm = new Cell[playableCells];
    
    
    // Initializes the playable rows and cols
    for (int x = 2; x < row - 1 ; x++)
    {
        for (int y = 1; y < col - 1 ; y++)
        {
            board[x][y] = lastIdx;
            
            freePool[lastIdx].setRow(x);
            freePool[lastIdx].setCol(y);
            freePool[lastIdx].setFreePoolIdx(lastIdx);
            
            lastIdx++;
        }
    }
    
    lastIdx--;
}


//**************************************************************
//
// Generate a munchie by randomly picking a # 1 - 9 and then
// randomly picking a # between 0 and numOfPlayableFields
// which will be a index of the freePool. Go to that freePool idx
// and place the munchie at the row and col stored there.
//
//**************************************************************
void Screen::genMunchie(int numOfPlayableFields , int headRow , int headCol)
{
     munchieValue = rand() % 9 + 1;
    
    std::string str = std::to_string(munchieValue);
    char c = str.at(0);
    
    srand(unsigned (time(0)));
    
    int freeIdx = rand() % numOfPlayableFields;
    
    munchieRow = freePool[freeIdx].getRow();
    munchieCol = freePool[freeIdx].getCol();
    
    // make sure that the munchie is not placed on the worm head itself
    if (headRow == munchieRow && headCol == munchieCol)
    {
        do
        {
            int freeIdx = rand() % numOfPlayableFields;
            munchieRow = freePool[freeIdx].getRow();
            munchieCol = freePool[freeIdx].getCol();
            
        }while (headRow == munchieRow && headCol == munchieCol);
        
    }
    
    mvaddch(munchieRow , munchieCol , c);
    refresh();
    
}

//**************************************************************
//
// Add to the worm array (circular array)
//
//**************************************************************
void Screen::addToWorm(int row , int col)
{
    worm[head].setRow(row);
    worm[head].setCol(col);
    
    screenCellTaken(row, col);

    int playableCells = (_numOfRows - 3) * (_numOfCols - 2);

    head = (head + 1) % playableCells;
    
    mvaddch(row, col, 'o');
    refresh();

    _isEmpty = false;
}

//**************************************************************
//
// Move the worm by removing the tail and adding a new 'o' at the
// location of the head, thus worm moves in constant time
//
//**************************************************************
void Screen::moveWorm(int row , int col)
{
    int tailRow = worm[tail].getRow();
    int tailCol = worm[tail].getCol();
    
    //"remove" the tail
    screenCellFree(tailRow, tailCol);
    mvaddch(tailRow, tailCol, ' ');
    refresh();
    
    //then essentially "tail++"
    int playableCells = (_numOfRows - 3) * (_numOfCols - 2);
    tail = (tail + 1) % playableCells;
    
    // add new 'o' to worm array
    addToWorm(row, col);
    
}


//**************************************************************
//
// Indicate that a screen cell has been taken on the 2d board array
//
//**************************************************************
void Screen::screenCellTaken(int row , int col)
{
    
    // get the index stored at this spot in the board
    int poolIdx = board[row][col];
    
    // save the freePool idx
    worm[head].setFreePoolIdx(poolIdx);
    
    // set this spot in the board as taken
    board[row][col] = -1;
    
    
    // get the row and col stored in the last index of the freePool
    int freePoolRow = freePool[lastIdx].getRow();
    int freePoolCol = freePool[lastIdx].getCol();
    
    
    // set the row and col of the now taken index in the freePool to the free Row and Col
    freePool[poolIdx].setRow(freePoolRow);
    freePool[poolIdx].setCol(freePoolCol);
    
}


//**************************************************************
//
// Indicate that a screen cell has been freed on the 2d board array
//
//**************************************************************
void Screen::screenCellFree(int row, int col)
{
    // get the freePoolIdx that will be stored in the 2d array
    int idx = worm[tail].getFreePoolIdx();
    
    // set the board at this spot to hold that idx
    board[row][col] = idx;
    
    // indicate that this idx in the freePool is now free
    freePool[idx].setRow(row);
    freePool[idx].setCol(col);
}



Screen::~Screen()
{
    // delete board
    for (int x = 0; x < _numOfRows ; x++)
        delete [] board[x];
        delete [] board;
    
    // delete freePool
    delete [] freePool;
    
    // delete worm
    delete [] worm;
}
