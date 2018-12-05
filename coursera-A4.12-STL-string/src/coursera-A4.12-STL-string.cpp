//============================================================================
// Name        : 12-STL-string.cpp
// Author      : Lukasz Plewa (l.plewa@adbglobal.com)
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

int main() {
    std::ios::sync_with_stdio(false);
    cout << "STL string and parsing" << endl; // prints STL string and parsing

    std::locale loc("pl_PL.UTF-8");
    std::wcout.imbue(loc); // Use it for output
    wfstream szwFileStr("pan-tadeusz.txt");
    szwFileStr.imbue(loc); // Use it for file input
    std::wstring str_test;
    istream_iterator<std::wstring> start(szwFileStr), end;

    vector<wstring> words(start, end);

    unsigned long wcount = 0;
    for(auto it = words.begin(); it != (words.begin()+200); ++it)
    {
        cout << (*it).size() << "\t:";
        cout << *it << endl;
        wcount++;
    }
/*
    sort(words.begin(), words.end());
    wcount = 0;
    for(auto str: words)
    {
        cout << str << endl;
        wcount++;
    }
*/
    cout << "Read " << wcount << " words from file";
    szwFileStr.close();
return 0;
}
