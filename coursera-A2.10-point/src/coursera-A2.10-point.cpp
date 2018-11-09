//============================================================================
// Name        : 10-point.cpp
// Author      : Lukasz Plewa (l.plewa@adbglobal.com)
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <ncurses.h>

#include "point.h"
using namespace std;


enum class modes {RED=1, YELLOW, GREEN, BLUE};

int main() {
    cout << "XY point class and their methods" << endl; // prints XY point class and their methods
    Point p1(2.0, 4.5);

    cout << "My first point p1 is " << p1 << endl;
    Point a(3.5, -2.5), b(2.5, -4.5);
    cout << "a = " << a << ", b = " << b << endl;
    cout << "Sum = " << a + b << endl;

    drawCoordinate(100);

    initscr(); //1
    if( has_colors() == TRUE ) //1
    {
        start_color(); //2
        init_pair( static_cast<int>(modes::RED), COLOR_RED, COLOR_BLACK ); //3
        attron( COLOR_PAIR( static_cast<int>(modes::RED) ) ); //4
        printw( "Kolorowy napis. ;P" );
        attroff( COLOR_PAIR(static_cast<int>(modes::RED) ) ); //Wyłączenie koloru tekstu
    }
    else
    {
        printw( "Twoja Konsolka nie obsluguje kolorow. :/ " );
    }
    getch(); //czekanie na naciśnięcie

    endwin(); //4

    return 0;
}
