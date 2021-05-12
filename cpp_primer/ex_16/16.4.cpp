#include <iostream>
#include <vector>
using namespace std;

template <typename T, typename M>
T find(T begin, T end, const M& m)
{
    for( ; begin != end; ++begin)
    {
        if(*begin == m)
        {
            return begin;
        }
    }
    return end;
}

int main()
{
    vector<int> v1{1, 2, 3, 5, 6};
    auto re1 = find(v1.begin(), v1.end(), 4);
    if(re1 != v1.end())
        cout << "找到了" << endl;
    else
        cout << "没找到" << endl;
    return 0;
}
