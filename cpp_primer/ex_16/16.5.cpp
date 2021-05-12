#include <iostream>

using namespace std;

template <unsigned N, typename T>
void print(const T (&p1)[N])
{
    for(auto i : p1)
        cout << i << endl;
}

int main()
{
    //int a[5] = {1, 2 ,3, 4, 5};
    string a[5] = {"sb", "asd", "asd", "aaa", "asd"};
    print(a);
    return 0;
}

