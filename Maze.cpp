//Project Identifier: B99292359FFD910ED13A7E6C7F9705B8742F0D79
#include "Maze.h"

// Read and process command line options.
void Maze::getOptions(int argc, char** argv) {
    int option_index = 0, option = 0;
    int searchOpts = 0;
    int outputOpt = 0;
    // Don't display getopt error messages about options
    opterr = false;

    // use getopt to find command line options
    struct option longOpts[] = {{ "stack", no_argument, nullptr, 's' },
                                { "queue", no_argument, nullptr, 'q' },
                                { "output", required_argument, nullptr, 'o'},
                                { "help", no_argument, nullptr, 'h'},
                                { nullptr, 0, nullptr, '\0' }};

    while ((option = getopt_long(argc,
                                 argv,
                                 "sqo:h",
                                 longOpts,
                                 &option_index)
                                    ) != -1) 
    {
        switch (option) {
            case 's':
                ++searchOpts;
                if (searchOpts > 1)
                {
                    //prevent setting stack mode and queue mode
                    std::cerr << "Stack or queue can only be specified once\n";
                    exit(1);
                }
                else
                {
                    isStackMode = true;
                }
                break;
            case 'q':
                ++searchOpts;
                if (searchOpts > 1)
                {
                    //prevent setting stack mode and queue mode
                    std::cerr << "Stack or queue can only be specified once\n";
                    exit(1);
                }
                else
                {
                    isStackMode = false;
                }
                
                break;
            case 'o':
                ++outputOpt;
                if (*optarg == 'M')
                {
                    isMapOutput = true;
                }
                else
                {
                    isMapOutput = false;
                }
                break;             
            case 'h':  //TO DO: update help message
                std::cout << "This program reads a file that contains a maze.\n"
                          << "It can use different search methods to find the\n"
                          << "path from a starting location to the countess.\n"
                          << "If output mode is flagged, an M or L is "
                          << "required (map or list).\n"
                          << "Usage: \'./project1\n\t[--stack | -s]\n"
                          <<                      "\t[--queue | -q]\n"
                          <<                      "\t[--output | -o] <M/L>\n"
                          <<                      "\t[--help | -h]\n"
                          <<                      std::endl;
                exit(0);
            default:
                std::cerr << "Unknown command line option\n";
                exit(1);
        }
    } //while
    
    // stack or queue must be specified
    if (searchOpts == 0)
    {
        std::cerr << "Stack or queue must be specified\n";
        exit(1);
    }
    if (outputOpt < 1) //default to map output
    {
        isMapOutput = true;
    }
}

bool Maze::readData()
{
    bool success = true;

    readHeader(); //we can assume lines 1-3 are correct

    //resize each room/row/col vetor as we go
    //for map mode we'll read in data from within this loop
    map.resize(numRooms);

    for (uint32_t roomIdx = 0; roomIdx < numRooms && success; ++roomIdx)
    {
        map[roomIdx].resize(roomSize);

        for (uint32_t rowIdx = 0; rowIdx < roomSize && success; ++rowIdx)
        {
            map[roomIdx][rowIdx].resize(roomSize);

            if (isMapInput)
            {
                success = readMapInput(roomIdx, rowIdx);
            }
            
        }
    }
    
    //for list mode we need to read after resizing
    if (!isMapInput)
    {
        success = readListInput();
    }

    return success;
}

bool Maze::readMapInput(uint32_t room, uint32_t row)
{
    bool success = true;
    char input;
    char COMMENT = '/';
    std::string junk;

    for (uint32_t col = 0; col < roomSize && success; ++col)
    {
        if (std::cin >> input)
        {
            //remove entire line if this is a comment
            if (input == COMMENT)
            {
                getline(std::cin, junk);
                --col; //reset column index
            }
            else
            {
                Location nextLoc(room, row, col);
                success = addTileToMap(nextLoc, input);
            }
        }
        else
        {
            success = false;
        }
    }

    return success;
}

