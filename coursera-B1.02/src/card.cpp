
#include "card.h"

std::ostream & operator<<(std::ostream& out, const Card& c)
{
    out << c.v << c.s; // presumes << overloaded for Pips and Suit
    return out;
}
