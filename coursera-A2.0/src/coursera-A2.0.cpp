//============================================================================
// Name        : 0.cpp
// Author      : Lukasz Plewa (l.plewa@adbglobal.com)
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>

using namespace std;

//#define ENUM_CLASS_DEMO 1
//#define SUM_ARRAY_DEMO  1
#define GRAPH_STRUCTURE 1

#ifdef ENUM_CLASS_DEMO
enum class days
    {SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY};

ostream& operator<< (ostream &out, const days &d)
{
    out << static_cast<int>(d); return out;
}

days operator++ (days &d)           //PREFIX OPERATOR
{
    d = static_cast<days>((static_cast<int>(d)+1) % 7); return d;
}

days operator++ (days &d, int)      //POSTFIX OPERATOR
{
    days temp = d;
    d = static_cast<days>((static_cast<int>(d)+1) % 7);
    return temp;
}
#endif //ENUM_CLASS_DEMO

#ifdef SUM_ARRAY_DEMO
const unsigned int tab_length = 40;
const double initial = 44.8;

// generic sum of array with initial value
template <class Summable>
Summable sum(const Summable data[], int size, Summable s = 0)
{
    for (int i=0; i<size; i++)
        s += data[i];
    return s;
}

// Minuend      - edjemna
// Subtrahend   - odjemnik
template <class Arithmetic>
Arithmetic substraction(Arithmetic data[], int size, Arithmetic minuend = 0)
{
    for (int i=0; i<size; i++)
        minuend -= data[i];
    return minuend;
}

template <class Printable>
void printArray(Printable data[], int size)
{
    for (int i=0; i<size; i++) {
        if ( !(i%10)) cout << endl;
        cout << data[i] << '\t';
    }
    cout << endl;
}
#endif //SUM_ARRAY_DEMO

#ifdef GRAPH_STRUCTURE

#endif //GRAPH_STRUCTURE

/* ************************************************************************** */
int main() {
#ifdef ENUM_CLASS_DEMO
    cout << "Demonstrate class enum" << endl;
    days today{days::WEDNESDAY};
    cout << "Today value is        " << today << endl;
    cout << "Pre increment value is  " << ++today << endl;
    cout << "Post increment value is " << today++ << endl;
cout << "Today vaue is         " << today << endl;
#endif // ENUM_CLASS_DEMOS

#ifdef SUM_ARRAY_DEMO
    double sumarray_double[tab_length];
    for (unsigned int i=0; i < tab_length; i++)
        sumarray_double[i] = static_cast<double>(i);
    cout << "Sum of " << tab_length << " elements is " << sum(sumarray_double, tab_length) << endl;
    cout << "Sum of " << tab_length << " elements with initial value " << initial << " is " << sum(sumarray_double, tab_length, initial) << endl;

    cout << "Difference of " << tab_length << " elements from " << 100.0 << " is " << substraction(sumarray_double, tab_length, 100.0) << endl;
    cout << "Print table elements:";
    printArray(sumarray_double, tab_length);
#endif //SUM_ARRAY_DEMO

#ifdef GRAPH_STRUCTURE

#endif //GRAPH_STRUCTURE
    return 0;
}