bool Maze::readListInput()
{
    bool success = true;
    char input;
    const char COMMENT = '/';
    char junk;
    std::string junkStr;
    uint32_t room;
    uint32_t row;
    uint32_t col;
    Tile tile;
    Location loc;

    while (std::cin >> input && success) //reads in '/' or '('
    {
        if (input == COMMENT)
        {
            getline(std::cin, junkStr);
        }
        else
        {
            std::cin >> room;
            std::cin >> junk; //read in comma
            std::cin >> row;
            std::cin >> junk; //read in comma
            std::cin >> col;
            std::cin >> junk;  //read in comma
            std::cin >> input;
            getline(std::cin, junkStr);
            
            loc = Location(room, row, col);
            success = (isValidLocation(loc, true) && addTileToMap(loc, input));
        }
    }

    return success;
}

bool Maze::addTileToMap(Location &nextLoc, char input)
{
    bool success = true;

    if (input == 'S') //initiate search if this is the starting spot
    {
        Tile tile(input);
        map[nextLoc.room][nextLoc.row][nextLoc.col] = tile;
        addLocationToSearch(nextLoc, input);
        startLoc = nextLoc;
    }
    else if (std::isdigit(input) ||
                input == '!' ||
                input == '#' ||
                input == 'C')
    {
        Tile tile(input);
        map[nextLoc.room][nextLoc.row][nextLoc.col] = tile;
    }
    //no need to add '.' since that's the default
    else if (input != '.')
    {
        success = false;
        std::cerr << "Unknown map character\n";
    }
    
    return success;
}

bool Maze::isValidLocation(const Location &inLoc, bool outputErr) const
{
    bool isValid = true;

    if (inLoc.room >= numRooms)
    {
        isValid = false;
        if (outputErr)
        {
            std::cerr << "Invalid room number\n";
        }
    }
    else if (inLoc.row >= roomSize)
    {
        isValid = false;
        if (outputErr)
        {
            std::cerr << "Invalid row number\n";
        }
    }
    else if (inLoc.col >= roomSize)
    {
        isValid = false;
        if (outputErr)
        {
            std::cerr << "Invalid column number\n";
        }
    }

    return isValid;
}

void Maze::readHeader()
{
    char inputMode;
    char MAP_MODE = 'M';

    std::cin >> inputMode;
    std::cin >> numRooms;
    std::cin >> roomSize;
    
    if (inputMode == MAP_MODE)
    {
        isMapInput = true;
    }
    else
    {
        isMapInput = false;
    }
}

void Maze::processSearchContainer()
{
    //remove next tile in container
    //add n,e,s,w tiles to container
    Location loc;
    const uint32_t NUM_DIRS = 4;
    char travelDir[NUM_DIRS] = {'n', 'e', 's', 'w'};
    char dir;
    Location locToAdd;
    
    //To DO stop if countess found

    if (isStackMode) //if we're in stack mode pop from back (last one in)
    {
        loc = searchContainer.back();
        searchContainer.pop_back();
    }
    else //we're in queue mode so pop from front
    {
        loc = searchContainer.front();
        searchContainer.pop_front();
    }

    Tile curTile = map[loc.room][loc.row][loc.col];

    if (curTile.isWarpPipe())
    {
        dir = static_cast<char>('0' + loc.room);
        
        //check if pipe leads to room that exists
        if (getNextLocation(loc, curTile.getValue(), locToAdd, false))
        {
            addLocationToSearch(locToAdd, dir);
        }
    }
    else //otherwise add n/s/e/w tiles if possible
    {
        for (size_t i = 0; i < NUM_DIRS && solnPath.empty(); ++i)
        {
            dir = travelDir[i];
            //check if we're at edge of room
            if (getNextLocation(loc, dir, locToAdd, false))
            {
                addLocationToSearch(locToAdd, dir);
            }
        }
    }
}

