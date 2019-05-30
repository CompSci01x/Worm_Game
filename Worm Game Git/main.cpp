
//***************************
//
// Class: CS-315
//
// Lab 6
//
// Date: November 21, 2018
//***************************

#include<curses.h>
#include <iostream>
#include <cstdlib>

#include "getChar.hpp"
#include "Screen.hpp"
#include "prototypes.hpp"


void startup( void );
void terminate( void );
void pauseGame(char , int , int);
void hitMunchieFunct(int & , int , Screen &);



int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        std::cout << "usage: " << argv[0] << " numOfRows ( 10 - 25 (inclusive) )" << " numOfCols ( 10 - 80 (inclusive) )" << std::endl;
        exit(1);
    }
    
    
    if ( std::stoi(argv[1]) < 10 ||  std::stoi(argv[1]) > 25)
    {
        std::cout << "The number of rows must be between 10 and 25 inclusive.\n";
        exit(1);
    }
    
    if (std::stoi(argv[2]) < 10 ||  std::stoi(argv[2]) > 80)
    {
        std:: cout << "The number of cols must be between 10 and 80 inclusive.\n";
        exit(1);
    }
    

    int totalRow = std::stoi(argv[1]);  // stores totalNum of Rows
    int totalCol = std::stoi(argv[2]);  // stores totalNum of Cols
    int playableCells = ((totalRow - 3) * (totalCol - 2));   // stores the totalNum of playable cells
    
    
    bool hitMunchie = false;
    int munchieCounter = 0;
    int score = 0;

    char ch;    // holds user input 'h,j,k, or l'

    startup();

    // create Screen object, create and initialize the board
    Screen scr;
    scr.createBoard(totalRow , totalCol);
    

    // headRow and headCol keep track of cur location of the worm head
    int headRow = (totalRow - 3) / 2;
    int headCol = (totalCol - 2)/ 2;
    mvaddch (headRow , headCol , '@');
   
    // oldRow and oldCol keep track of the prev position of head
    int oldRow;
    int oldCol;
    
    // generate a munchie
    scr.genMunchie(playableCells , headRow , headCol);

    
    refresh();
    get_char();

    
    do
    {
        oldRow = headRow;
        oldCol = headCol;
        
        ch = get_char();
        
        // pause game
        if (ch == 'z')
            pauseGame(ch , totalRow , totalCol);
        
        // exit game
        if (ch == 'x')
        {
            terminate();
            std::cout << "Game Exited\n";
            break;
        }
        
        // if there is only a worm head on the screen
        if (scr.isWormEmpty())
            mvaddch(oldRow , oldCol , ' ');
        

        // move head
        if (ch == 'l')
            headCol += 1;
        else if (ch == 'h')
            headCol -= 1;
        else if (ch == 'k')
            headRow -= 1;
        else if (ch == 'j')
            headRow += 1;

        mvaddch(headRow , headCol , '@');
           refresh();
        
        // if worm hit a wall
        if (headRow == std::stoi(argv[1]) - 1 || headCol == std::stoi(argv[2]) - 1 || headRow == 1 || headCol == 0)
        {
            terminate();
            std::cout << "Hit a wall!\n";
            break;
        }
        
        // if worm hit itself
        if (scr.isTaken(headRow, headCol) == -1)
        {
            terminate();
            std:: cout << "Worm hit itself!\n";
            break;
        }
        
        // if worm hit a munchie
        if (scr.getMunchieRow() == headRow && scr.getMunchieCol() == headCol)
        {
            hitMunchieFunct(score, totalCol, scr);
            
            // generate another munchie and set hitMunchie = true
            scr.genMunchie(playableCells , headRow , headCol);
            hitMunchie = true;
        }
        
        
        // keeps track of how many 'o' to add to the worm, so once # of 'o' = score
        // switch hitMunchie to false, thus no more 'o' will be added until another munchie has been hit again
        if (munchieCounter == score)
            hitMunchie = false;
        else
            munchieCounter++;
        
        // if a munchie has been hit add to worm else move the worm (if the worm has 'o's)
        if (hitMunchie)
            scr.addToWorm(oldRow , oldCol);
        else if (!scr.isWormEmpty())
            scr.moveWorm(oldRow, oldCol);
       
    }while (ch != 'x');


    std::cout << "Score: " << score << "\n";
    terminate();

    return 0;
}

/////////////////////////////////////////////////////////////////////////////////

void startup( void )
{
    initscr();        /* activate curses                                            */
    curs_set(0);   /* prevent the cursor to be displayed                         */
    clear();        /* clear the screen that curses provides                      */
    noecho();        /* prevent the input chars to be echoed to the screen         */
    cbreak();        /* change the stty so that characters are delivered to the
                      program as they are typed--no need to hit the return key!  */
}

void terminate( void )
{
    mvcur( 0, COLS - 1, LINES - 1, 0 );
    clear();
    refresh();
    endwin();
}


void pauseGame(char ch , int totalRow , int totalCol)
{
    mvaddstr(totalRow + 2, totalCol/2 - 5, "Game Paused");
    refresh();
    do
    {
        ch = get_char();
    }while(ch == 'z');
    
    mvaddstr(totalRow + 2, totalCol/2 - 5, "Game Resumed");
    refresh();
    get_char();
    mvaddstr(totalRow + 2, totalCol/2 - 5, "             ");
    refresh();
}

void hitMunchieFunct(int &score , int totalCol , Screen &scr)
{
    // update score
    score += scr.getMunchieValue();
    
    // updating the score (at top of screen)
    std:: string strScore = std::to_string(score);
    
    for (int z = 0 ; z < strScore.size() ; z++)
        mvaddch(0, (totalCol+1) + z , strScore.at(z));
}
