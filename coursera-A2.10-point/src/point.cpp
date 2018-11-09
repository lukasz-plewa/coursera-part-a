/*
 * point.cpp
 *
 *  Created on: Nov 6, 2018
 *      Author: L.Plewa
 */

#include <iomanip>
#include "point.h"

std::ostream& operator<< (std::ostream& os, const Point& p )
{
    os << std::setprecision( 6 )
       << "(" << p.x << ", " << p.y << ")";
    return os;
}

Point operator+ (Point& first, const Point& second)
{
    Point sum((first.x + second.x), (first.y + second.y));
    return sum;
}


void drawCoordinate(unsigned int size)
{

    std::cout << "Coordinate of size " << size << std::endl;

}
