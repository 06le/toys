#include <iostream>
#include <random>
#include <vector>
using namespace std;

//17.28
unsigned Get_UniformDistribution_Random()
{
    default_random_engine e;
    uniform_int_distribution<unsigned> u;

    return u(e);
}

//17.29
unsigned Get_UniformDistribution_Random(unsigned seed)
{
    default_random_engine e(seed);
    uniform_int_distribution<unsigned> u;

    return u(e);
}

//17.30
unsigned Get_UniformDistribution_Random(unsigned seed, unsigned min, unsigned max)
{
    default_random_engine e(seed);
    uniform_int_distribution<unsigned> u(min, max);

    return u(e);
}


int main()
{
    default_random_engine e;
    uniform_int_distribution<int> u(0, 9);

    unsigned n1 = Get_UniformDistribution_Random();
    cout << n1 << '\n';

    unsigned n2 = Get_UniformDistribution_Random(13);
    cout << n2 << '\n';

    unsigned n3 = Get_UniformDistribution_Random(55, 0, 9);
    cout << n3 << '\n';

}

