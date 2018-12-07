#include <iostream>
#include <iomanip>
#include <vector>
#include <iterator>
#include <string>
#include <fstream>

template<typename Bidirectional>
bool isPalindrome(Bidirectional first, Bidirectional last);


int main(int argc, const char * argv[])
{
    std::fstream textfile;
    textfile.open("big.txt");

    std::istream_iterator<std::string> start(textfile), end;
    std::vector<std::string> words(start, end);
    unsigned long pcount = 0, wcount = 0;

    for(auto word = words.begin(); word != words.end(); ++word)
    {
        if( isPalindrome(word->begin(), word->end()) && word->size() > 1)
        {
            std::cout << std::setw(4) << pcount << " palindrome found: " << *word << std::endl;
            pcount++;
        }
        wcount++;
    }
    std::cout << "Finished. Palindromes found " << pcount << " out of " << wcount << " words" << std::endl;
    return 0;
}

template<typename Bidirectional>
bool isPalindrome(Bidirectional first, Bidirectional last)
{
    while(true)
    {
        last--;
        if(first == last)
            break;
        if(*first != *last)
            return false;
        first++;
        if(first == last)
            break;
    }
    return true;
}