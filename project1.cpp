//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#include "Maze.h"
#include "Tile.h"
using namespace std;


//Author: Andrew Heldrich
//Date: 21 01 2021
//Purpose: implements Marco maze game
//Revision History:

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);
    Maze mazeGame;

    
    mazeGame.getOptions(argc, argv);
    if (!mazeGame.readData())
    {
        exit(1); //we encountered bad input
    }

    while (!mazeGame.isSearchDone())
    {
        mazeGame.processSearchContainer();
    }

    mazeGame.backTraceSoln();
    mazeGame.outputSoln();

    return 0;
}



//TODO: Enter Function Defs  