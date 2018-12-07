#ifndef PIPS_H_
#define PIPS_H_

#include <cassert>
#include <iostream>

enum class Suit {SPADE, HEART, DIAMOND, CLUB};

// ACE - 1
// 2...10
// Jocker - 11
// Queen - 12
// King 13
class Pips {
public:
    Pips(int val) : v(val) { assert(v>0 && v<14); }
    friend std::ostream& operator<<(std::ostream& out, const Pips& p);
    int get_pips() { return v; }
private:
    int v;
};


std::ostream& operator<<(std::ostream& out, const Suit& s);
std::ostream& operator<<(std::ostream& out, const Pips& p);

#endif // PIPS_H_
