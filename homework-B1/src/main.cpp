#include <iostream>
#include <cstdlib>
#include <string>

#include "hexboard.h"

//
// HW 4 expectations:
// 1) Be able to draw the board using ASCII symbols and a given size, such as 7 by 7 or 11 by 11.
// 2) Input a move and determine if a move is legal.
// 3) Determine who won.
// 

int main()
{   
    const std::string player_turn[2] = {("RED (X)  > ") ,("BLUE (O) > ")};
    unsigned int turn = 0;              // RED player has first move
    unsigned int board_dimension = 11;

    std::cout << "Hex board size?" << std::endl;
    std::cin >> board_dimension;
    if ( board_dimension > MAX_BOARD_SIZE || board_dimension < 3 )
    {
        std::cerr << board_dimension << " is wrong Size. Valid are from 3 to " << MAX_BOARD_SIZE << std::endl;
        return 2;
    }

    // Create HexBoard object, initilize screen and print board
    HexBoard board(board_dimension);
    system("clear");
    board.printBoard();

    // User must input coordinates column and row number pair, for example: '2 9'
    for (std::string line; std::cout << player_turn[turn] && std::getline(std::cin, line); )
    {
        if (!line.empty())
        {
            HexBoardPosition move(line);
            if (move.isValid())
            {
                // Put user move and check if move is legal. If not legal, then  try again
                if (board.putColor(move, (turn == 0 ? Colour::RED : Colour::BLUE) ))
                {
                    system("clear");
                    board.printBoard();
                    std::cout << (turn == 0 ? "RED (X):  " : "BLUE (O): ") << move << std::endl;
                    if (board.RedWin() || board.BlueWin())
                        break;
                    turn = (turn ? 0 : 1);  // Next player (colour) turn
                }
            }
            else
            {
                std::cout << "Invalid input: \'" << move << "\' Try again." << std::endl;
            }
        }
    }
    
    std::cout << "Exiting." << std::endl;
    return 0;
}
