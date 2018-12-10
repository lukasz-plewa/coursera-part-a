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
void HexBoardGraph::putColor(unsigned int x, unsigned int y, Colour c)
{
    if (x && y && x <= dimension && y <= dimension)
    {
        unsigned int id = (y-1) * dimension + (x-1);
        if (getColour(id) == Colour::NONE)
            setColour(id, c);
        else
            std::cerr << "ERROR, position (" << x << ", " << y << ") already occupied by " << graph[id]->getColour() << std::endl;
    }
    else
    {
        std::cerr << "ERROR, wrong coordinates" << std::endl;
    }
}