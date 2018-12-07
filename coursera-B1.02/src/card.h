#ifndef CARD_H_
#define CARD_H_

#include <iostream>
#include "pips.h"

class Card
{
public:
    Card() : s(Suit::SPADE), v(1) {}
    Card(Suit s, Pips v) :  s(s), v(v) {}
    friend std::ostream& operator<<(std::ostream& out, const Card& c);
    Suit get_suit() { return s; }
    Pips get_pips() const { return v; }
private:
    Suit s;
    Pips v;
};


#endif //CARD_H_