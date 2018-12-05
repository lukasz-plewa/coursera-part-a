#include <iostream>
#include <vector>

using namespace std;

int main()
{
    vector<int>* pV = new vector<int>(100);
    cout << "Hello world!" << endl;

    for (vector<int>::iterator a = pV->begin(); a != pV->end(); ++a)
    {
        *a = 6;
    }

    for (auto a: *(pV))
        cout << a << ", ";
    return 0;
}
