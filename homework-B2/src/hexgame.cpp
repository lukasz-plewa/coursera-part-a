#include <cstdlib>
#include <string>
#include <random>

#include "hexgame.h"

//
//
//
// HexGame class
//
//
//
HexGame::HexGame(unsigned int size) :
                turn(Colour::BLUE),
                player_turn(Colour::BLUE),
                board_dimension(size)
{
    board = new HexBoardGraph(size);
    rng.seed(std::random_device{}());
    eval_board = nullptr;
}


// Evaluate a position using a Monte Carlo selection of moves
HexBoardPosition HexGame::EvalNextPosition(Colour col)
{
    const unsigned int trials = 500;
    // Helper board for evaluation calculations
    if (eval_board == nullptr)
        eval_board = new HexBoardGraph(board_dimension);

    std::vector<unsigned int> free_ids;
    // Load legal moves on board
    board->GetEmptyIds(free_ids);
    
    unsigned int best_id = 0;
    unsigned int best_id_count = 0;

    // Check every legal move
    for (auto id: free_ids)
    {
        unsigned int current_id_counter = 0;
        for (unsigned int i = 0; i < trials; ++i)
        {
            eval_board->InitializeBoardColors(board);
            eval_board->putColor(id, col);
            // Is our color win this time?
            if (EvalSimulation( (col==Colour::BLUE ? Colour::RED : Colour::BLUE)) == col)
            {
//                std::cout << "Our color wins when started from " << eval_board->graphIdToPosition(id) << std::endl;
                current_id_counter++;
            }
        }
        if (current_id_counter > best_id_count)
        {
//            std::cout << "Better starting position found: " << eval_board->graphIdToPosition(id) << " wins " << current_id_counter << std::endl;
            best_id_count = current_id_counter;
            best_id = id;
        }
    }

    return eval_board->graphIdToPosition(best_id);
}

// Pick random graph id from empty vertices (without color)
unsigned int HexGame::RandomMove(HexBoardGraph *b)
{
    std::vector<unsigned int> free_ids;
    b->GetEmptyIds(free_ids);

    if (free_ids.size() == 1)
        return free_ids[0];

    std::uniform_int_distribution<unsigned int> dist(0, free_ids.size()-1);
    return free_ids[dist(this->rng)];
}

// Perform move with Monte Carlo evaluation
bool HexGame::ComputerMove(HexBoardGraph* b, Colour col)
{
    if (b->boardFull())
    {
        return false;
    }
    else
    {
        HexBoardPosition pos = EvalNextPosition(col);
        std::cout << "Computer move: " << pos << std::endl;
        return b->putColor(pos, col);
    }
}

// Just put player color on board if position empty
bool HexGame::PlayerMove(const HexBoardPosition &pos, Colour c)
{
    return board->putColor(pos, c);
}

// Get player move from console
HexBoardPosition& HexGame::getPlayerMove(HexBoardPosition &pos, const Colour c)
{
    for (std::string line; std::cout << playerPrompt(c) << " > " && std::getline(std::cin, line);)
    {
        if (!line.empty())
        {
            HexBoardPosition position(line);
            if (position.isValid())
            {
                pos = position;
                break;
            }
        }
    }
    return pos;
}

// Before start game, get board size and player color
// If user will input wrong color (for example "aa") then game simulation will start
bool HexGame::playerInitialize()
{
    std::string player_color;
    unsigned int dimension = 0;

    std::cout << "Hex board size?" << std::endl;
    std::cin >> dimension;
    std::cout << "Which player you want to play? r - red (O), b - blue (O)" << std::endl;
    std::cin >> player_color;

    if ( dimension > MAX_BOARD_SIZE || dimension < 3 )
    {
        std::cerr << dimension << " is wrong Size. Valid are from 3 to " << MAX_BOARD_SIZE << std::endl;
        return false;
    }
    if ( player_color.size() != 1 )
    {
        std::cerr << player_color << " is wrong. Valid are: \'r\' and \'b\' " << std::endl;
        return false;
    }
    // By default player is blue, change to red if chosen 'r'
    if (*player_color.begin() == 'r') player_turn = Colour::RED;

    // Modify board dimension when chosen different than default
    if (dimension != this->board_dimension)
    {
        delete board;
        board = new HexBoardGraph(dimension);
        std::cout << "Board size changed from " << board_dimension << " to " << dimension << std::endl;
        this->board_dimension = dimension;
    }
    return true;
}

// Returns color of the winner
Colour HexGame::GameSimulation(Colour first)
{
    Colour turn = first;
    Colour winner = Colour::NONE;

    while(ComputerMove(board, turn))
    {
        if (turn == Colour::BLUE)
        {
            if (board->BlueWin()) { winner = Colour::BLUE; break; }
        }
        else
        { 
            if (board->RedWin()) { winner = Colour::RED; break; }
        }
        ++turn; // next player move
    };
    return winner;
}

// Returns color of the winner on evaluation board
Colour HexGame::EvalSimulation(Colour first)
{
    Colour turn = first;
    Colour winner = Colour::NONE;

    while(!eval_board->boardFull())
    {
        eval_board->putColor(RandomMove(eval_board), turn);
        ++turn; // next player move
    };

    if (eval_board->BlueWin()) { winner = Colour::BLUE; }
    else if (eval_board->RedWin()) { winner = Colour::RED; }

    return winner;
}

// A loop controlling game play with computer
bool HexGame::interactiveGamePlay()
{
    HexBoardPosition move;
    bool have_winner = false;

    //    system("clear");
    printBoard();
    while(!board->boardFull())
    {
        if (turn == player_turn)
        {
            getPlayerMove(move, turn);
            if (PlayerMove( move, turn) == false)
                continue;
        }
        else
        {
            ComputerMove(board, turn);
        }
        system("clear");
        printBoard();
        std::cout << "Free positions: " << board->freePositionsCount() << std::endl;
        std::cout << playerPrompt(turn) << ": " << move << std::endl;
        if (checkWin())
        {
            have_winner = true;
            break;
        }
        ++turn;
    }
    return have_winner;
}

// Checks if any color win
bool HexGame::checkWin()
{
    if (board->RedWin())
    {
        std::cout << "Red player wins" << std::endl;
        return true;
    }
    else if (board->BlueWin())
    {
        std::cout << "Blue player wins" << std::endl;
        return true;
    }
    return false;
}

