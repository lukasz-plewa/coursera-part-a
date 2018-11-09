/*
 * point.h
 *
 *  Created on: Nov 6, 2018
 *      Author: L.Plewa
 */

#ifndef POINT_H_
#define POINT_H_

#include <iostream>



void drawCoordinate(unsigned int size);


class Point
{
    double x, y;
public:
    Point() { this->x = this->y = 0.0; }    // default constructor
    Point(double ix, double iy) : x(ix), y(iy) {}
    double getx() { return x; }             // accessor method
    double gety() { return y; }
    void setx(double value) { x = value; } // mutator method
    void sety(double value) { y = value; }

    friend std::ostream& operator<< (std::ostream& os, const Point& p );
    friend Point operator+ (Point& first, const Point& second);
};


#endif /* POINT_H_ */
