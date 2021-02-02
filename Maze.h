//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#ifndef _MAZE_H_
#define _MAZE_H_

#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <getopt.h>
#include "Tile.h"
#include <string>


//Programmer: Andrew Heldrich
//Date: 21 01 2021
//Purpose: 

struct Location
{
    uint32_t room;
    uint32_t row;
    uint32_t col;
    Location ()
    {
        room = 0;
        row = 0;
        col = 0;
    }
    Location (uint32_t inRoom, uint32_t inRow, uint32_t inCol)
    {
        room = inRoom;
        row = inRow;
        col = inCol;
    }
};

class Maze
{
    private:
        std::vector< std::vector< std::vector < Tile > > > map;
        std::deque < Location > searchContainer;
        std::stack < Location > solnPath;
        Location startLoc;
        uint32_t numTilesDiscovered;
        uint32_t roomSize;
        uint32_t numRooms;
        bool isStackMode;
        bool isMapInput;
        bool isMapOutput;


        //reads input file that is in map mode
        bool readMapInput(uint32_t room, uint32_t row);

        //reads input file that is in list mode
        bool readListInput();

        //adds tile to map vector if it is valid. returns false if it encounters
        //bad input
        bool addTileToMap(Location &inLoc, char input);

        //sets input mode and map/room size based on header in file
        void readHeader();

        //returns true if location is within bounds of map, optionally outputs
        //error message
        bool isValidLocation(const Location &inLoc, bool outputErr) const;
        
        //returns true if new location found in direction of current location
        //outLoc updated with new location
        bool getNextLocation(Location &inLoc,
                                char dir,
                                Location &outLoc,
                                bool previous);

        //adds location to search container if possible 
        //(ie discoverable/walkable)
        void addLocationToSearch(Location &inLoc, char dir);

        //outputs contents of map to list w/ soln outlined
        void outputList();

        //outputs contents of map to map w/ soln outlined
        void outputMap();


    public:
        //The primary ctor to be used when creating a new car. Will auto-assign
        //Since this insertion operator really ought to be a method, but 
        //can't be due to the way C++ manages operator overloading, we
        //make it an "honorary method" by declaring it as a friend.
        Maze()
        {
            isStackMode = false;
            isMapInput = false;
            isMapOutput = false;
            roomSize = 0;
            numRooms = 0;
            numTilesDiscovered = 0;
        }

        //parses command line options
        void getOptions(int argc, char** argv);
        
        //reads in data from input file based on input mode
        bool readData();

        //checks if the entire map is searched or we found the countess
        bool isSearchDone()
        {
            return (!solnPath.empty() || searchContainer.empty());
        }

        //adds all possible locations to search based on current location 
        void processSearchContainer();
        
        //starts at countess location and builds path to start
        void backTraceSoln();

        //outputs soln based on output mode
        void outputSoln();
};

#endif // _Maze_H_
