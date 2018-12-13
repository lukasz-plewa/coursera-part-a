#include <iterator>
#include <string>
#include <sstream>

#include "hexboard.h"

/*
 5 x 5 hex board graph example (5 rows and 5 cols):

. - . - . - . - .
 \ / \ / \ / \ / \
  . - . - . - . - .
   \ / \ / \ / \ / \
    . - . - . - . - .
     \ / \ / \ / \ / \
      . - . - . - . - .
       \ / \ / \ / \ / \
        . - . - . - . - .

*/

HexBoardGraph::HexBoardGraph(unsigned int n) : ListGraph(n*n), dimension(n)
{
    unsigned int row, node;
    unsigned int current = 0;

    // Connections for first row (consequent nodes only):
    for(node = 0; node < (n-1); ++node)
    {
        Add( node, node+1 );
    }
    // Connections for 2nd and other rows
    for(row = 1; row < n; ++row)
    {
        for(node = 0; node < n; ++node)
        {
            current = row * n + node;
            // Connection to upper row with same offset
            Add( current, current - n );
            // Connection to upper row and next node: only if not last node in row
            if(node < (n-1))
            {
                Add( current, current - n + 1 );
                Add( current, current + 1 );
            }
        }
    }
}

void HexBoardGraph::printBoard()
{
    for (unsigned int row = 0; row < dimension; ++row)
    {
        std::string indent((2*row), ' ');
        std::cout << indent;
        for (unsigned int i = 0; i < dimension; ++i)
        {
            std::cout << graph[row*dimension+i]->getColour();
            if (i < (dimension-1))
                std::cout << " - ";
        }
        std::cout << std::endl;
        if (row == (dimension-1))
            break;
        std::cout << indent;
        for (unsigned int i = 0; i < (dimension-1); ++i)
        {
            std::cout << " \\ /";
        }
        std::cout << " \\" << std::endl;
    }
}

/*
 * x - position in row (A - K)
 * y - row number (1 - 11)
 */
bool HexBoardGraph::putColor(const HexBoardPosition& p, Colour c)
{
    if (p.x && p.y && p.x <= dimension && p.y <= dimension)
    {
        unsigned int id = (p.y-1) * dimension + (p.x-1);
        if (getColour(id) == Colour::NONE)
        {
            setColour(id, c);
            return true;
        }
        else
            std::cerr << "ERROR, position (" << p << ") already occupied by " << graph[id]->getColour() << std::endl;
    }
    else
    {
        std::cerr << "ERROR, wrong coordinates" << std::endl;
    }
    return false;
}

// Check if RED player already wins (is connection from top to bottom row)
bool HexBoardGraph::RedWin()
{

}

// Check if BLUE player already wins (is connection from left to right side)
bool HexBoardGraph::BlueWin()
{

}


/*
 * HexBoardPosition
 */

HexBoardPosition::HexBoardPosition(const std::string &pos) : valid(false)
{
    std::istringstream iss(pos);
    std::vector<std::string> splited((std::istream_iterator<std::string>{iss}),
                                      std::istream_iterator<std::string>());
    unsigned int X = 0, Y = 0;

    if(splited.size() == 2 && splited[0].size() == 1 && splited[1].size() > 0 && splited[1].size() < 3)
    {
        X = splited[0].at(0) + 1 - 'a';
        Y = std::stoi(splited[1]);
        if (X > 0 && X <= MAX_BOARD_SIZE && Y > 0 && Y <= MAX_BOARD_SIZE)
        {
            x = X;
            y = Y;
            valid = true;
        }
        else
            std::cerr << "ERROR, Position not valid." << std::endl;
    }
    else
    {
        std::cerr << "ERROR, wrong input to board position: Example: \'C 3\'" << std::endl;
    }
}

