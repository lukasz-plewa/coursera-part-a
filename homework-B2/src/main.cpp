
#include "hexgame.h"

const unsigned int board_dimension = 11;

int main()
{
    HexGame game(board_dimension);

    // Player can change board size by entering new dimension
    if (game.playerInitialize())
    {
        std::cout << "Interactive play starts..." << std::endl; 
        game.interactiveGamePlay();
    }
    else
    {
        Colour winner = game.GameSimulation(Colour::BLUE);  // Simulation, BLUE has first move
        if (winner != Colour::NONE)
        {
            std::cout << "Winner: " << winner << std::endl;
        }
    }

    std::cout << "Exiting." << std::endl;
    return 0;
}
