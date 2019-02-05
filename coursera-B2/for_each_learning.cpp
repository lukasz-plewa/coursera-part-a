//What does the following print

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// old style, without lambda expressions
void inc(int &i) { static int n = 1; i = n++; }
void outvec(int i) { cout << i << endl; }

// Function object
class gen {
public:
    gen(double x_zero, double increment) : x(x_zero), incr(increment) {}
    double operator()() {x += incr; return x * x; }
private:
    double x, incr;
};

double integrate(gen g, int n)
{
    vector<double> fx(n);

    generate(fx.begin(), fx.end(), g);
    return (accumulate(fx.begin(), fx.end(), 0.0) / n);
}

int main()
{
    std::string words[5] = {"my", "hop", "mop", "hope", "cope" };

    string *where;
    where = find(words, words+5, "hop");
    cout << *++where << endl; // mop
    sort(words, words+5);
    where = find(words, words+5, "hop");
    cout << *++where << endl; // hope

    vector<int> v(6);
    for_each(v.begin(), v.end(), inc);
    for_each(v.begin(), v.end(), outvec);

    vector<int> vv(6);
    for_each(vv.begin(), vv.end(), [](int &i) { static int n = 10; i = n++; });
    for_each(vv.begin(), vv.end(), [](int i) { cout << i << endl; });

    cout << endl << "vector copy:" << endl;
    vector<int> vvv(6);
    for_each(vvv.begin(), vvv.end(), [](int i) { cout << i << endl; });
    copy(vv.begin(), vv.end(), vvv.begin());
    for_each(vvv.begin(), vvv.end(), [](int i) { cout << i << endl; });


    double v1[3] = {1.0, 2.5, 4.6 },
           v2[3] = {1.0, 2.0, -3.5};
    double sum, inner_p;

    sum = accumulate(v1, v1+3, accumulate(v2, v2+3, 0.0));
    inner_p = inner_product(v1, v1+3, v2, 0.0);
    cout << "sum = " << sum << ", product = " << inner_p << endl;


    // Function objects
    sum = accumulate(v1, v1+3, 0.0, minus<double>());  // accumulation for binary operation over the array v1[]
    cout << "sum = " << sum << endl;

    //
    const int n = 10000;
    gen g(0.0, 1.0/n);
    cout << "Integration program x**2" << endl;
    cout << integrate(g, n) << endl;
}
