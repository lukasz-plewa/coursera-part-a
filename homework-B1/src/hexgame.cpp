#include <iostream>
#include <cstdlib>
#include <string>

#include "hexboard.h"

const std::string player_turn[2] = {("RED (X)  > ") ,("BLUE (O) > ")};

int main()
{
    unsigned int turn = 0;              // RED player has first move
    unsigned int board_dimension = 11;

    std::cout << "Hex board size?" << std::endl;
    std::cin >> board_dimension;
    if ( board_dimension > MAX_BOARD_SIZE || board_dimension < 3 )
    {
        std::cerr << board_dimension << " is wrong Size. Valid are from 3 to " << MAX_BOARD_SIZE << std::endl;
        return 2;
    }

    HexBoardGraph board(board_dimension);

    system("clear");
    board.printBoard();
    for (std::string line; std::cout << player_turn[turn] && std::getline(std::cin, line); )
    {
        if (!line.empty())
        {
            HexBoardPosition move(line);
            if (move.isValid())
            {
                if (board.putColor(move, (turn == 0 ? Colour::RED : Colour::BLUE) ))
                {
                    system("clear");
                    board.printBoard();
                    std::cout << (turn == 0 ? "RED (X):  " : "BLUE (O): ") << move << std::endl;
                    if (board.RedWin() || board.BlueWin())
                        break;
                    turn = (turn ? 0 : 1);  // Next turn
                }
            }
            else
            {
                if (line.compare("exit") == 0)
                {
                    break;
                }
                std::cout << "Invalid input: \'" << move << "\' Try again." << std::endl;
            }
        }
    }

    
    std::cout << "Exiting." << std::endl;
    return 0;
}
