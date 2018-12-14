#ifndef HEXBOARD_H_
#define HEXBOARD_H_

#include "graph.h"

const unsigned int MAX_BOARD_SIZE  = 20;

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

    friend class HexBoardGraph;
};


inline std::ostream& operator<< (std::ostream& os, const HexBoardPosition& pos)
{
    os << "(" << static_cast<char>('A' + pos.getX() - 1) << " " << pos.getY() << ")"; return os;
}


class HexBoardGraph : public MstGraph
{
public:
    HexBoardGraph() : MstGraph(), dimension(0) {}
    HexBoardGraph(unsigned int n);      // Board with n rows and colls
    void Add(unsigned int from, unsigned int to) { this->MstGraph::Add(from, to, 1); }
    bool RedWin();
    bool BlueWin();

    unsigned int BoardSize() { return dimension; }
    void printBoard();
    bool putColor(const HexBoardPosition& p, Colour c);

private:
    unsigned int dimension;
    HexBoardPosition graphIdToPosition(unsigned int Id);
    unsigned int HexBoardPositionToId(const HexBoardPosition &pos);
};

#endif //HEXBOARD_H_