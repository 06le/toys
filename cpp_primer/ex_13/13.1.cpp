/*
class Employee {
public:
    Employee();
    Employee(const string& name);

    const int id() const { return id_; }

private:
    string name_;
    int id_;
    static int s_increment;
};
int Employee::s_increment = 0;

Employee::Employee()
{
    id_ = s_increment++;
}

Employee::Employee(const string& name)
{
    id_ = s_increment++;
    name_ = name;
}*/
/*13.1 拷贝构造函数是一个构造函数,定义同类型的一个对象初始化另一个对象时做什么.
       第一个参数是对类类型的引用，其他额外参数都有默认值
       使用它：
       *将一个对象赋给另一个对象时
       *函数形参是非引用类型
       *函数返回类型是非引用类型
       *花括号列表初始化一个数组中的元素或一个聚合类中的成员。
        */

/*13.2 rhs是参数的参数,非引用传参的时候要传值,传值就调用拷贝构造函数,拷贝构造函数又要传值,传值又调用拷贝构造函数...
       形成无限死循环，*/

/*13.3 当我们复制strblob时，共享的ptr成员的use-count添加一个。
       当我们复制strblobprts时，弱指针成员的使用计数不会更改。（因为该计数属于共享指针）*/

/*13.4  Point global;
        Point foo_bar(Point arg) // 1
        {
            Point local = arg, *heap = new Point(global); // 2, 3
            *heap = local;
            Point pa[ 4 ] = { local, *heap }; // 4, 5
        return *heap; // 6
        } */

/*13.5 Hasptr(const &Hasptr ex) : ps(new string (*ex.ps)), i(ex.i) {} */

/*13.6 拷贝赋值运算符本质上是函数，接受一个与其所在类相同类型的参数
       当一个类对象赋值给另一个同类型对象会用到
       合成拷贝运算符将右侧对象的每个非static成员赋予给左侧对象相应的成员
       类未定义自己的拷贝运算符就会生成合成拷贝运算符*/

/*13.7 都会发生浅拷贝，计数与13.3相同*/

/*13.8 Hasptr operator=(const &Hasptr ex) {ps = *ex.ps; i = *ex.i,; return *this;}*/

/*13.9 析构函数是成员函数，不接受参数，无返回值，不能重载，先执行函数体，再销毁成员。
       对于某些类，合成析构函数被用来组织该类型的对象销毁，否则合成析构函数体为空，销毁成员。
       */

/*13.10 当strblob对象被销毁时，动态对象的使用计数将减少。如果该动态对象没有共享指针，它将被释放。
        当strblobpter对象被销毁时，动态分配的对象将不会被释放。*/

/*13.11 ~Hasptr(){delete ps;}*/

/*13.12 3次 accum, item1, item2*/

/*13.13 #include <iostream>
#include <vector>
using namespace std;
struct X {
    X() {cout << "构造函数" << endl;}
    X(const X&) {cout << "拷贝构造" << endl;}
    ~X() {cout << "析构函数" << endl;}
    X& operator= (const X& ) {cout << "赋值运算符" << endl; return *this;};

};
void fun (X& x)
{
    cout << "函数调用" << endl;
}
int main()
{
    X x1;
    vector <X> ex1;
    ex1.push_back(x1);
    auto a2 = new X (x1);
    delete a2;
    fun(x1);
    return 0;
}*/

/*13.14 输出相同mysn*/

/*13.15 会改变上一题中调用的输出结果，因为我们定义了一个拷贝构造函数能生成新的序号。输出结果是三个不同的mysn。*/

/*13.16 会改变，因为参数是引用，调用赋值运算符，但是函数没有任何赋值运算符，输出结果是相同的。*/

/*13.17 // from Github pezy
//13.14
#include <iostream>
class numbered {
public:
    numbered()
    {
        static int unique = 10;
        mysn = unique++;
    }
    int mysn;
};
void f(numbered s)
{
    std::cout << s.mysn << std::endl;
}
int main()
{
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}
//13.15
#include <iostream>
class numbered {
public:
    numbered()
    {
        static int unique = 10;
        mysn = unique++;
    }
    numbered(const numbered& n) { mysn = n.mysn + 1; }
    int mysn;
};
void f(numbered s)
{
    std::cout << s.mysn << std::endl;
}
int main()
{
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}
//13.16
#include <iostream>
class numbered {
public:
    numbered()
    {
        static int unique = 10;
        mysn = unique++;
    }
    numbered(const numbered& n) { mysn = n.mysn + 1; }
    int mysn;
};
void f(const numbered& s)
{
    std::cout << s.mysn << std::endl;
}
int main()
{
    numbered a, b = a, c = b;
    f(a);
    f(b);
    f(c);
}*/

/*13.18
#include <iostream>

using namespace std;

class Employee{
public:
    Employee();
    Employee(const string &n1) : name(n1) {pe_card = tem++;}
private:
    string name;
    int pe_card;
    static int tem;
};
int Employee::tem = 0;
Employee::Employee() {pe_card = tem++;}
int main()
{

    return 0;
}*/

/*13.19 不需要，因为现实世界员工无法复制*/

