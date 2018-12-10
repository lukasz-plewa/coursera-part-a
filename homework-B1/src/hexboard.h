#ifndef HEXBOARD_H_
#define HEXBOARD_H_

#include "graph.h"

class HexBoardGraph : public ListGraph
{
public:
    HexBoardGraph() : ListGraph(), dimension(0) {}
    HexBoardGraph(unsigned int n);      // Board with n rows and colls
    void Add(unsigned int from, unsigned int to) { this->ListGraph::Add(from, to, 1); }

    void printBoard();
    void putColor(unsigned int x, unsigned int y, Colour c);

private:
    unsigned int dimension;
};

#endif //HEXBOARD_H_