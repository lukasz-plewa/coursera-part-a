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
    HexBoardPosition() : valid(false), x(0), y(0) {}
    explicit HexBoardPosition(unsigned int X, unsigned int Y) : valid(false), x(X), y(Y)
    { 
        if (X > 0 && X <= MAX_BOARD_SIZE && Y > 0 && Y <= MAX_BOARD_SIZE) valid = true;
    }
    explicit HexBoardPosition(const std::string &pos);
    HexBoardPosition(const HexBoardPosition &original);
    unsigned int getX() const { return x; }
    unsigned int getY() const { return y; }
    bool isValid() { return valid; }

    friend class HexBoardGraph;
};


inline std::ostream& operator<< (std::ostream& os, const HexBoardPosition& pos)
{
    os << "(" << static_cast<char>('a' + pos.getX() - 1) << " " << pos.getY() << ")"; return os;
}

//
// Hex game board representation class.
// Inherits from MstGraph and uses HexBoardPosition class to make easier game implementation
//
class HexBoardGraph : public MstGraph
{
public:
    HexBoardGraph() : MstGraph(), dimension(0), occupied(0) {}
    HexBoardGraph(unsigned int n);      // Board with n rows and colls
    HexBoardGraph(const HexBoardGraph &board);

    void Add(unsigned int from, unsigned int to) { this->MstGraph::Add(from, to, 1); }
    bool RedWin();
    bool BlueWin();

    void InitializeBoardColors(const HexBoardGraph *source_board);
    unsigned int BoardSize() { return dimension; }
    void printBoard();
    bool putColor(const HexBoardPosition& p, Colour c);
    bool putColor(unsigned int id, Colour c);
    // Calculate from graph ID to HEX board position
    inline HexBoardPosition graphIdToPosition(unsigned int Id) {
        return HexBoardPosition(Id % dimension + 1, Id / dimension + 1); }
    // Translate HEX board position to graph ID
    inline unsigned int HexBoardPositionToId(const HexBoardPosition &pos) {
        return ( (pos.y-1) * dimension + pos.x - 1); }
    inline bool isIdFree(unsigned int id) { return (graph[id]->getColour() == Colour::NONE); }
    inline bool boardFull() { return (occupied == this->V()); }
    inline unsigned int freePositionsCount() { return (this->V() - occupied); }
    void GetEmptyIds(std::vector<unsigned int>& list);

private:
    unsigned int dimension;
    unsigned int occupied;
};


#endif //HEXBOARD_H_
