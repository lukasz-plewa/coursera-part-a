// Converted C program to C++
// change to C++ io
// change to one line comments
// change defines of constants to const
// change array to vector<>
// inline any short function

#include <iostream>
#include <vector>
using namespace std;

const int N = 40;

// The sum integer and vect is passed by reference and changes
// made here reflect in calling function
inline void sum(int& sum, vector<int>& vect)
{
    sum = 0;
    for(auto & element : vect)
        sum += element;
}

int main()
{
    int accum = 0;      // Clear initial accum value
    vector<int> data;   // Empty vector object

    // Populate vector with integer
    for(int i = 0; i < N; ++i)
        data.push_back(i);

    sum(accum, data);

    cout << "sum is " << accum << endl;

    return 0;
}


