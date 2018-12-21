#ifndef HEXGAME_H_
#define HEXGAME_H_

#include <string>
#include <random>

#include "hexboard.h"


class HexGame
{
    HexBoardGraph* board;
    HexBoardGraph* eval_board;
    std::mt19937 rng;

    Colour turn;
    Colour player_turn;
    unsigned int board_dimension;
    const std::string player_prompt[3] = {("ERROR"), ("BLUE (X)"), ("RED (O)")};
    inline const std::string& playerPrompt(Colour c) { return player_prompt[static_cast<int>(c)]; }
    unsigned int RandomMove(HexBoardGraph *b);
    bool ComputerMove(HexBoardGraph* b, Colour col);
    bool PlayerMove(const HexBoardPosition &pos, Colour c);
    bool checkWin();
    HexBoardPosition EvalNextPosition(Colour c);
public:
    explicit HexGame(unsigned int size);
    ~HexGame() { delete board; }
    bool playerInitialize();
    HexBoardPosition& getPlayerMove(HexBoardPosition &pos, const Colour c);
    inline void printBoard() { board->printBoard(); }
    Colour GameSimulation(Colour first);
    Colour EvalSimulation(Colour first);
    bool interactiveGamePlay();
};

#endif //HEXGAME_H_
