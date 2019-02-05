#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iterator>
#include "card.h"

void init_deck(std::vector<Card> &d);
void print_deck(std::vector<Card> &deck);
bool is_flush(std::vector<Card> &hand);
bool is_straight(std::vector<Card> &hand);
bool is_straight_flush(std::vector<Card> &hand);

int main(int argc, const char * argv[])
{
    Card card1(Suit::HEART, 3);
    std::cout << "Our first card is: " << card1 << std::endl;

    std::vector<Card> deck(53);
    init_deck(deck);
    print_deck(deck);

    std::cout << std::endl << "Now prepare a monte carlo simulation" << std::endl;

    int how_many;
    int flush_count = 0;
    int str_count = 0;
    int str_flush_count = 0;
    std::mt19937 gen{std::random_device{}()};   // Random number generator
    std::cout << "How many shuffles?" << std::endl;
    std::cin >> how_many;

    for(int loop = 0; loop < how_many; ++loop)
    {
        std::shuffle( deck.begin(), deck.end(), gen);
        std::vector<Card> hand(5);
        int i = 0;
        for(auto p = deck.begin(); i<5; ++p)
            hand[i++] = *p;
        if (is_flush(hand))
            flush_count++;
        if(is_straight(hand))
            str_count++;
        if(is_straight_flush(hand))
            str_flush_count++;
    }
    std::cout << "Flushes          " << flush_count << " out of " << how_many << std::endl;
    std::cout << "Straights        " << str_count << " out of " << how_many << std::endl;
    std::cout << "Straight Flushes " << str_flush_count << " out of " << how_many << std::endl;

    return 0;
}


void init_deck(std::vector<Card> &d)
{
    const int spade_offset = -1;
    const int heart_offset = 12;
    const int diamond_offset = 25;
    const int club_offset = 38;
    for (int i = 1; i < 14; ++i)
    {
        Card spade(Suit::SPADE, i);
        d[i+spade_offset] = spade;

        Card heart(Suit::HEART, i);
        d[i+heart_offset] = heart;

        Card diamond(Suit::DIAMOND, i);
        d[i+diamond_offset] = diamond;

        Card club(Suit::CLUB, i);
        d[i+club_offset] = club;
    }
}

void print_deck(std::vector<Card> &deck)
{
    std::copy(deck.begin(), deck.end(), std::ostream_iterator<Card>(std::cout, ", "));
    std::cout << std::endl;
}

bool is_flush(std::vector<Card> &hand)
{
    Suit s = hand[0].get_suit();
    for( auto p = hand.begin()+1; p != hand.end(); ++p)
    {
        if(s != p->get_suit())
            return false;
    }
    return true;
}

bool is_straight(std::vector<Card> &hand)
{
    int pips_v[5], i = 0;

    for(auto p = hand.begin(); p != hand.end(); ++p)
    {
        pips_v[i++] = (p->get_pips()).get_pips();
    }
    std::sort(pips_v, pips_v+5); // STL iterator range
    if(pips_v[0] != 1) // non aces
    {
        return (pips_v[0] == pips_v[1]-1 && pips_v[1] == pips_v[2]-1) && (pips_v[2] == pips_v[3]-1 && pips_v[3] == pips_v[4]-1);
    }
    else // aces has a specific logic
    {
        return (pips_v[0] == pips_v[1]-1 && pips_v[1] == pips_v[2]-1) && (pips_v[2] == pips_v[3]-1 && pips_v[3] == pips_v[4]-1)
            || (pips_v[1] == 10) && (pips_v[2] == 11) && (pips_v[3] == 12) && (pips_v[4] == 13);
    }
}


bool is_straight_flush(std::vector<Card> &hand)
{
    return is_flush(hand) && is_straight(hand);
}