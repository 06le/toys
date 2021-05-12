#include <iostream>
#include <functional>

using namespace std;

class DebugDelete
{
    public:
        template <typename T>
        void operator()(ostream& os, T* t) const
        {
            delete t;
            os << "deleting unique_ptr" << endl;
        }
};

int main()
{
    int k = 3;
    int* i = &k;
    DebugDelete d;
    d(cout, i);
    return 0;
}
