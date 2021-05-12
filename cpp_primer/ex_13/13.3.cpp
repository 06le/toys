//13.29
不需要调用拷贝构造函数，而是直接交换指针，所以不会导致递归循环*/

//13.30 --  13.31
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class Hasptr{
    friend void swap(Hasptr&, Hasptr&);
    friend bool operator<(const Hasptr&, const Hasptr&);
public:
    Hasptr(const string &s = string()) : i(0), ps(new string(s)) {}                  //接受一个string的构造函数
    Hasptr(const Hasptr& ex1) : i(ex1.i), ps(new string (*ex1.ps)) {}                //拷贝构造函数
    Hasptr &operator=(const Hasptr &ex1)                                             //拷贝赋值运算符
    {
        i = ex1.i;
        auto newp = new string(*ex1.ps);
        delete ps;
        ps = newp;
        return *this;
    }
    void swap(Hasptr& ex1, Hasptr& ex2) //Hasptr版本swap函数
    {
        using std::swap;
        swap(ex1.i, ex2.i);
        swap(ex1.ps, ex2.ps);
        cout << "当存在类型特定的swap版本，匹配程度优先于std中定义的版本会调用该函数" <<endl;
    }

private:
    int i;
    string *ps;
};
bool operator<(const Hasptr& ex1, const Hasptr& ex2)
{
    return *ex1.ps < *ex2.ps;
}

//13.32
不会，swap函数是避免值进行多余的拷贝降低性能，类指针版本直接交换指针，没有进行多余拷贝操作。
