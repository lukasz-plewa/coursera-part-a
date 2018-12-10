#include <iostream>
#include <cstdlib>

#include "hexboard.h"

void process(std::string &line);

const unsigned int board_dimention = 11;

int main()
{
    std::cout << "Creating board with size " << board_dimention << std::endl;
    HexBoardGraph board(board_dimention);
    board.printGraph();
    std::cout << "This board " << (board.isConnected() ? "is" : "is not") << " connected" << std::endl;

    board.putColor(1, 1, Colour::RED);
    board.putColor(1, 2, Colour::RED);
    board.putColor(1, 3, Colour::RED);
    board.putColor(1, 4, Colour::RED);
    board.putColor(4, 1, Colour::RED);
    board.putColor(11, 9, Colour::BLUE);
    board.putColor(11, 10, Colour::BLUE);
    board.putColor(10, 11, Colour::BLUE);

    for (std::string line; std::cout << "APP > " && std::getline(std::cin, line); )
    {
        unsigned int xpos = 0;
        if (!line.empty())
        {
            auto inputC = line.begin();
            system("clear");
            std::cout << "Input << " << *inputC;
            if (*inputC >= 'a' && *inputC <= 'k')
            {
                xpos = static_cast<unsigned int>(*inputC - 'a') + 1;
                std::cout << "Entered pos " << xpos << std::endl;

                inputC++;
                if(*inputC > '0' && *inputC <= '9')
                {
                    
                        std::cout << *inputC << std::endl;
            
                }
                board.printBoard();
            }
        }

        
        std::cout << "Entered pos " << xpos << std::endl;
    }

    board.printBoard();
}