bool Maze::getNextLocation(Location &inLoc,
                            char dir,
                            Location &outLoc,
                            bool reverse)
{
    bool isValid = false;
    int idxIncrement = -1;
    uint32_t row = inLoc.row;
    uint32_t col = inLoc.col;
    uint32_t room = inLoc.room;

    if (reverse) //increment differently if we're going backwards
    {
        idxIncrement = 1;
    }
    
    if (dir == 'n')
    {
        row += idxIncrement;
    }
    else if (dir == 'e')
    {
        col -= idxIncrement;
    }
    else if (dir == 's')
    {
        row -= idxIncrement;
    }
    else if (dir == 'w')
    {
        col += idxIncrement;
    }
    else if (std::isdigit(dir)) //we came from a warp pipe
    {
        room = static_cast<uint32_t>(dir - '0');    
    }

    outLoc = Location(room, row, col);
    if (isValidLocation(outLoc, false))
    {
        isValid = true;
    }

    return isValid;
}

void Maze::addLocationToSearch(Location &inLoc, char dir)
{
    Tile inTile = map[inLoc.room][inLoc.row][inLoc.col];
    
    if (inTile.isWalkable())
    {
        inTile.setPath(dir);
        //update map location w/ tile changes
        map[inLoc.room][inLoc.row][inLoc.col] = inTile;
        searchContainer.push_back(inLoc); //alwys add to back on deque
        numTilesDiscovered++;

        if (inTile.isCountess())  //if we found the countess, update soln
        {
            solnPath.push(inLoc);
        }
    }
}

void Maze::backTraceSoln()
{
    Tile tmpTile;
    Location loc;
    Location prevLoc;
    char dir = 'x';

    if (!solnPath.empty())
    {
        loc = solnPath.top();
        solnPath.pop(); //this removes countess location since we don't need it
        tmpTile = map[loc.room][loc.row][loc.col];
        
        while (!tmpTile.isStart())
        {
            dir = tmpTile.getPath();

            //find previous location
            getNextLocation(loc, dir, prevLoc, true);

            //get tile at previous location
            tmpTile = map[prevLoc.room][prevLoc.row][prevLoc.col];
            tmpTile.setValue(dir); //update value w/ path we used
            map[prevLoc.room][prevLoc.row][prevLoc.col] = tmpTile;
            solnPath.push(prevLoc); //add new location to soln path
            loc = prevLoc;
        }
    }
}

void Maze::outputSoln()
{
    //TO DO output map
    if (solnPath.empty())
    {
        std::cout << "No solution, " 
                  << std::to_string(numTilesDiscovered) 
                  << " tiles discovered.\n";
    }
    else
    {
        if (isMapOutput)
        {
            outputMap();
        }
        else
        {
            outputList();
        }
    }
}

void Maze::outputList()
{
    Location loc;
    Tile tile;

    std::cout << "Path taken:\n";
    while (!solnPath.empty())
    {
        loc = solnPath.top();
        tile = map[loc.room][loc.row][loc.col];
        std::cout << '(' << loc.room << ',' << loc.row << ',' << loc.col << ','
                  << tile.getValue() << ')' << "\n";
        solnPath.pop();
    }
}

void Maze::outputMap()
{
    Tile tile;
    std::cout << "Start in room " << startLoc.room
              << ", row " << startLoc.row << ", column "
              << startLoc.col << "\n";
              
    for (uint32_t roomIdx = 0; roomIdx < numRooms; ++roomIdx)
    {
        std::cout << "//castle room " << roomIdx << "\n";

        for (uint32_t rowIdx = 0; rowIdx < roomSize; ++rowIdx)
        {
            for (uint32_t colIdx = 0; colIdx < roomSize; ++colIdx)
            {   
                tile = map[roomIdx][rowIdx][colIdx];
                std::cout << tile.getValue();
            }

            std::cout << "\n";
        }

    }
}
