#ifndef HEXBOARD_H_
#define HEXBOARD_H_

#include "graph.h"

const unsigned int MAX_BOARD_SIZE  = 11;

// Simple class to abstract board position over our graph representation
class HexBoardPosition
{
    bool valid;
    unsigned int x, y;          // Indexed from 1
    
public:
    explicit HexBoardPosition(unsigned int X, unsigned int Y) : valid(false), x(X), y(Y)
    {
        if (X > 0 && X <= MAX_BOARD_SIZE && Y > 0 && Y <= MAX_BOARD_SIZE) valid = true;
    }
    explicit HexBoardPosition(const std::string &pos);
    unsigned int getX() const { return x; }
    unsigned int getY() const { return y; }
    bool isValid() { return valid; }

    friend class HexBoard;
};

// output operator to print position coordinates on board
inline std::ostream& operator<< (std::ostream& os, const HexBoardPosition& pos)
{
    os << "(" << pos.getX() << " " << pos.getY() << ")"; return os;
}

/*
 * Hex game board representation class. 
 * Inherits from MstGraph and uses HexBoardPosition class to make easier game implementation
 * I'm using in this class a graph representation developed in Part A of that course
 */
class HexBoard : public MstGraph
{
public:
    HexBoard() : MstGraph(), dimension(0) {}
    HexBoard(unsigned int n);      // Board with n * n size
    void Add(unsigned int from, unsigned int to) { this->MstGraph::Add(from, to, 1); }
    bool RedWin();
    bool BlueWin();

    unsigned int BoardSize() { return dimension; }
    void printBoard();
    bool putColor(const HexBoardPosition& p, Colour c);

private:
    unsigned int dimension;
    unsigned int occupied;
    HexBoardPosition graphIdToPosition(unsigned int Id);
    unsigned int HexBoardPositionToId(const HexBoardPosition &pos);
};

#endif //HEXBOARD_H_