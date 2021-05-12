#include <iostream>

using namespace std;

template <unsigned N, typename T>
T* begin(T (&p)[N])
{
    return p;
}

template <unsigned N, typename T>
T* end(T (&p)[N])
{
    return p + N;
}
