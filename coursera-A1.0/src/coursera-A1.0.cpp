
#include <iostream>
#include <vector>
using namespace std;

const unsigned int elements_size   = 40;

template <class elem_type>
elem_type sum(vector<elem_type> &table){
    elem_type accumulate = 0;
    for (auto el : table)
        accumulate += el;
    return accumulate;
}

int main()
{
    vector<int> elements;

    for (unsigned int i=0; i < elements_size; i++)
        elements.push_back(i);

    cout << "Sum of " << elements_size << " elements is " << sum(elements) << endl;
    return 0;
}
