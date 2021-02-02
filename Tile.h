//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79

#ifndef _TILE_H_
#define _TILE_H_

#include <iostream>
#include <string>
#include <vector>
#include <deque>

//Programmer: Andrew Heldrich
//Date: 21 01 2021
//Purpose: represents tile on map vector in maze game

class Tile
{
    private:
        char value;
        char path;
        bool canWalk;

    public:
        Tile()
        {
            value = '.';  //default to walkable space
            path = 'x';
            canWalk = true;
        }

        Tile(char inValue)
        {
            value = inValue;
            path = 'x';

            if (isWarpPipe() || isCountess() || isStart() || inValue == '.')
            {
                canWalk = true;
            }
            else
            {
                canWalk = false;
            }
        }

        char getValue() const
        {
            return value;
        }

        bool isStart() const
        {
            return (path == 'S' || value == 'S');
        }
        
        bool isCountess() const
        {
            return value == 'C';
        }

        bool isWalkable()
        {
            return canWalk;
        }

        bool isWarpPipe() const
        {
            return std::isdigit(value);
        }

        void setPath(char inChar)
        {
            path = inChar;
            //if we're setting path, the tile is discovered and we can't walk on
            //it anymore
            canWalk = false; 
        }
        
        char getPath() const
        {
            return path;
        }

        void setValue(char inChar)
        {
            value = (std::isdigit(inChar)) ? 'p' : inChar;
        }

};


#endif // _TILE_H_
