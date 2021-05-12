#include <iostream>
#include <vector>

using namespace std;

template <typename T>
void print(vector<T>& vec)
{
    typename T::size_type i;
    auto size = vec.size();
    for(i = 0; i < size; i++)
    {
        cout << vec[i];
    }
}
template <typename ite>
void print2(vector<ite> vec2)
{
    for(auto beg = vec2.begin(); beg != vec2.end(); beg++)
    {
        cout << *beg;
    }
}

int main()
{
    vector<string> ex{"dfa", "asdkfal"};
    vector<int> ex2{1, 2, 3};
    print(ex);
    cout << endl;
    print2(ex2);
    return 0;
}
