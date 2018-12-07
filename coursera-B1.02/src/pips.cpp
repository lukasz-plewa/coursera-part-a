#include <iostream>
#include "pips.h"

std::ostream& operator<<(std::ostream& out, const Pips& p)
{
    out << p.v;
    return out;
}

std::ostream& operator<<(std::ostream& out, const Suit& s)
{
    switch(s)
    {
        case Suit::SPADE:
            out << "SPADE";
            break;
        case Suit::HEART:
            out << "HEART";
            break;
        case Suit::DIAMOND:
            out << "DIAMOND";
            break;
        case Suit::CLUB:
            out << "CLUB";
            break;
    }
    return out;
}