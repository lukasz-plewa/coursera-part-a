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

HexBoardGraph::HexBoardGraph(unsigned int n) : MstGraph(n*n), dimension(n), occupied(0)
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

// create vector with IDs of empty vertices (without color)
void HexBoardGraph::GetEmptyIds(std::vector<unsigned int>& list)
{
    for (unsigned int i = 0; i < V(); ++i)
    {
        if( getColour(i) == Colour::NONE)
        {
            list.push_back(i);
        }
    }
}

// Copy graph colors from source board - handy for move evaluation
void HexBoardGraph::InitializeBoardColors(const HexBoardGraph *source_board)
{
    if (dimension == source_board->dimension)
    {
        for (unsigned int id = 0; id < this->V(); ++id)
        {
            setColour(id, source_board->getColour(id));
        }
        this->occupied = source_board->occupied;
    }
}

// Print board as ASCII - used for every game iteration
void HexBoardGraph::printBoard()
{
    std::cout << "RED (" << Colour::RED << ") is on top and bottom." << std::endl
              << "BLUE (" << Colour::BLUE << ") is on left and right." << std::endl;
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

// Try to put color on board position 
bool HexBoardGraph::putColor(const HexBoardPosition& p, Colour c)
{
    if (p.x && p.y && p.x <= dimension && p.y <= dimension)
    {
        unsigned int id = (p.y-1) * dimension + (p.x-1);
        if (getColour(id) == Colour::NONE)
        {
            setColour(id, c);
            occupied++;
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

// Try to put color on board - using direct graph id
bool HexBoardGraph::putColor(unsigned int id, Colour c)
{
    if (getColour(id) == Colour::NONE)
    {
        setColour(id, c);
        occupied++;
        return true;
    }

    std::cerr << "ERROR, position (" << graphIdToPosition(id) << ") already occupied by " << graph[id]->getColour() << std::endl;
    return false;
}

// Check if RED player already wins (is connection from top to bottom row)
bool HexBoardGraph::RedWin()
{
    // Iterate on top and bottom rows
    for(auto rowTop = graph.begin(); rowTop != graph.begin() + dimension; ++rowTop)
    {
        if ((*rowTop)->getColour() == Colour::RED)
        {
            // Now start from first Vertex in last row
            for(auto rowBottom = graph.begin() + dimension * (dimension-1); rowBottom != graph.end(); ++rowBottom)
            {
                if ((*rowBottom)->getColour() == Colour::RED)
                {
                    if (this->areConnected((*rowTop)->Id(), (*rowBottom)->Id(), Colour::RED))
                    {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Check if BLUE player already wins (is connection from left to right side)
bool HexBoardGraph::BlueWin()
{
    for(auto colLeft = graph.begin(); colLeft != graph.end(); colLeft+=dimension)
    {
        if ((*colLeft)->getColour() == Colour::BLUE)
        {
            auto colRight = graph.begin();
            while (colRight != graph.end())
            {
                colRight += (dimension - 1);    // move to last element in row
                if ((*colRight)->getColour() == Colour::BLUE &&
                    areConnected((*colLeft)->Id(), (*colRight)->Id(), Colour::BLUE))
                {
                        return true;
                }
                ++colRight;     // move one after last in row (next row or end())
            }
        }
    }
    return false;
}

//
//
// HexBoardPosition class
//
//
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
        std::cerr << "ERROR, wrong input to board position: Example: \'c 3\'" << std::endl;
    }
}

// HexBoardPosition copy constructor
HexBoardPosition::HexBoardPosition(const HexBoardPosition &original)
{
    valid = original.valid;
    x = original.x;
    y = original.y;
}
