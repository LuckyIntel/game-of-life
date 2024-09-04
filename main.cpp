/*
    Game of life in your console/terminal.
    Play with game's rules according to the instructions please.

    This thing ain't getting updates till something serious really occurs,
    game of school starts for me sadly.

    -1 : Dead / Non-Alive cells
    1 : Alive cells

    GAME_SIZE, CLEAR_SCREEN_COUNT can't be lower than 0 or can't be 0.

    The code first shows you the table, then runs the simulation calculation,
    this means the code just calculates the next move right before you see it.

    The reason i created my own time is i just tried <chrono> but it seems like
    i just mess up with something so i came up with my own idea.Maybe will fix
    this in future for precision.

    I made the code send all of the data in just one string array instead of
    outputting everything one by one.This will mostly make it more stable.

    I believe this code is problaly easy to understand for you all since even
    someone like me can understand it in one sight, you will too.I know the code
    isn't brilliant or something but i mean don't be greedy man.I tried my best.
*/
#include <iostream> // input, output
#include <vector> // std::vector
#include <cmath> // sqrt
#include <string> // std::string, std::getline
#include <fstream> // std::ifstream
#include <ctype.h> // isdigit
#define EMPTY_CELL '.' // Changing this will not cause any problem
#define FILLED_CELL '#' // Changing this will also not cause any problem
#define DEFAULT_GRID_PATH "table.txt" // Change if needed.
#define CLEAR_SCREEN_COUNT 255 // Don't dare to make it zero.
#define GAME_SIZE 900  // Needs to be a square of a number.Also don't make zero.(example : 900)
#define PER_SIZE static_cast<unsigned int>(sqrt(GAME_SIZE)) // Get's square root of game size.Don't change.Please.
#define TIME_STEP_SPEED 0.001 // Increasing or decreasing this will make the calculations and drawing faster.Can cause stability problems.

bool startGame = false; // Making this true will cause unexpected behavior in the start.The game itself will work as expected.
double globalTime = 0; // Increasing or decreasing this can cause problems.
std::vector<int> gameTable(GAME_SIZE, -1); // Creates a game table of the size of game size and makes all of the elements -1
std::string COUTSTR = ""; // Needed for cout.Don't change.Breaks output if missing.

inline bool importTable()
{
    std::ifstream tableFile(DEFAULT_GRID_PATH); // Feel free to play with this.Just type your file name instead of DEFAULT_GRID_PATH
    if (!tableFile.fail()) // If tableFile does exist and something unexpected doesn't occur.
    {
        std::string tableReader;
        while (std::getline(tableFile, tableReader))
        {
            int tIdx = std::stoi(tableReader);
            if (tIdx > 0 && tIdx < GAME_SIZE) gameTable[tIdx] *= -1;
        };
        tableFile.close();
        return true;
    };
    tableFile.close();
    return false;
};

inline bool isNumber(std::string stringToCheck)
{
    bool iN = true; // Making this false from start will make every input behave like a string instead of a number.Don't change.
    for (char idx : stringToCheck) { if (!isdigit(idx)) { iN = false; break; }; };
    return iN;
};

inline std::vector<char> createRepTable()
{
    std::vector<char> repTable(GAME_SIZE); // Unexpected behavior when another number given other than GAME_SIZE
    for (unsigned int i = 0; i < gameTable.size(); i++)
    {
        char representer;
        switch (gameTable[i])
        {
            case -1: representer = EMPTY_CELL; break;
            case 1: representer = FILLED_CELL; break;
        };
        repTable[i] = representer;
    };
    return repTable;
};

inline void drawTable()
{
    std::vector<char> repTable = createRepTable();
    for (unsigned int i = 0; i < repTable.size(); i++)
    {
        if (i % PER_SIZE == 0 && i != 0) COUTSTR += "\n";
        COUTSTR += repTable[i];
        COUTSTR += " ";
    };
    repTable.clear();
    repTable.shrink_to_fit();
};

const unsigned int getNeighborCount(unsigned int idx)
{
    unsigned int neighborCount = 0;
    bool searchLeft = true, searchRight = true, searchUp = true, searchDown = true;

    for (unsigned int i = 0; i < PER_SIZE; i++)
    {
        if (idx == PER_SIZE * i) searchLeft = false; else if (idx == (PER_SIZE * i) - 1 && idx != 0) searchRight = false;
        if (idx == i) searchUp = false; else if (idx == GAME_SIZE - 1 - i) searchDown = false;
    };
    
    if (searchRight && gameTable[idx + 1] == 1) neighborCount++; // Right
    if (searchLeft && gameTable[idx - 1] == 1) neighborCount++; // Left
    if (searchDown && gameTable[idx + PER_SIZE] == 1) neighborCount++; // Down
    if (searchDown && searchRight && gameTable[idx + PER_SIZE + 1] == 1) neighborCount++; // Down, right
    if (searchDown && searchLeft && gameTable[idx + PER_SIZE - 1] == 1) neighborCount++; // Down, left
    if (searchUp && gameTable[idx - PER_SIZE] == 1) neighborCount++; // Up
    if (searchUp && searchRight && gameTable[idx - PER_SIZE + 1] == 1) neighborCount++; // Up, right
    if (searchUp && searchLeft && gameTable[idx - PER_SIZE - 1] == 1) neighborCount++; // Up, left
    return neighborCount;
};

inline void simulateTable()
{
    for (unsigned int i = 0; i < gameTable.size(); i++)
    {
        unsigned int neighborCount = getNeighborCount(i);
        switch (gameTable[i])
        {
            case -1: if (neighborCount == 3) gameTable[i] *= -1; break;
            case 1: if (neighborCount < 2) gameTable[i] *= -1; else if (neighborCount > 3) gameTable[i] *= -1; break;
        };
    };
};

inline void editTable()
{
    std::string affectedIdx;
    drawTable();
    COUTSTR += "\nSelect index to edit (or type \"s\" to start) : ";
    std::cout << COUTSTR;
    COUTSTR.clear();
    COUTSTR.shrink_to_fit();
    std::getline(std::cin, affectedIdx);
    if (affectedIdx == "s") startGame = true;
    else if (isNumber(affectedIdx) && std::stoi(affectedIdx) >= 0 && std::stoi(affectedIdx) <= GAME_SIZE - 1) gameTable[std::stoi(affectedIdx)] *= -1;
    if (!startGame) editTable();
};

// I really didn't want to treat MacOS and other OS users like they are adopted this time.I may introduce you to my very not genius clear system.
inline void universalClear() { for (unsigned int i = 0; i < CLEAR_SCREEN_COUNT; i++) { COUTSTR += "\n"; }; };

int main()
{
    if (!importTable()) std::cout << "Can't find \"" << DEFAULT_GRID_PATH << "\"\nAsking for indices manually.\n";
    else std::cout << "Data successfully loaded from \"" << DEFAULT_GRID_PATH << "\"\n";

    editTable();

    while (startGame)
    {
        globalTime += TIME_STEP_SPEED;
        if (globalTime > 100000.0)
        {
            globalTime = 0;
            universalClear();
            drawTable();
            simulateTable();
            std::cout << COUTSTR;
            COUTSTR.clear();
            COUTSTR.shrink_to_fit();
        };
    };
};